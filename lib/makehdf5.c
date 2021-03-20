#include <stdlib.h>
#include <string.h>

#include <zip.h>
#include <hdf5/serial/hdf5_hl.h>

#include "plexostables.h"
#include "makehdf5.h"

#define H5PLEXOS_VERSION "v0.6.0"

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

void add_configs(hid_t f) {

    H5LTset_attribute_string(f, "/", "h5plexos", H5PLEXOS_VERSION);

    struct plexosConfig** rows = *(tables[config].rows);
    size_t n_rows = tables[config].count;

    for (size_t i = 0; i < n_rows; i++) {
        H5LTset_attribute_string(f, "/", rows[i]->element, rows[i]->value);
    }

}

void add_collections(hid_t meta, int compressionlevel) {

    size_t n_collections = tables[collection].count;
    size_t n_memberships = tables[membership].count;

    for (size_t m = 0; m < n_memberships; m++) {

        struct plexosMembership* membership = data.memberships[m];
        struct plexosCollection* collection = membership->collection.ptr;
        size_t c_m = membership->collection_membership_idx;

        if (collection->rows == NULL) {
            collection->rows = calloc(collection->nmembers, sizeof(struct plexosMembershipRow));
        }
        struct plexosMembershipRow* row = &(collection->rows[c_m]);

        if (collection->isobjects) {
            strcpy(row->first, membership->childobject.ptr->name);
            strcpy(row->second, membership->childobject.ptr->category.ptr->name);
        } else {
            strcpy(row->first, membership->parentobject.ptr->name);
            strcpy(row->second, membership->childobject.ptr->name);
        }

    }

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

        struct plexosCollection* collection = data.collections[c];

        if (collection->nmembers > 0) {

            if (collection->isobjects) {
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
                collection->h5name, table_group, collection->h5name,
                n_membershipfields, collection->nmembers, sizeof(struct plexosMembershipRow),
                field_names, field_offsets, field_types, collection->nmembers,
                NULL, compressionlevel, collection->rows);

        }

    }

    H5Gclose(objects);
    H5Gclose(relations);
    H5Tclose(string_type);

}

void add_times(hid_t meta, const char* localformat, int compressionlevel) {

    // PLEXOS data format notes:
    // Period type 0 - phase-native interval / block data
    // Maps to ST periodtype 0 via relevant phase table
    // Store both block and interval results on disk (if not ST)?

    // Period type 1-7 - period-type-specific data
    // Direct mapping to period labels

    hid_t times =
        H5Gcreate2(meta, "times", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

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

void add_values(hid_t dat, int compressionlevel) {

    for (size_t i = 0; i < tables[key_index].count; i++) {

        struct plexosKeyIndex* ki = data.keyindices[i];

        hid_t dset = dataset(dat, ki, compressionlevel);

        // Just create the datasets for now, populate later
        // double* values = resultvalues(resultdata, ki.periodtype, ki.position, ki.length)
        // hid_t mem_space, file_space =
        //     _(ki.key.band, keyindex->key.ptr->membership.ptr->collection_membership_idx)

        // H5Dwrite(dset, mem_type, mem_space, file_space, xfer_plist, values)
        H5Dclose(dset);

    }

}

hid_t dataset(hid_t dat, struct plexosKeyIndex* ki, int compressionlevel) {

    struct plexosCollection* collection = ki->key.ptr->membership.ptr->collection.ptr;
    struct plexosProperty* property = ki->key.ptr->property.ptr;

    struct plexosTable* phase = get_nominal_phase(ki->key.ptr->phase);
    struct plexosTable* period = get_nominal_period(ki->periodtype);

    bool is_summarydata = property->issummary && ki->periodtype != 0;
    char* collection_name = collection->h5name;
    char* property_name = is_summarydata ? property->summaryname : property->name;

    // TODO: Create phase, period, collection groups if needed


    hid_t dset;
    if (H5LTfind_dataset(g, property_name)) {
        dset = _
    } else {
        // TODO: Create property dataset if needed
        // (property->nbands, ki->length, collection->nmembers)
        // H5LTmake_dataset_double(dat, name, 3, dims, values)
        // set period_offset = ki->periodoffset,
        // set units = property->summaryunit.ptr->value or property->unit.ptr->value
        dset = _
    }

    return dset

}

char default_localformat[20] = "%d/%m/%Y %T";

void create_hdf5(zip_t* archive, int* err, const char* outfile) {

    // TODO: set these runtime arguments
    int compressionlevel = 1;
    char* timeformat = &default_localformat;

    hid_t f = H5Fcreate(outfile, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    add_configs(f);

    hid_t meta =
        H5Gcreate2(f, "metadata", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    add_collections(meta, compressionlevel);
    add_times(meta, timeformat, compressionlevel);
    H5Gclose(meta);

    hid_t dat =
        H5Gcreate2(f, "data", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //add_values(dat, compressionlevel);
    H5Gclose(dat);

    H5Fclose(f);

}
