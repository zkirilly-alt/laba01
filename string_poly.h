#pragma once
#include <stddef.h>
#include <wchar.h>

typedef struct StringTypeInfo StringTypeInfo;

typedef struct {
    void* data;
    const StringTypeInfo* type;
} PolyString;

struct StringTypeInfo {
    void (*print_to_buffer)(const void* data, char* buffer, size_t buffer_size);
    PolyString* (*concat)(const PolyString* a, const PolyString* b);
    PolyString* (*substring)(const PolyString* str, int start, int end);
    int (*find)(const PolyString* text, const PolyString* pattern, int sensitive);
    size_t (*length)(const void* data);
    void (*free)(void* data);
    char* (*to_utf8)(const void* data);
};

extern const StringTypeInfo AsciiType;
extern const StringTypeInfo WideType;

PolyString* create_poly_string_ascii(const char* str);
PolyString* create_poly_string_wide(const wchar_t* str);
PolyString* create_poly_string_wide_from_utf8(const char* utf8_str);
void free_poly_string(PolyString* str);
void print_poly_string_to_buffer(const PolyString* str, char* buffer, size_t buffer_size);
PolyString* concat_poly_strings(const PolyString* a, const PolyString* b);
PolyString* substring_poly_string(const PolyString* str, int start, int end);
int find_in_poly_string(const PolyString* text, const PolyString* pattern, int sensitive);
size_t poly_string_length(const PolyString* str);
char* poly_string_to_utf8(const PolyString* str);
int is_ascii_only(const char* str);