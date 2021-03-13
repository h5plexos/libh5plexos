#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "plexostables.h"

bool strequals(const char* x, const char* y) {
    return strcmp(x, y) == 0;
}

bool strbool(const char* x) {
    return strequals(x, "true");
}

char* stralloc(const char* x) {
    char* p = calloc(strlen(x), sizeof(char) + 1);
    strcpy(p, x);
    return p;
}

void populate_config(void* p, const char* field, const char* value) {

    struct plexosConfig* row = p;

    if (strequals(field, "element")) {
        row->element = stralloc(value);
    } else if (strequals(field, "value")) {
        row->value = stralloc(value);
    } else {
        fprintf(stderr, "Unexpected field %s in config table\n", field);
        exit(EXIT_FAILURE);
    }

}

void populate_unit(void* p, const char* field, const char* value) {

    struct plexosUnit* row = p;

    if (strequals(field, "value")) {
        row->value = stralloc(value);
    } else if (strequals(field, "lang_id")) {
        row->lang = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in unit table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_timeslice(void* p, const char* field, const char* value) {

    struct plexosTimeslice* row = p;

    if (strequals(field, "name")) {
        row->name = stralloc(value);
    } else {
        fprintf(stderr, "Unexpected field %s in timeslice table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_model(void* p, const char* field, const char* value) {

    struct plexosModel* row = p;

    if (strequals(field, "name")) {
        row->name = stralloc(value);
    } else {
        fprintf(stderr, "Unexpected field %s in model table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_band(void* p, const char* field, const char* value) {

    struct plexosBand* row = p;

    fprintf(stderr, "Unexpected field %s in band table\n", field);
    exit(EXIT_FAILURE);
}

void populate_sample(void* p, const char* field, const char* value) {

    struct plexosSample* row = p;

    if (strequals(field, "sample_name")) {
        row->name = stralloc(value);
    } else {
        fprintf(stderr, "Unexpected field %s in sample table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_sample_weight(void* p, const char* field, const char* value) {

    struct plexosSampleWeight* row = p;

    if (strequals(field, "phase_id")) {
        row->phase = atoi(value);
    } else if (strequals(field, "value")) {
        row->value = atof(value);
    } else if (strequals(field, "sample_id")) {
        row->sample.idx = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in sample_weight table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_class_group(void* p, const char* field, const char* value) {

    struct plexosClassGroup* row = p;

    if (strequals(field, "name")) {
        row->name = stralloc(value);
    } else if (strequals(field, "lang_id")) {
        row->lang = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in class_group table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_class(void* p, const char* field, const char* value) {

    struct plexosClass* row = p;

    if (strequals(field, "name")) {
        row->name = stralloc(value);
    } else if (strequals(field, "state")) {
        row->state = atoi(value);
    } else if (strequals(field, "lang_id")) {
        row->lang = atoi(value);
    } else if (strequals(field, "class_group_id")) {
        row->classgroup.idx = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in class table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_category(void* p, const char* field, const char* value) {

    struct plexosCategory* row = p;

    if (strequals(field, "name")) {
        row->name = stralloc(value);
    } else if (strequals(field, "rank")) {
        row->rank = atoi(value);
    } else if (strequals(field, "class_id")) {
        row->class.idx = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in category table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_attribute(void* p, const char* field, const char* value) {

    struct plexosAttribute* row = p;

    if (strequals(field, "name")) {
        row->name = stralloc(value);
    } else if (strequals(field, "description")) {
        row->description = stralloc(value);
    } else if (strequals(field, "enum_id")) {
        row->enum_ = atoi(value);
    } else if (strequals(field, "class_id")) {
        row->class.idx = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in attribute table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_collection(void* p, const char* field, const char* value) {

    struct plexosCollection* row = p;

    if (strequals(field, "name")) {
        row->name = stralloc(value);
    } else if (strequals(field, "complement_name")) {
        row->complementname = stralloc(value);
    } else if (strequals(field, "lang_id")) {
        row->lang = atoi(value);
    } else if (strequals(field, "parent_class_id")) {
        row->parentclass.idx = atoi(value);
    } else if (strequals(field, "child_class_id")) {
        row->childclass.idx = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in collection table\n", field);
        exit(EXIT_FAILURE);
    }
}

void populate_property(void* p, const char* field, const char* value) {

    struct plexosProperty* row = p;

    if (strequals(field, "name")) {
        row->name = stralloc(value);
    } else if (strequals(field, "summary_name")) {
        row->summaryname = stralloc(value);
    } else if (strequals(field, "lang_id")) {
        row->lang = atoi(value);
    } else if (strequals(field, "enum_id")) {
        row->enum_ = atoi(value);
    } else if (strequals(field, "is_multi_band")) {
        row->ismultiband = strbool(value);
    } else if (strequals(field, "is_period")) {
        row->isperiod = strbool(value);
    } else if (strequals(field, "is_summary")) {
        row->issummary = strbool(value);
    } else if (strequals(field, "unit_id")) {
        row->unit.idx = atoi(value);
    } else if (strequals(field, "summary_unit_id")) {
        row->summaryunit.idx = atoi(value);
    } else if (strequals(field, "collection_id")) {
        row->collection.idx = atoi(value);
    } else {
        fprintf(stderr, "Unexpected field %s in property table\n", field);
        exit(EXIT_FAILURE);
    }

}

void populate_object(void* p, const char* field, const char* value) {

    struct plexosObject* row = p;

}

void populate_membership(void* p, const char* field, const char* value) {

    struct plexosMembership* row = p;

}

void populate_attribute_data(void* p, const char* field, const char* value) {

    struct plexosAttributeData* row = p;

}

void populate_period_0(void* p, const char* field, const char* value) {

    struct plexosPeriod0* row = p;

}

void populate_period_1(void* p, const char* field, const char* value) {

    struct plexosPeriod1* row = p;

}

void populate_period_2(void* p, const char* field, const char* value) {

    struct plexosPeriod2* row = p;

}

void populate_period_3(void* p, const char* field, const char* value) {

    struct plexosPeriod3* row = p;

}

void populate_period_4(void* p, const char* field, const char* value) {

    struct plexosPeriod4* row = p;

}

void populate_period_6(void* p, const char* field, const char* value) {

    struct plexosPeriod6* row = p;

}

void populate_period_7(void* p, const char* field, const char* value) {

    struct plexosPeriod7* row = p;

}

void populate_phase_2(void* p, const char* field, const char* value) {

    struct plexosPhase2* row = p;

}

void populate_phase_3(void* p, const char* field, const char* value) {

    struct plexosPhase3* row = p;

}

void populate_phase_4(void* p, const char* field, const char* value) {

    struct plexosPhase4* row = p;

}

void populate_key(void* p, const char* field, const char* value) {

    struct plexosKey* row = p;

}

void populate_key_index(void* p, const char* field, const char* value) {

    struct plexosKeyIndex* row = p;

}

struct plexosData data = {0};

struct plexosTable tables[n_plexostables] = {

    [config] = {.idx = config,
                .name = "t_config",
                .populator = populate_config,
                .rows = (void***) &data.configs,
                .rowsize = sizeof(struct plexosConfig)},

    [unit] = {.idx = unit,
              .name = "t_unit",
              .id = "unit_id",
              .zeroindexed = true,
              .populator = populate_unit,
              .rows = (void***) &data.units,
              .rowsize = sizeof(struct plexosUnit)},

    [timeslice] = {.idx = timeslice,
                   .name = "t_timeslice",
                   .id = "timeslice_id",
                   .zeroindexed = true,
                   .populator = populate_timeslice,
                   .rows = (void***) &data.timeslices,
                   .rowsize = sizeof(struct plexosTimeslice)},

    [model] = {.idx = model,
               .name = "t_model",
               .id = "model_id",
               .populator = populate_model,
               .rows = (void***) &data.models,
               .rowsize = sizeof(struct plexosModel)},

    [band] = {.idx = band,
              .name = "t_band",
              .id = "band_id",
              .populator = populate_band,
              .rows = (void***) &data.bands,
              .rowsize = sizeof(struct plexosBand)},

    [sample] = {.idx = sample,
                .name = "t_sample",
                .id = "sample_id",
                .zeroindexed = true,
                .populator = populate_sample,
                .rows = (void***) &data.samples,
                .rowsize = sizeof(struct plexosSample)},

    [sample_weight] = {.idx = sample_weight,
                       .name = "t_sample_weight",
                       .populator = populate_sample_weight,
                       .rows = (void***) &data.sampleweights,
                       .rowsize = sizeof(struct plexosSampleWeight)},

    [class_group] = {.idx = class_group,
                     .name= "t_class_group",
                     .id = "class_group_id",
                     .populator = populate_class_group,
                     .rows = (void***) &data.classgroups,
                     .rowsize = sizeof(struct plexosClassGroup)},

    [class] = {.idx = class,
               .name = "t_class",
               .id = "class_id",
               .populator = populate_class,
               .rows = (void***) &data.classes,
               .rowsize = sizeof(struct plexosClass)},

    [category] = {.idx = category,
                  .name = "t_category",
                  .id = "category_id",
                  .populator = populate_category,
                  .rows = (void***) &data.categories,
                  .rowsize = sizeof(struct plexosCategory)},

    [attribute] = {.idx = attribute,
                   .name = "t_attribute",
                   .id = "attribute_id",
                   .populator = populate_attribute,
                   .rows = (void***) &data.attributes,
                   .rowsize = sizeof(struct plexosAttribute)},

    [collection] = {.idx = collection,
                    .name = "t_collection",
                    .id = "collection_id",
                    .populator = populate_collection,
                    .rows = (void***) &data.collections,
                    .rowsize = sizeof(struct plexosCollection)},

    [property] = {.idx = property,
                  .name = "t_property",
                  .id = "property_id",
                  .populator = populate_property,
                  .rows = (void***) &data.properties,
                  .rowsize = sizeof(struct plexosProperty)},

    [object] = {.idx = object,
                .name = "t_object",
                .id = "object_id",
                .populator = populate_object,
                .rows = (void***) &data.objects,
                .rowsize = sizeof(struct plexosObject)},

    [membership] = {.idx = membership,
                    .name = "t_membership",
                    .id = "membership_id",
                    .populator = populate_membership,
                    .rows = (void***) &data.memberships,
                    .rowsize = sizeof(struct plexosMembership)},

    [attribute_data] = {.idx = attribute_data,
                        .name = "t_attribute_data",
                        .populator = populate_attribute_data,
                        .rows = (void***) &data.attributedata,
                        .rowsize = sizeof(struct plexosAttributeData)},

    [period_0] = {.idx = period_0,
                  .name = "t_period_0",
                  .id = "interval_id",
                  .populator = populate_period_0,
                  .rows = (void***) &data.intervals,
                  .rowsize = sizeof(struct plexosPeriod0)},

    [period_1] = {.idx = period_1,
                  .name = "t_period_1",
                  .id = "day_id",
                  .populator = populate_period_1,
                  .rows = (void***) &data.days,
                  .rowsize = sizeof(struct plexosPeriod1)},

    [period_2] = {.idx = period_2,
                  .name = "t_period_2",
                  .id = "week_id",
                  .populator = populate_period_2,
                  .rows = (void***) &data.weeks,
                  .rowsize = sizeof(struct plexosPeriod2)},

    [period_3] = {.idx = period_3,
                  .name = "t_period_3",
                  .id = "month_id",
                  .populator = populate_period_3,
                  .rows = (void***) &data.months,
                  .rowsize = sizeof(struct plexosPeriod3)},

    [period_4] = {.idx = period_4,
                  .name = "t_period_4",
                  .id = "fiscal_year_id",
                  .populator = populate_period_4,
                  .rows = (void***) &data.years,
                  .rowsize = sizeof(struct plexosPeriod4)},

    [period_6] = {.idx = period_6,
                  .name = "t_period_6",
                  .id = "hour_id",
                  .populator = populate_period_6,
                  .rows = (void***) &data.hours,
                  .rowsize = sizeof(struct plexosPeriod6)},

    [period_7] = {.idx = period_7,
                  .name = "t_period_7",
                  .id = "quarter_id",
                  .populator = populate_period_7,
                  .rows = (void***) &data.quarters,
                  .rowsize = sizeof(struct plexosPeriod7)},

    [phase_2] = {.idx = phase_2,
                 .name = "t_phase_2",
                 .populator = populate_phase_2,
                 .rows = (void***) &data.pasa,
                 .rowsize = sizeof(struct plexosPhase2)},

    [phase_3] = {.idx = phase_3,
                 .name = "t_phase_3",
                 .populator = populate_phase_3,
                 .rows = (void***) &data.mt,
                 .rowsize = sizeof(struct plexosPhase3)},

    [phase_4] = {.idx = phase_4,
                 .name = "t_phase_4",
                 .populator = populate_phase_4,
                 .rows = (void***) &data.st,
                 .rowsize = sizeof(struct plexosPhase4)},

    [key] = {.idx = key,
             .name = "t_key",
             .id = "key_id",
             .populator = populate_key,
             .rows = (void***) &data.keys,
             .rowsize = sizeof(struct plexosKey)},

    [key_index] = {.idx = key_index,
                   .name = "t_key_index",
                   .populator = populate_key_index,
                   .rows = (void***) &data.keyindices,
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

void* row_array(int table_idx) {
    size_t n = tables[table_idx].max_idx;
    void* p = calloc(n, sizeof(void*));
    memset(p, 0, n * sizeof(void*));
    return p;
}

void init_data() {

    data.configs = row_array(config);
    data.units = row_array(unit);
    data.timeslices = row_array(timeslice);
    data.models = row_array(model);
    data.bands = row_array(band);

    data.samples = row_array(sample);
    data.sampleweights = row_array(sample_weight);

    data.classgroups = row_array(class_group);
    data.classes = row_array(class);
    data.categories = row_array(category);

    data.attributes = row_array(attribute);
    data.collections = row_array(collection);
    data.properties = row_array(property);

    data.objects = row_array(object);
    data.memberships = row_array(membership);
    data.attributedata = row_array(attribute_data);

    data.intervals = row_array(period_0);
    data.days = row_array(period_1);
    data.weeks = row_array(period_2);
    data.months = row_array(period_3);
    data.years = row_array(period_4);
    data.hours = row_array(period_6);
    data.quarters = row_array(period_7);

    data.pasa = row_array(phase_2);
    data.mt = row_array(phase_3);
    data.st = row_array(phase_4);

    data.keys = row_array(key);
    data.keyindices = row_array(key_index);

}
