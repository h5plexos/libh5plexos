#include <stdlib.h>
#include <string.h>

#include <zip.h>
#include <hdf5/serial/hdf5_hl.h>

#include "plexostables.h"
#include "makehdf5.h"

#define H5PLEXOS_VERSION "v0.6.0"
#define MAXSTRINGLENGTH 255 // TODO: Generalize this with the XML length

enum membershipRowFields {
    first, second, n_membershipfields
};

struct plexosMembershipRow {
    char first[MAXSTRINGLENGTH];
    char second[MAXSTRINGLENGTH];
};

const char* object_names[n_membershipfields] = {
    [first] = "Name", [second] = "Category"
};

const char* relation_names[n_membershipfields] = {
    [first] = "Parent", [second] = "Child"
};

size_t field_offsets[n_membershipfields] = {
    [first] = HOFFSET(struct plexosMembershipRow, first),
    [second] = HOFFSET(struct plexosMembershipRow, second)
};

struct plexosCollectionTable {
    struct plexosMembershipRow* rows;
    size_t nrows;
    char name[MAXSTRINGLENGTH];
    bool is_objects;
};

void strcat_nospaces(char* dest, char* src) {
    for (size_t i = 0; i < strlen(src); i++) {
        if (src[i] != ' ') strncat(dest, src + i, 1);
    }
}

void set_collection_name(
    struct plexosCollectionTable* table, struct plexosCollection* coll) {

    table->name[0] = '\0';
    size_t namesize = 0;

    if (!table->is_objects) {

        char* prefix = strcmp(coll->complementname, "") == 0 ?
            coll->parentclass.ptr->name : coll->complementname;

        namesize = strlen(prefix);

        if (namesize + 1 <= MAXSTRINGLENGTH) {
            strcat_nospaces(table->name, prefix);
            strcat(table->name, "_");
        } else {
            // This is not strictly true, since we're counting the spaces
            // against the length but not actually storing them
            fprintf(stderr, "Encountered a collection prefix longer than %d characters: %s\n",
                    MAXSTRINGLENGTH, prefix);
            exit(EXIT_FAILURE);
        }
    }

    namesize += strlen(coll->name);
    if (namesize <= MAXSTRINGLENGTH) {
        strcat_nospaces(table->name, coll->name);
    } else {
        // This is not strictly true, since we're counting the spaces against
        // the the length but not actually storing them
        fprintf(stderr, "Encountered a collection name longer than %d characters: %s%s\n",
            MAXSTRINGLENGTH, table->name, coll->name);
        exit(EXIT_FAILURE);
    }

}

size_t find_collection(struct plexosCollection* coll) {
    for (size_t c = 0; c < tables[collection].count; c++) {
        if (coll == data.collections[c]) {
            return c;
        }
    }
    fprintf(stderr, "Could not find collection %s\n", coll->name);
    exit(EXIT_FAILURE);
}

void add_configs(hid_t f) {

    H5LTset_attribute_string(f, "/", "h5plexos", H5PLEXOS_VERSION);

    struct plexosConfig** rows = *(tables[config].rows);
    size_t n_rows = tables[config].count;

    for (size_t i = 0; i < n_rows; i++) {
        H5LTset_attribute_string(f, "/", rows[i]->element, rows[i]->value);
    }

}

