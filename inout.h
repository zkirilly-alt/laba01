#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>
#include "string_poly.h"

typedef struct {
    char* strings;
    size_t count;
    size_t capacity;
} StringArray;

extern const size_t INITIAL_STRING_ARRAY_CAPACITY;
extern const size_t READ_STRING_BUFFER_SIZE;
extern const size_t READ_STRING_INITIAL_SIZE;
extern const size_t FILENAME_BUFFER_SIZE;
extern const size_t LINE_BUFFER_SIZE;
extern const size_t MAX_STRING_LENGTH;

void allinout();
int input_char_string();
int input_char16_string();
int input_from_file();
int read_ascii_string(char **result);
void save_to_char_file(const char *text);
void save_to_char16_file(const char *text);
void clear_input_buffer();
StringArray* read_strings_from_file(const char* filename);
void free_string_array(StringArray* array);
void display_strings(const StringArray* array);
char* select_string(const StringArray* array);
PolyString* get_poly_string_from_source(const char* prompt);
void run_string_operations_menu();