#ifndef xmlparse_INCLUDED
#define xmlparse_INCLUDED

struct parseSpec {
    void* start;
    void* text;
    void* end;
};

void parse(zip_t* archive, int* err, zip_int64_t xml_idx, struct parseSpec spec);

struct parseSpec summary_pass;
struct parseSpec data_pass;

#endif // xmlparse_INCLUDED