void add_collections(hid_t f, int compressionlevel) {

    size_t n_collections = tables[collection].count;
    size_t n_memberships = tables[membership].count;

    struct plexosCollectionTable* collection_memberships =
        calloc(n_collections, sizeof(struct plexosCollectionTable));
    memset(collection_memberships, 0, n_collections * sizeof(struct plexosCollectionTable));

    for (size_t m = 0; m < n_memberships; m++) {
        struct plexosMembership* membership = data.memberships[m];
        size_t c = find_collection(membership->collection.ptr);
        data.memberships[m]->collection_idx = c;
        data.memberships[m]->collection_membership_idx = collection_memberships[c].nrows;
        collection_memberships[c].nrows++;
    }

    for (size_t c = 0; c < n_collections; c++) {

        struct plexosCollectionTable* table = &(collection_memberships[c]);
        struct plexosCollection* coll = data.collections[c];

        table->is_objects = strcmp(coll->parentclass.ptr->name, "System") == 0;
        table->rows = calloc(table->nrows, sizeof(struct plexosMembershipRow));
        set_collection_name(table, coll);

    }

    for (size_t m = 0; m < n_memberships; m++) {

        size_t c = data.memberships[m]->collection_idx;
        size_t c_m = data.memberships[m]->collection_membership_idx;
        struct plexosCollectionTable* table = &collection_memberships[c];
        struct plexosMembershipRow* row = &table->rows[c_m];

        if (table->is_objects) {
            strcpy(row->first, data.memberships[m]->childobject.ptr->name);
            strcpy(row->second, data.memberships[m]->childobject.ptr->category.ptr->name);
        } else {
            strcpy(row->first, data.memberships[m]->parentobject.ptr->name);
            strcpy(row->second, data.memberships[m]->childobject.ptr->name);
        }

    }

    hid_t meta =
        H5Gcreate2(f, "metadata", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t objects =
        H5Gcreate2(meta, "objects", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t relations =
        H5Gcreate2(meta, "relations", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t string_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(string_type, MAXSTRINGLENGTH);

    hid_t field_types[n_membershipfields];
    field_types[first] = string_type;
    field_types[second] = string_type;

    hid_t table_group;
    const char** field_names;

    for (size_t c = 0; c < n_collections; c++) {

        struct plexosCollectionTable table = collection_memberships[c];

        if (table.nrows > 0) {

            if (table.is_objects) {
                table_group = objects;
                field_names = object_names;
            } else {
                table_group = relations;
                field_names = relation_names;
            }

            // This sets the maximum dataspace size to infinite (the current
            // size is of course table.nrows). Not a big deal, but might be
            // nice to keep current == max
            H5TBmake_table(
                table.name, table_group, table.name,
                n_membershipfields, table.nrows, sizeof(struct plexosMembershipRow), 
                field_names, field_offsets, field_types, table.nrows,
                NULL, compressionlevel, table.rows);

        }

    }

    H5Tclose(string_type);
    H5Gclose(meta);
    H5Gclose(objects);
    H5Gclose(relations);

}

void add_times(hid_t f, const char* localformat, int compressionlevel) {

    // PLEXOS data format notes:
    // Period type 0 - phase-native interval / block data
    // Maps to ST periodtype 0 via relevant phase table
    // Store both block and interval results on disk (if not ST)?

    // Period type 1-7 - period-type-specific data
    // Direct mapping to period labels

    hid_t times =
        H5Gcreate2(f, "metadata/times", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t string_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(string_type, 20);


    for (size_t i_t = 0; i_t < n_plexosperiods; i_t++) {

        struct plexosTable table = tables[period_tables[i_t]];
        void** periodrows = *(table.rows);
        char* period_timestamps = calloc(table.count, sizeof(char[20]));
        memset(period_timestamps,'\0', table.count * sizeof(char[20]));

        for (size_t i = 0; i < table.count; i++) {
            table.puttimestamp(period_timestamps + i*sizeof(char[20]),
                               periodrows[i], localformat);
        }

        H5LTmake_dataset(times, table.h5name, 1, &table.count,
                         string_type, period_timestamps);

    }

    H5Gclose(times);

}

char default_localformat[20] = "%d/%m/%Y %T";

void create_hdf5(zip_t* archive, int* err, const char* outfile) {

    // TODO: set these runtime arguments
    int compressionlevel = 1;
    char* timeformat = &default_localformat;

    hid_t f = H5Fcreate(outfile, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    add_configs(f);
    add_collections(f, compressionlevel);
    add_times(f, timeformat, compressionlevel);
    // add_values

    H5Fclose(f);

}

