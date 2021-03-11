#include <stdlib.h>
#include <stdio.h>

#include <zip.h>

#include "plexostables.h"
#include "parsexml.h"

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
        fprintf(stderr, "The XML file could not be found in the archive. "
                        "Are you sure this is a PLEXOS output?\n");
        return;
    }

    parse(archive, &err, xml_idx, summary_pass);
    // parse(archive, xml_idx, data_pass);
    zip_discard(archive);

    // Now do things with [summary] data
    printf("Count\tMax Idx\tTable\n");
    printf("=====\t=======\t=====\n");
    for (int i = 0; i < n_plexostables; i++) {
        printf("%d\t%d\t%s\n",
               tables[i].count, tables[i].max_idx, tables[i].name);
    }

    // Set up pointers to index values (need to cast placeholder pointers to ints)
    // Consolidate data (eliminate gaps between elements then reallocarray)
    // Create and populate HDF5 file

    return;

}
