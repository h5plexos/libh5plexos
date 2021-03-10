#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <zip.h>
#include <expat.h>

#include "plexostables.h"

#define XMLBUFFERSIZE 32768
#define MAXIDXLENGTH 7

enum xmlDepth { root, row, rowData, n_xmllevels };

struct parserState {
    int depth;
    struct plexosTable* table;
    bool idxField;
    char row_idx[MAXIDXLENGTH];
    size_t row_idx_length;
};

struct parserState state = {};

static void XMLCALL start(void* data, const XML_Char* el, const XML_Char** attr) {
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
                        exit(-1); 

    }

    state.depth++;

}

static void XMLCALL text(void* data, const XML_Char* s, int n) {

    if (state.idxField) {

        state.row_idx_length += n;

        if (state.row_idx_length > MAXIDXLENGTH) {
            fprintf(stderr,
                    "Encountered a PLEXOS entity index longer than "
                    "%d characters: '%s%.*s'",
                    MAXIDXLENGTH, state.row_idx, n, s);
            exit(-1);
        }

        strncat(state.row_idx, s, n);

    }

}

static void XMLCALL end(void* data, const XML_Char* el) {
    (void)data;
    (void)el;

    if (state.idxField) {

        int idx = atoi(state.row_idx);

        if (!state.table->zeroindexed) {
            idx--;
        }

        if (idx > state.table->max_idx) {
            state.table->max_idx = idx;
        }

        state.idxField = false;
        state.row_idx[0] = '\0';
        state.row_idx_length = 0;

    }

    state.depth--;

}

void h5plexos(const char* infile, const char* outfile) {

    int err = 0;
    zip_t* archive = zip_open(infile, ZIP_RDONLY, &err);
    if (archive == NULL) {
        fprintf(stderr, "Error %d occured when loading zip file %s\n", err, infile);
        return;
    }

    /* TODO: Obviously, don't hardcode the XML file name...  */
    zip_int64_t xml_idx = zip_name_locate(archive, "Model Base_8200 Solution.xml", 0);
    if (xml_idx == -1) {
        fprintf(stderr, "The XML file could not be found in the archive. Are you sure this is a PLEXOS output?\n");
        return;
    }

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
    XML_SetElementHandler(p, start, end);
    XML_SetCharacterDataHandler(p, text);

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

    zip_discard(archive);

    printf("Count\tMax Idx\tTable\n");
    printf("=====\t=======\t=====\n");
    for (int i = 0; i < n_plexostables; i++) {
        printf("%d\t%d\t%s\n",
               tables[i].count, tables[i].max_idx, tables[i].name);
    }

    return;

}
