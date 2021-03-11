#ifndef plexostables_h_INCLUDED
#define plexostables_h_INCLUDED

#include <stdbool.h>

enum plexosTableIdx {
    config, unit, timeslice, model, band, sample, sample_weight,
    class_group, class, category, attribute, collection, property,
    object, membership, attribute_data,
    period_0, period_1, period_2, period_3, period_4, period_6, period_7,
    phase_2, phase_3, phase_4, key, key_index, n_plexostables
};

typedef void populate_function(void* row, const char* field, const char* value);

struct plexosTable {

    int idx;
    char* name;
    char* id;
    bool zeroindexed;

    int count;
    int max_idx;

    size_t rowsize;
    populate_function* populator;
    void*** rows;

};

struct plexosTable tables[n_plexostables];

struct plexosTable* get_plexostable(const char* tablename);

// PLEXOS table row data

struct plexosConfig {
    char* element;
    char* value;
};

struct plexosUnit {
    char* value;
};

struct plexosTimeslice {
    char* name;
};

struct plexosModel {
    char* name;
};

struct plexosBand {
};

struct plexosSample {
    char* name; // sample_name
};

struct plexosSampleWeight {
    int phase; // phase_id
    double value;
    struct plexosSample* sample; // sample_id
};

struct plexosClassGroup {
    char* name;
};

struct plexosClass {
    char* name;
    int state;
    struct plexosClassGroup* classgroup; // class_group_id
};

struct plexosCategory {
    char* name;
    int rank;
    struct plexosClass* class; // class_id
};

struct plexosAttribute {
    char* name;
    char* description;
    int enum_; // enum_id
    struct plexosClass* class; // class_id
};

struct plexosCollection {
    char* name;
    char* complementname; // complement_name
    struct plexosClass* parentclass; // parent_class_id
    struct plexosClass* childclass; // child_class_id
};

struct plexosProperty {
    char* name;
    char* summaryname; // summary_name
    int enum_; // enum_id
    bool ismultiband; // is_multi_band
    bool isperiod; // is_period
    bool issummary; // is_summary
    struct plexosUnit* unit; // unit_id
    struct plexosUnit* summaryunit; // summary_unit_id
    struct plexosCollection* collection; // collection_id
};

struct plexosObject {
    char* name;
    int index;
    bool show;
    struct plexosClass* class; // class_id
    struct plexosCategory* category; // category_id
};

struct plexosMembership {
    struct plexosClass* parentclass; // parent_class_id
    struct plexosClass* childclass; // child_class_id
    struct plexosCollection* collection; // collection_id
    struct plexosObject* parentobject; // parent_object_id
    struct plexosObject* childobject; // child_object_id
};

struct plexosAttributeData {
    double value;
    struct plexosAttribute* attribute; // attribute_id
    struct plexosObject* object; // object_id
};

struct plexosPeriod0 {
    int periodofday; // period_of_day
    int hour; // hour_id
    int day; // day_id
    int week; // week_id
    int month; // month_id
    int quarter; // quarter_id
    int fiscalyear; // fiscal_year_id
    char* datetime;
};

struct plexosPeriod1 {
    int week; // week_id
    int month; // month_id
    int quarter; // quarter_id
    int fiscalyear; // fiscal_year_id
    char* date;
};

struct plexosPeriod2 {
    char* weekending; // week_ending
};

struct plexosPeriod3 {
    char* monthbeginning; // month_beginning
};

struct plexosPeriod4 {
    char* yearending; // year_ending
};

struct plexosPeriod6 {
    int day; // day_id
    char* datetime;
};

struct plexosPeriod7 {
    char* quarterbeginning; // quarter_beginning
};

struct plexosPhase2 {
    int period; // period_id
    struct plexosPeriod0* interval; // interval_id
};

struct plexosPhase3 {
    int period; // period_id
    struct plexosPeriod0* interval; // interval_id
};

struct plexosPhase4 {
    int period; // period_id
    struct plexosPeriod0* interval; // interval_id
};

struct plexosKey {
    int phase; // phase_id
    // note this periodtype is not accurate, use KeyIndex.periodtype instead
    int periodtype; // period_type_id - 
    int band; // band_id
    struct plexosMembership* membership; // membership_id
    struct plexosModel* model; // model_id
    struct plexosProperty* property; // property_id
    struct plexosSample* sample; // sample_id
    struct plexosTimeslice* timeslice; //timeslice_id
};

struct plexosKeyIndex {
    int periodtype; // period_type_id
    int position; // bytes from binary file start
    int length; // in 8-byte (64-bit float) increments
    int periodoffet; // period_offset // temporal data offset (if any) in stored times
    struct plexosKey* key; // key_id
};

/*void populate_x(void* row, plexosTableIdx table,
              const char* fieldname, const char* value); */


struct plexosData {

    struct plexosConfig* configs;
    struct plexosUnit* units;
    struct plexosTimeslice* timeslices;
    struct plexosModel* models;
    struct plexosBand* bands;

    struct plexosSample* samples;
    struct plexosSampleWeight* sampleweights;

    struct plexosClassGroup* classgroups;
    struct plexosClass* classes;
    struct plexosCategory* categories;

    struct plexosAttribute* attributes;
    struct plexosCollection* collections;
    struct plexosProperty* properties;

    struct plexosObject* objects;
    struct plexosMembership* memberships;
    struct plexosAttributeData* attributedata;

    struct plexosPeriod0* intervals;
    struct plexosPeriod1* days;
    struct plexosPeriod2* weeks;
    struct plexosPeriod3* months;
    struct plexosPeriod4* years;
    struct plexosPeriod6* hours;
    struct plexosPeriod7* quarters;

    struct plexosPhase2* pasa;
    struct plexosPhase3* mt;
    struct plexosPhase4* st;

    struct plexosKey* keys;
    struct plexosKeyIndex* keyindices;

};

struct plexosData data;

#endif // plexostables_h_INCLUDED
