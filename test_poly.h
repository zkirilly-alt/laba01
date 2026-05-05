#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "string_poly.h"

typedef PolyString* (*ConcatFunc)(const PolyString*, const PolyString*);
typedef PolyString* (*SubstringFunc)(const PolyString*, int, int);
typedef int (*FindFunc)(const PolyString*, const PolyString*, int);
typedef size_t (*LengthFunc)(const PolyString*);
typedef char* (*ToUtf8Func)(const PolyString*);

void run_test(const char* test_name, int passed, const char* input_desc, const char* expected, const char* actual);
void test_concat(ConcatFunc func, PolyString* a, PolyString* b, const char* expected_result, const char* test_description);
void test_substring(SubstringFunc func, PolyString* str, int start, int end, const char* expected_result, const char* test_description);
void test_find(FindFunc func, PolyString* text, PolyString* pattern, int sensitive, int expected_position, const char* test_description);
void test_length(LengthFunc func, PolyString* str, size_t expected_length, const char* test_description);
void test_to_utf8(ToUtf8Func func, PolyString* str, const char* expected_output, const char* test_description);
PolyString* create_test_ascii(const char* str);
PolyString* create_test_wide(const char* str);
void run_all_tests(void);