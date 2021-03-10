#include <stdbool.h>
#include <string.h>

#include "plexostables.h"

struct plexosTable tables[n_plexostables] = {

    [config] = {.name = "t_config"},

    [unit] = {.name = "t_unit",
              .id = "unit_id",
              .zeroindexed = true},

    [timeslice] = {.name = "t_timeslice",
                   .id = "timeslice_id",
                   .zeroindexed = true},

    [model] = {.name = "t_model",
               .id = "model_id"},

    [band] = {.name = "t_band"},

    [sample] = {.name = "t_sample",
                .id = "sample_id",
                .zeroindexed = true},

    [sample_weight] = {.name = "t_sample_weight"},

    [class_group] = {.name= "t_class_group",
                     .id = "class_group_id"},

    [class] = {.name = "t_class",
               .id = "class_id"},

    [category] = {.name = "t_category",
                  .id = "category_id"},

    [attribute] = {.name = "t_attribute",
                   .id = "attribute_id"},

    [collection] = {.name = "t_collection",
                    .id = "collection_id"},

    [property] = {.name = "t_property",
                  .id = "property_id"},

    [object] = {.name = "t_object",
                .id = "object_id"},

    [membership] = {.name = "t_membership",
                    .id = "membership_id"},

    [attribute_data] = {.name = "t_attribute_data"},

    [period_0] = {.name = "t_period_0",
                  .id = "interval_id"},

    [period_1] = {.name = "t_period_1",
                  .id = "day_id"},

    [period_2] = {.name = "t_period_2",
                  .id = "week_id"},

    [period_3] = {.name = "t_period_3",
                  .id = "month_id"},

    [period_4] = {.name = "t_period_4",
                  .id = "fiscal_year_id"},

    [period_6] = {.name = "t_period_6",
                  .id = "hour_id"},

    [period_7] = {.name = "t_period_7",
                  .id = "quarter_id"},

    [phase_2] = {.name = "t_phase_2"},

    [phase_3] = {.name = "t_phase_3"},

    [phase_4] = {.name = "t_phase_4"},

    [key] = {.name = "t_key",
             .id = "key_id"},

    [key_index] = {.name = "t_key_index"}

};

/* TODO: Switch to a more performant data structure for lookups (trie?) */
struct plexosTable* get_plexostable(const char* tablename) {

    struct plexosTable* ptr = NULL;

    for (int i = 0; i < n_plexostables; i++) {
        if (strcmp(tablename, tables[i].name) == 0) {
            ptr = &(tables[i]);
            break;
        }
    }

    return ptr;

}
