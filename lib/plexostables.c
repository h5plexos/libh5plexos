#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "plexostables.h"

void populate_config(struct plexosConfig* row, const char* field, const char* value) {
    // need to cast row from void* to struct plexosConfig* (for all)
    if (field == "element") {
        row->element = value; // need to copy all of these!
    } else if (field == "value") {
        row->value = value;
    } else {
        fprintf(stderr, "Unexpected field %s in config table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_unit(struct plexosUnit* row, const char* field, const char* value) {
    if (field == "value") {
        row->value = value;
    } else {
        fprintf(stderr, "Unexpected field %s in unit table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_timeslice(struct plexosTimeslice* row, const char* field, const char* value) {
    if (field == "name") {
        row->name = value;
    } else {
        fprintf(stderr, "Unexpected field %s in timeslice table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_model(struct plexosModel* row, const char* field, const char* value) {
    if (field == "name") {
        row->name = value;
    } else {
        fprintf(stderr, "Unexpected field %s in model table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_band(struct plexosBand* row, const char* field, const char* value) {
    fprintf(stderr, "Unexpected field %s in band table", field);
    exit(EXIT_FAILURE);
}

void populate_sample(struct plexosSample* row, const char* field, const char* value) {
    if (field == "name") {
        row->name = value;
    } else {
        fprintf(stderr, "Unexpected field %s in sample table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_sample_weight(struct plexosSampleWeight* row, const char* field, const char* value) {
    if (field == "phase_id") {
        row->phase = atoi(value);
    } else if (field == "value") {
        row->value = atof(value);
    } else if (field == "sample_id") {
        row->sample = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in sample_weight table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_class_group(struct plexosClassGroup* row, const char* field, const char* value) {
    if (field == "name") {
        row->name = value;
    } else {
        fprintf(stderr, "Unexpected field %s in class_group table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_class(struct plexosClass* row, const char* field, const char* value) {
    if (field == "name") {
        row->name = value;
    } else if (field == "state") {
        row->state = atoi(value);
    } else if (field == "class_group_id") {
        row->classgroup = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in class table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_category(struct plexosCategory* row, const char* field, const char* value) {
    if (field == "name") {
        row->name = value;
    } else if (field == "rank") {
        row->rank = atoi(value);
    } else if (field == "class_id") {
        row->class = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in category table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_attribute(struct plexosAttribute* row, const char* field, const char* value) {
    if (field == "name") {
        row->name = value;
    } else if (field == "description") {
        row->description = value;
    } else if (field == "enum_id") {
        row->enum_ = atoi(value);
    } else if (field == "class_id") {
        row->class = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in attribute table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_collection(struct plexosCollection* row, const char* field, const char* value) {
    if (field == "name") {
        row->name = value;
    } else if (field == "complement_name") {
        row->complementname = value;
    } else if (field == "parent_class_id") {
        row->parentclass = atoi(value);
    } else if (field == "child_class_id") {
        row->childclass = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in collection table", field);
        exit(EXIT_FAILURE);
    }
}

void populate_property(struct plexosProperty* row, const char* field, const char* value) {}
void populate_object(struct plexosObject* row, const char* field, const char* value) {}
void populate_membership(struct plexosMembership* row, const char* field, const char* value) {}
void populate_attribute_data(struct plexosAttributeData* row, const char* field, const char* value) {}

void populate_period_0(struct plexosPeriod0* row, const char* field, const char* value) {}
void populate_period_1(struct plexosPeriod1* row, const char* field, const char* value) {}
void populate_period_2(struct plexosPeriod2* row, const char* field, const char* value) {}
void populate_period_3(struct plexosPeriod3* row, const char* field, const char* value) {}
void populate_period_4(struct plexosPeriod4* row, const char* field, const char* value) {}
void populate_period_6(struct plexosPeriod6* row, const char* field, const char* value) {}
void populate_period_7(struct plexosPeriod7* row, const char* field, const char* value) {}

void populate_phase_2(struct plexosPhase2* row, const char* field, const char* value) {}
void populate_phase_3(struct plexosPhase3* row, const char* field, const char* value) {}
void populate_phase_4(struct plexosPhase4* row, const char* field, const char* value) {}

void populate_key(struct plexosKey* row, const char* field, const char* value) {}
void populate_key_index(struct plexosKeyIndex* row, const char* field, const char* value) {}

struct plexosData data = {0};

struct plexosTable tables[n_plexostables] = {

    [config] = {.idx = config,
                .name = "t_config",
                .populator = populate_config,
                .rows = &data.configs,
                .rowsize = sizeof(struct plexosConfig)},

    [unit] = {.idx = unit,
              .name = "t_unit",
              .id = "unit_id",
              .zeroindexed = true,
              .populator = populate_unit,
              .rows = &data.units,
              .rowsize = sizeof(struct plexosUnit)},

    [timeslice] = {.idx = timeslice,
                   .name = "t_timeslice",
                   .id = "timeslice_id",
                   .zeroindexed = true,
                   .populator = populate_timeslice,
                   .rows = &data.timeslices,
                   .rowsize = sizeof(struct plexosTimeslice)},

    [model] = {.idx = model,
               .name = "t_model",
               .id = "model_id",
               .populator = populate_model,
               .rows = &data.models,
               .rowsize = sizeof(struct plexosModel)},

    [band] = {.idx = band,
              .name = "t_band",
              .populator = populate_band,
              .rows = &data.bands,
              .rowsize = sizeof(struct plexosBand)},

    [sample] = {.idx = sample,
                .name = "t_sample",
                .id = "sample_id",
                .zeroindexed = true,
                .populator = populate_sample,
                .rows = &data.samples,
                .rowsize = sizeof(struct plexosSample)},

    [sample_weight] = {.idx = sample_weight,
                       .name = "t_sample_weight",
                       .populator = populate_sample_weight,
                       .rows = &data.sampleweights,
                       .rowsize = sizeof(struct plexosSampleWeight)},

    [class_group] = {.idx = class_group,
                     .name= "t_class_group",
                     .id = "class_group_id",
                     .populator = populate_class_group,
                     .rows = &data.classgroups,
                     .rowsize = sizeof(struct plexosClassGroup)},

    [class] = {.idx = class,
               .name = "t_class",
               .id = "class_id",
               .populator = populate_class,
               .rows = &data.classes,
               .rowsize = sizeof(struct plexosClass)},

    [category] = {.idx = category,
                  .name = "t_category",
                  .id = "category_id",
                  .populator = populate_category,
                  .rows = &data.categories,
                  .rowsize = sizeof(struct plexosCategory)},

    [attribute] = {.idx = attribute,
                   .name = "t_attribute",
                   .id = "attribute_id",
                   .populator = populate_attribute,
                   .rows = &data.attributes,
                   .rowsize = sizeof(struct plexosAttribute)},

    [collection] = {.idx = collection,
                    .name = "t_collection",
                    .id = "collection_id",
                    .populator = populate_collection,
                    .rows = &data.collections,
                    .rowsize = sizeof(struct plexosCollection)},

    [property] = {.idx = property,
                  .name = "t_property",
                  .id = "property_id",
                  .populator = populate_property,
                  .rows = &data.properties,
                  .rowsize = sizeof(struct plexosProperty)},

    [object] = {.idx = object,
                .name = "t_object",
                .id = "object_id",
                .populator = populate_object,
                .rows = &data.objects,
                .rowsize = sizeof(struct plexosObject)},

    [membership] = {.idx = membership,
                    .name = "t_membership",
                    .id = "membership_id",
                    .populator = populate_membership,
                    .rows = &data.memberships,
                    .rowsize = sizeof(struct plexosMembership)},

    [attribute_data] = {.idx = attribute_data,
                        .name = "t_attribute_data",
                        .populator = populate_attribute_data,
                        .rows = &data.attributedata,
                        .rowsize = sizeof(struct plexosAttributeData)},

    [period_0] = {.idx = period_0,
                  .name = "t_period_0",
                  .id = "interval_id",
                  .populator = populate_period_0,
                  .rows = &data.intervals,
                  .rowsize = sizeof(struct plexosPeriod0)},

    [period_1] = {.idx = period_1,
                  .name = "t_period_1",
                  .id = "day_id",
                  .populator = populate_period_1,
                  .rows = &data.days,
                  .rowsize = sizeof(struct plexosPeriod1)},

    [period_2] = {.idx = period_2,
                  .name = "t_period_2",
                  .id = "week_id",
                  .populator = populate_period_2,
                  .rows = &data.weeks,
                  .rowsize = sizeof(struct plexosPeriod2)},

    [period_3] = {.idx = period_3,
                  .name = "t_period_3",
                  .id = "month_id",
                  .populator = populate_period_3,
                  .rows = &data.months,
                  .rowsize = sizeof(struct plexosPeriod3)},

    [period_4] = {.idx = period_4,
                  .name = "t_period_4",
                  .id = "fiscal_year_id",
                  .populator = populate_period_4,
                  .rows = &data.years,
                  .rowsize = sizeof(struct plexosPeriod4)},

    [period_6] = {.idx = period_6,
                  .name = "t_period_6",
                  .id = "hour_id",
                  .populator = populate_period_6,
                  .rows = &data.hours,
                  .rowsize = sizeof(struct plexosPeriod6)},

    [period_7] = {.idx = period_7,
                  .name = "t_period_7",
                  .id = "quarter_id",
                  .populator = populate_period_7,
                  .rows = &data.quarters,
                  .rowsize = sizeof(struct plexosPeriod7)},

    [phase_2] = {.idx = phase_2,
                 .name = "t_phase_2",
                 .populator = populate_phase_2,
                 .rows = &data.pasa,
                 .rowsize = sizeof(struct plexosPhase2)},

    [phase_3] = {.idx = phase_3,
                 .name = "t_phase_3",
                 .populator = populate_phase_3,
                 .rows = &data.mt,
                 .rowsize = sizeof(struct plexosPhase3)},

    [phase_4] = {.idx = phase_4,
                 .name = "t_phase_4",
                 .populator = populate_phase_4,
                 .rows = &data.st,
                 .rowsize = sizeof(struct plexosPhase4)},

    [key] = {.idx = key,
             .name = "t_key",
             .id = "key_id",
             .populator = populate_key,
             .rows = &data.keys,
             .rowsize = sizeof(struct plexosKey)},

    [key_index] = {.idx = key_index,
                   .name = "t_key_index",
                   .populator = populate_key_index,
                   .rows = &data.keyindices,
                   .rowsize = sizeof(struct plexosKeyIndex)}

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
