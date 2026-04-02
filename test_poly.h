#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdarg.h>
#include "string_poly.h"

void run_all_tests(void);

int test_creation_ascii(const char* test_name, const char* input, int should_succeed);
int test_creation_wide(const char* test_name, const char* input, int should_succeed);
int test_concat(const char* test_name, const char* str1, const char* str2, const char* expected);
int test_wide_concat(const char* test_name, const char* str1, const char* str2, const char* expected);
int test_substring(const char* test_name, const char* str, int start, int end, const char* expected);
int test_find(const char* test_name, const char* text, const char* pattern, int sensitive, int expected_pos);
int test_length(const char* test_name, const char* str, size_t expected_len);
int test_wide_length(const char* test_name, const char* str, size_t expected_len);