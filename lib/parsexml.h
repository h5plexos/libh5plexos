#ifndef xmlparse_INCLUDED
#define xmlparse_INCLUDED

struct parseSpec {
    void* start;
    void* text;
    void* end;
};

void parse(zip_t* archive, int* err, zip_int64_t xml_idx, struct parseSpec spec);

extern struct parseSpec summary_pass;
extern struct parseSpec populate_pass;

#endif // xmlparse_INCLUDED

