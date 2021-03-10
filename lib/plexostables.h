#ifndef plexostables_h_INCLUDED
#define plexostables_h_INCLUDED

enum plexostable_idxs {
    config, unit, timeslice, model, band, sample, sample_weight,
    class_group, class, category, attribute, collection, property,
    object, membership, attribute_data,
    period_0, period_1, period_2, period_3, period_4, period_6, period_7,
    phase_2, phase_3, phase_4, key, key_index, n_plexostables
};

struct plexosTable {
    char* name;
    char* id;
    bool zeroindexed;
    int count;
    int max_idx;
};

struct plexosTable tables[n_plexostables];

struct plexosTable* get_plexostable(const char* tablename);

#endif // plexostables_h_INCLUDED

