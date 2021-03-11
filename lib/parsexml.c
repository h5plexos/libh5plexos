#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <zip.h>
#include <expat.h>

#include "plexostables.h"
#include "parsexml.h"

#define XMLBUFFERSIZE 32768
#define XMLMAXTEXTLENGTH 255

enum xmlDepth { root, row, rowData, n_xmllevels };

struct parserState {

    int depth;
    struct plexosTable* table;

    bool idxField;
    char row_field[XMLMAXTEXTLENGTH + 1];

    char row_value[XMLMAXTEXTLENGTH + 1];
    size_t row_value_length;

    void* row_data;

};

struct parserState state = {};

void parse(zip_t* archive, int* err, zip_int64_t xml_idx, struct parseSpec spec) {

    zip_file_t* xml = zip_fopen_index(archive, xml_idx, 0);
    if (xml == NULL) {
        fprintf(stderr, "Error %d occured when opening internal XML file.\n", err);
        return;
    }

    XML_Parser p = XML_ParserCreate(NULL);
    if (p == NULL) {
        fprintf(stderr, "Error occured when allocating XML parser.\n");
        return;
    }
    XML_SetElementHandler(p, spec.start, spec.end);
    XML_SetCharacterDataHandler(p, spec.text);

    char buf[XMLBUFFERSIZE];
    bool done = false;
    int n;

    while (!done) {

        n = zip_fread(xml, buf, XMLBUFFERSIZE);

        if (n <= 0) {
            done = true;
        }

        if (XML_Parse(p, buf, n, done) == XML_STATUS_ERROR) {
            fprintf(stderr,
                    "XML parse error at line %lu:\n%s\n",
                    XML_GetCurrentLineNumber(p), XML_ErrorString(XML_GetErrorCode(p)));
            return;
        }

    }
    XML_ParserFree(p);

    int ferr = zip_fclose(xml);
    if (ferr != 0) {
        fprintf(stderr, "Error %d occured when closing internal XML file.\n", ferr);
        return;
    }

}

// Summary (first pass) parsing

static void XMLCALL summary_start(void* data, const XML_Char* el, const XML_Char** attr) {
    (void)data;

    switch (state.depth) {

        case root:      if (strcmp(el, "SolutionDataset") != 0) {
                            fprintf(stderr, 
                                    "Unrecognized PLEXOS XML data format "
                                    "(root tag is not SolutionDataset)\n");
                            exit(-1);
                        }
                        break;

        case row:       state.table = get_plexostable(el);

                        if (state.table == NULL) {
                            fprintf(stderr, "Error parsing row in %s\n", el);
                            exit(-1);
                        }

                        if (state.table->count == 0) {
                            state.table->max_idx = -1;
                        }

                        state.table->count += 1;

                        if (state.table->id == NULL) {
                            state.table->max_idx += 1;
                        }

                        break;

        case rowData:   if (state.table->id != NULL) {
                            state.idxField =
                                (strcmp(el, state.table->id) == 0);
                        }
                        break;

        default:        fprintf(stderr,
                                "Unrecognized PLEXOS XML data format "
                                "(more than 3 levels of tags)\n");
                        exit(EXIT_FAILURE); 

    }

    state.depth++;

}

static void XMLCALL summary_text(void* data, const XML_Char* s, int n) {

/*    if (state.depth != rowData) {
        fprintf(stderr, "Unrecognized PLEXOS XML data format "
                        "(text data outside level %d of tags)\n", rowData);
        exit(EXIT_FAILURE);
    }*/

    if (state.idxField) {

        state.row_value_length += n;

        if (state.row_value_length > XMLMAXTEXTLENGTH) {
            fprintf(stderr,
                    "Encountered a PLEXOS entity index longer than "
                    "%d characters: '%s%.*s'",
                    XMLMAXTEXTLENGTH, state.row_value, n, s);
            exit(EXIT_FAILURE);
        }

        strncat(state.row_value, s, n);

    }

}

static void XMLCALL summary_end(void* data, const XML_Char* el) {
    (void)data;
    (void)el;

    if (state.idxField) {

        int idx = atoi(state.row_value);

        if (!state.table->zeroindexed) {
            idx--;
        }

        if (idx > state.table->max_idx) {
            state.table->max_idx = idx;
        }

        state.idxField = false;
        state.row_value[0] = '\0';
        state.row_value_length = 0;

    }

    state.depth--;

}

struct parseSpec summary_pass = {
    .start = summary_start,
    .text = summary_text,
    .end = summary_end,
};


// Data (second pass) parsing

static void XMLCALL data_start(
    void* data, const XML_Char* el, const XML_Char** attr) {

    switch (state.depth) {

        case root:      if (strcmp(el, "SolutionDataset") != 0) {
                            fprintf(stderr, 
                                    "Unrecognized PLEXOS XML data format "
                                    "(root tag is not SolutionDataset)\n");
                            exit(EXIT_FAILURE);
                        }
                        break;

        case row:       state.table = get_plexostable(el);

                        if (state.table == NULL) {
                            fprintf(stderr, "Error parsing row in %s\n", el);
                            exit(EXIT_FAILURE);
                        }

                        state.row_data = malloc(state.table->rowsize);

                        break;

        case rowData:   if (strlen(el) <= XMLMAXTEXTLENGTH) {
                            strcpy(state.row_field, el);
                        } else {
                            fprintf(stderr,
                                "Encountered a PLEXOS entity field longer than "
                                "%d characters: %s'",
                                XMLMAXTEXTLENGTH, el);
                            exit(EXIT_FAILURE);
                        }

                        break;

        default:        fprintf(stderr,
                                "Unrecognized PLEXOS XML data format "
                                "(more than %d levels of tags)\n", n_xmllevels);
                        exit(EXIT_FAILURE); 

    }

    state.depth++;

}

static void XMLCALL data_text(void* data, const XML_Char* s, int n) {
    switch (state.depth) {

        case rowData:   // TODO: concat into holding string

                        break;

/*        default:        fprintf(stderr,
                                "Unrecognized PLEXOS XML data format "
                                "(text data outside level %d of tags)\n",
                                rowData);
                        exit(EXIT_FAILURE);*/

    }

}

static void XMLCALL data_end(void* data, const XML_Char* el) {

    if (strlen(state.row_field) > 0) {

        if (state.row_field == state.table->id) {
            size_t idx = atoi(state.row_value);
            (*(state.table->rows))[idx] = state.row_data;
        } else {
            (state.table->populator)(
                state.row_data, state.row_field, state.row_value);
        }

        state.row_value[0] = '\0';

    }

}

struct parseSpec data_pass = {
    .start = data_start,
    .text = data_text,
    .end = data_end,
};
