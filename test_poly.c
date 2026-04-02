#include "test_poly.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static int tests_passed = 0;
static int tests_failed = 0;
static int tests_total = 0;

char* my_strdup(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char* new_str = malloc(len + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

void print_test_result(const char* test_name, int passed, const char* expected_str, const char* actual_str) {
    tests_total++;
    if (passed) {
        tests_passed++;
        printf("%s: ВЫПОЛНЕНО\n", test_name);
    } else {
        tests_failed++;
        printf("%s: ОШИБКА\n", test_name);
        printf("Ожидалось: %s\n", expected_str);
        printf("Получено:  %s\n", actual_str);
    }
}

int test_creation_ascii(const char* test_name, const char* input, int should_succeed) {
    PolyString* str = create_poly_string_ascii(input);
    
    int passed = 0;
    const char* expected_str = should_succeed ? "not NULL" : "NULL";
    char actual_str[64];
    
    if (should_succeed) {
        if (str != NULL && input != NULL) {
            passed = (strcmp((char*)str->data, input) == 0);
            if (passed) {
                snprintf(actual_str, sizeof(actual_str), "\"%s\"", (char*)str->data);
            } else {
                snprintf(actual_str, sizeof(actual_str), "\"%s\" (не совпадает)", (char*)str->data);
            }
        } else if (str != NULL && input == NULL) {
            snprintf(actual_str, sizeof(actual_str), "\"%s\"", (char*)str->data);
        } else {
            snprintf(actual_str, sizeof(actual_str), "NULL");
        }
    } else {
        passed = (str == NULL);
        snprintf(actual_str, sizeof(actual_str), "%s", str ? "not NULL" : "NULL");
    }
    
    print_test_result(test_name, passed, expected_str, actual_str);
    
    if (str) free_poly_string(str);
    
    return passed;
}

int test_creation_wide(const char* test_name, const char* input, int should_succeed) {
    PolyString* str = create_poly_string_wide_from_utf8(input);
    
    int passed = 0;
    const char* expected_str = should_succeed ? "not NULL" : "NULL";
    char actual_str[64];
    
    if (should_succeed) {
        passed = (str != NULL);
        snprintf(actual_str, sizeof(actual_str), "%s", str ? "not NULL" : "NULL");
    } else {
        passed = (str == NULL);
        snprintf(actual_str, sizeof(actual_str), "%s", str ? "not NULL" : "NULL");
    }
    
    print_test_result(test_name, passed, expected_str, actual_str);
    
    if (str) free_poly_string(str);
    
    return passed;
}

int test_concat(const char* test_name, const char* str1, const char* str2, const char* expected) {
    PolyString* a = create_poly_string_ascii(str1);
    PolyString* b = create_poly_string_ascii(str2);
    
    if (!a || !b) {
        if (a) free_poly_string(a);
        if (b) free_poly_string(b);
        print_test_result(test_name, 0, expected ? expected : "NULL", "ошибка создания");
        return 0;
    }
    
    PolyString* result_str = concat_poly_strings(a, b);
    
    int passed = 0;
    char* actual_str = NULL;
    
    if (result_str && result_str->data) {
        actual_str = poly_string_to_file_format(result_str);
        passed = (actual_str && strcmp(actual_str, expected) == 0);
    }
    
    const char* expected_display = expected ? expected : "NULL";
    const char* actual_display = actual_str ? actual_str : "NULL";
    
    print_test_result(test_name, passed, expected_display, actual_display);
    
    free(actual_str);
    free_poly_string(a);
    free_poly_string(b);
    if (result_str) free_poly_string(result_str);
    
    return passed;
}

int test_wide_concat(const char* test_name, const char* str1, const char* str2, const char* expected) {
    PolyString* a = create_poly_string_wide_from_utf8(str1);
    PolyString* b = create_poly_string_wide_from_utf8(str2);
    
    if (!a || !b) {
        if (a) free_poly_string(a);
        if (b) free_poly_string(b);
        print_test_result(test_name, 0, expected ? expected : "NULL", "ошибка создания");
        return 0;
    }
    
    PolyString* result_str = concat_poly_strings(a, b);
    
    int passed = 0;
    char* actual_str = NULL;
    
    if (result_str && result_str->data) {
        actual_str = poly_string_to_file_format(result_str);
        passed = (actual_str && strcmp(actual_str, expected) == 0);
    }
    
    const char* expected_display = expected ? expected : "NULL";
    const char* actual_display = actual_str ? actual_str : "NULL";
    
    print_test_result(test_name, passed, expected_display, actual_display);
    
    free(actual_str);
    free_poly_string(a);
    free_poly_string(b);
    if (result_str) free_poly_string(result_str);
    
    return passed;
}

int test_substring(const char* test_name, const char* str, int start, int end, const char* expected) {
    PolyString* source = create_poly_string_ascii(str);
    
    if (!source) {
        print_test_result(test_name, 0, expected ? expected : "NULL", "ошибка создания");
        return 0;
    }
    
    PolyString* substr = substring_poly_string(source, start, end);
    
    int passed = 0;
    char* actual_str = NULL;
    
    if (expected == NULL) {
        passed = (substr == NULL);
        if (substr) {
            actual_str = my_strdup("not NULL");
        } else {
            actual_str = my_strdup("NULL");
        }
    } else if (substr && substr->data) {
        actual_str = poly_string_to_file_format(substr);
        passed = (actual_str && strcmp(actual_str, expected) == 0);
    } else if (substr == NULL) {
        actual_str = my_strdup("NULL");
    } else {
        actual_str = my_strdup("(no data)");
    }
    
    const char* expected_display = expected ? expected : "NULL";
    const char* actual_display = actual_str ? actual_str : "NULL";
    
    print_test_result(test_name, passed, expected_display, actual_display);
    
    free(actual_str);
    free_poly_string(source);
    if (substr) free_poly_string(substr);
    
    return passed;
}

int test_find(const char* test_name, const char* text, const char* pattern, int sensitive, int expected_pos) {
    PolyString* text_str = create_poly_string_ascii(text);
    PolyString* pattern_str = create_poly_string_ascii(pattern);
    
    if (!text_str || !pattern_str) {
        if (text_str) free_poly_string(text_str);
        if (pattern_str) free_poly_string(pattern_str);
        
        char expected_str[64];
        char actual_str[64];
        snprintf(expected_str, sizeof(expected_str), "%d", expected_pos);
        snprintf(actual_str, sizeof(actual_str), "%s", "ошибка создания");
        print_test_result(test_name, 0, expected_str, actual_str);
        return 0;
    }
    
    int actual_pos = find_in_poly_string(text_str, pattern_str, sensitive);
    
    char expected_str[64];
    char actual_str[64];
    snprintf(expected_str, sizeof(expected_str), "%d", expected_pos);
    snprintf(actual_str, sizeof(actual_str), "%d", actual_pos);
    
    int passed = (actual_pos == expected_pos);
    print_test_result(test_name, passed, expected_str, actual_str);
    
    free_poly_string(text_str);
    free_poly_string(pattern_str);
    
    return passed;
}

int test_length(const char* test_name, const char* str, size_t expected_len) {
    PolyString* poly_str = create_poly_string_ascii(str);
    
    if (!poly_str) {
        char expected_str[64];
        char actual_str[64];
        snprintf(expected_str, sizeof(expected_str), "%zu", expected_len);
        snprintf(actual_str, sizeof(actual_str), "%s", "ошибка создания");
        print_test_result(test_name, 0, expected_str, actual_str);
        return 0;
    }
    
    size_t actual_len = poly_string_length(poly_str);
    
    char expected_str[64];
    char actual_str[64];
    snprintf(expected_str, sizeof(expected_str), "%zu", expected_len);
    snprintf(actual_str, sizeof(actual_str), "%zu", actual_len);
    
    int passed = (actual_len == expected_len);
    print_test_result(test_name, passed, expected_str, actual_str);
    
    free_poly_string(poly_str);
    
    return passed;
}

int test_wide_length(const char* test_name, const char* str, size_t expected_len) {
    PolyString* poly_str = create_poly_string_wide_from_utf8(str);
    
    if (!poly_str) {
        char expected_str[64];
        char actual_str[64];
        snprintf(expected_str, sizeof(expected_str), "%zu", expected_len);
        snprintf(actual_str, sizeof(actual_str), "%s", "ошибка создания");
        print_test_result(test_name, 0, expected_str, actual_str);
        return 0;
    }
    
    size_t actual_len = poly_string_length(poly_str);
    
    char expected_str[64];
    char actual_str[64];
    snprintf(expected_str, sizeof(expected_str), "%zu", expected_len);
    snprintf(actual_str, sizeof(actual_str), "%zu", actual_len);
    
    int passed = (actual_len == expected_len);
    print_test_result(test_name, passed, expected_str, actual_str);
    
    free_poly_string(poly_str);
    
    return passed;
}

void test_string_creation() {
    printf("\nТЕСТ: Создание ASCII строк\n");
    
    test_creation_ascii("create_ascii_normal", "Hello", 1);
    test_creation_ascii("create_ascii_empty", "", 1);
    test_creation_ascii("create_ascii_special", "123!@#", 1);
    test_creation_ascii("create_ascii_null", NULL, 0);
}

void test_wide_creation() {
    printf("\nТЕСТ: Создание Wide строк\n");
    
    test_creation_wide("create_wide_russian", "Привет", 1);
    test_creation_wide("create_wide_mixed", "Hello Привет", 1);
    test_creation_wide("create_wide_empty", "", 1);
    test_creation_wide("create_wide_unicode", "Euro: €, Copyright: ©", 1);
    test_creation_wide("create_wide_null", NULL, 0);
}

void test_concatenation() {
    printf("\nТЕСТ: Конкатенация ASCII строк\n");
    
    test_concat("concat_normal", "Hello ", "World", "Hello World");
    test_concat("concat_empty_first", "", "World", "World");
    test_concat("concat_empty_second", "Hello ", "", "Hello ");
    test_concat("concat_both_empty", "", "", "");
}

void test_wide_concatenation() {
    printf("\nТЕСТ: Конкатенация Wide строк\n");
    
    test_wide_concat("concat_wide_russian", "Привет, ", "мир!", "Привет, мир!");
    test_wide_concat("concat_wide_mixed", "Hello ", "Привет", "Hello Привет");
    test_wide_concat("concat_wide_unicode", "Цена: ", "100 €", "Цена: 100 €");
    test_wide_concat("concat_wide_empty", "Привет", "", "Привет");
}

void test_substring_operations() {
    printf("\nТЕСТ: Подстроки\n");
    
    test_substring("substring_normal", "Hello World", 0, 4, "Hello");
    test_substring("substring_middle", "Hello World", 6, 10, "World");
    test_substring("substring_single", "Hello World", 4, 4, "o");
    test_substring("substring_start_negative", "Hello World", -5, 4, "Hello");
    test_substring("substring_end_too_large", "Hello World", 0, 100, "Hello World");
    test_substring("substring_invalid_range", "Hello World", 10, 5, NULL);
}

void test_find_operations() {
    printf("\nТЕСТ: Поиск подстрок\n");
    
    test_find("find_sensitive_found", "Hello World", "World", 1, 6);
    test_find("find_sensitive_not_found", "Hello World", "world", 1, -1);
    test_find("find_insensitive", "Hello World", "world", 0, 6);
    test_find("find_empty_pattern", "Hello World", "", 1, 0);
    test_find("find_pattern_too_long", "Hello", "Hello World", 1, -1);
}

void test_length_operations() {
    printf("\nТЕСТ: Длина строк\n");
    
    test_length("length_ascii_normal", "Hello", 5);
    test_length("length_ascii_empty", "", 0);
    test_length("length_ascii_spaces", "Hello World", 11);
    
    test_wide_length("length_wide_russian", "Привет", 6);
    test_wide_length("length_wide_mixed", "Hello Привет", 11);
    test_wide_length("length_wide_empty", "", 0);
}

void test_edge_cases() {
    printf("\nТЕСТ: Крайние случаи\n");
    
    char boundary[128];
    for (int i = 0; i < 127; i++) {
        boundary[i] = (char)(i + 1);
    }
    boundary[127] = '\0';
    
    PolyString* boundary_str = create_poly_string_ascii(boundary);
    int passed = (boundary_str != NULL);
    print_test_result("edge_all_ascii", passed, "not NULL", boundary_str ? "not NULL" : "NULL");
    if (boundary_str) free_poly_string(boundary_str);
}

void run_all_tests() {
    setlocale(LC_ALL, "");
    
    tests_passed = 0;
    tests_failed = 0;
    tests_total = 0;
    
    printf("ПОЛНОЕ ТЕСТИРОВАНИЕ ВСЕХ ФУНКЦИЙ\n");
    
    test_string_creation();
    test_wide_creation();
    test_concatenation();
    test_wide_concatenation();
    test_substring_operations();
    test_find_operations();
    test_length_operations();
    test_edge_cases();
    
    printf("ИТОГИ ТЕСТИРОВАНИЯ\n");
    printf("Всего тестов: %d\n", tests_total);
    printf("Пройдено: %d\n", tests_passed);
    printf("Провалено: %d\n", tests_failed);
    printf("Успешность: %.1f%%\n", 
           tests_total > 0 ? (tests_passed * 100.0 / tests_total) : 0);
}