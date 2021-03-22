#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <zip.h>

#include "plexostables.h"
#include "parsexml.h"
#include "makehdf5.h"

void h5plexos(const char* infile, const char* outfile) {

    int err = 0;
    zip_t* archive = zip_open(infile, ZIP_RDONLY, &err);
    if (archive == NULL) {
        fprintf(stderr, "Error %d occured when loading zip file %s\n", err, infile);
        return;
    }

    const char* infile_name = strrchr(infile,  '/');
    infile_name = infile_name == NULL ? infile : &(infile_name[1]);
    size_t infile_length = strlen(infile_name);
    char xml_name[infile_length+1];
    strncpy(xml_name, infile_name, infile_length-3);
    xml_name[infile_length-3] = '\0';
    strcat(xml_name, "xml");
    printf("Looking for %s inside zip archive\n", xml_name);

    zip_int64_t xml_idx = zip_name_locate(archive, xml_name, 0);
    if (xml_idx == -1) {
        fprintf(stderr, "'%s' could not be found in the archive. "
                        "Are you sure this is a PLEXOS output?\n", xml_name);
        return;
    }

    parse(archive, &err, xml_idx, summary_pass);

    printf("Count\tMax Idx\tTable\n");
    printf("=====\t=======\t=====\n");
    for (int i = 0; i < n_plexostables; i++) {
        printf("%d\t%d\t%s\n",
               tables[i].count, tables[i].max_idx, tables[i].name);
    }

    // libray invocations may init multiple times, reset_data instead?
    // also reset tables and parser state?
    init_data();
    parse(archive, &err, xml_idx, populate_pass);

    char fname[13];
    struct zip_stat stat = {};
    for (size_t i = 0; i < 8; i++) {

        sprintf(fname, "t_data_%u.BIN", i);
        zip_int64_t bin_idx = zip_name_locate(archive, fname, 0);

        if (bin_idx >= 0) {

            zip_file_t* bin = zip_fopen_index(archive, bin_idx, 0);
            if (bin== NULL) {
                fprintf(stderr, "Error %d occured when opening %s.\n", err, fname);
                return;
            }

            zip_stat_index(archive, bin_idx, 0, &stat);
            printf("%s\t%u bytes\n", fname, stat.size);
            data.values[i] = malloc(stat.size);
            zip_int64_t n = zip_fread(bin, data.values[i], stat.size);

            if (n < stat.size) {
                fprintf(stderr, "Only read %u bytes from %u byte file\n", n, stat.size);
                exit(EXIT_FAILURE);
            }

        }

    }

    finalize_data();
    create_hdf5(archive, &err, outfile);
    zip_discard(archive);

    return;

}
