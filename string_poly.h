#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>


typedef struct StringTypeInfo StringTypeInfo;

typedef struct {
    void* data;
    StringTypeInfo* type;
} PolyString;

struct StringTypeInfo {
    void (*print)(const void*);
    PolyString* (*concat)(const PolyString*, const PolyString*);
    PolyString* (*substring)(const PolyString*, int, int);
    int (*find)(const PolyString*, const PolyString*, int);
    size_t (*length)(const void*);
    void (*free)(void*);
    char* (*to_file_string)(const void*);
};

extern const size_t POLY_STRING_BUFFER_SIZE;
extern const size_t POLY_STRING_ARRAY_INITIAL_CAPACITY;

PolyString* create_poly_string_ascii(const char* str);
PolyString* create_poly_string_wide(const wchar_t* str);
PolyString* create_poly_string_wide_from_utf8(const char* utf8_str);
void free_poly_string(PolyString* str);
void print_poly_string(const PolyString* str);
PolyString* concat_poly_strings(const PolyString* a, const PolyString* b);
PolyString* substring_poly_string(const PolyString* str, int start, int end);
int find_in_poly_string(const PolyString* text, const PolyString* pattern, int sensitive);
size_t poly_string_length(const PolyString* str);
char* poly_string_to_file_format(const PolyString* str);
void clear_input_buffer();
PolyString* get_poly_string_from_source(const char* prompt);
void run_string_operations_menu();

void demo_concatenation();
void demo_substring();
void demo_search(int sensitive);