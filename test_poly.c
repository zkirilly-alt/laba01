#include "test_poly.h"
#include <string.h>
#include <stdlib.h>

static int tests_passed = 0;
static int tests_failed = 0;
static int tests_total = 0;

void run_test(const char* test_name, int passed, const char* input_desc, const char* expected, const char* actual) {
    tests_total++;
    if (passed) {
        tests_passed++;
        printf("Пройден: %s\n", test_name);
    } else {
        tests_failed++;
        printf("ОШИБКА: %s\n", test_name);
        printf("   Вход: %s\n", input_desc);
        printf("   Ожидалось: %s\n", expected);
        printf("   Получено: %s\n", actual);
    }
}

PolyString* create_test_ascii(const char* str) {
    return create_poly_string_ascii(str);
}

PolyString* create_test_wide(const char* str) {
    return create_poly_string_wide_from_utf8(str);
}

void test_concat(ConcatFunc func, PolyString* a, PolyString* b, const char* expected_result, const char* test_description) {
    char input_desc[512];
    char* a_str = poly_string_to_utf8(a);
    char* b_str = poly_string_to_utf8(b);
    snprintf(input_desc, sizeof(input_desc), "concat(\"%s\", \"%s\")", a_str ? a_str : "NULL", b_str ? b_str : "NULL");
    PolyString* result = func(a, b);
    char* actual_str = result ? poly_string_to_utf8(result) : NULL;
    int is_expected_null = (strcmp(expected_result, "NULL") == 0);
    int passed = 0;
    if (is_expected_null) {
        passed = (result == NULL && actual_str == NULL);
    } else {
        passed = (result != NULL && actual_str != NULL && strcmp(actual_str, expected_result) == 0);
    }
    const char* actual_display = actual_str ? actual_str : "NULL";
    run_test(test_description, passed, input_desc, expected_result, actual_display);
    free(a_str);
    free(b_str);
    free(actual_str);
    if (result) free_poly_string(result);
}

void test_substring(SubstringFunc func, PolyString* str, int start, int end, const char* expected_result, const char* test_description) {
    char input_desc[512];
    char* str_str = poly_string_to_utf8(str);
    snprintf(input_desc, sizeof(input_desc), "substring(\"%s\", %d, %d)", str_str ? str_str : "NULL", start, end);
    PolyString* result = func(str, start, end);
    char* actual_str = result ? poly_string_to_utf8(result) : NULL;
    int is_expected_null = (strcmp(expected_result, "NULL") == 0);
    int passed = 0;
    if (is_expected_null) {
        passed = (result == NULL && actual_str == NULL);
    } else {
        passed = (result != NULL && actual_str != NULL && strcmp(actual_str, expected_result) == 0);
    }
    const char* actual_display = actual_str ? actual_str : "NULL";
    run_test(test_description, passed, input_desc, expected_result, actual_display);
    free(str_str);
    free(actual_str);
    if (result) free_poly_string(result);
}

void test_find(FindFunc func, PolyString* text, PolyString* pattern, int sensitive, int expected_position, const char* test_description) {
    char input_desc[512];
    char* text_str = poly_string_to_utf8(text);
    char* pattern_str = poly_string_to_utf8(pattern);
    snprintf(input_desc, sizeof(input_desc), "find(\"%s\", \"%s\", %s)", text_str ? text_str : "NULL", pattern_str ? pattern_str : "NULL", sensitive ? "sensitive" : "insensitive");
    int actual_position = func(text, pattern, sensitive);
    char expected_str[32];
    char actual_str[32];
    snprintf(expected_str, sizeof(expected_str), "%d", expected_position);
    snprintf(actual_str, sizeof(actual_str), "%d", actual_position);
    int passed = (actual_position == expected_position);
    run_test(test_description, passed, input_desc, expected_str, actual_str);
    free(text_str);
    free(pattern_str);
}

void test_length(LengthFunc func, PolyString* str, size_t expected_length, const char* test_description) {
    char input_desc[512];
    char* str_str = poly_string_to_utf8(str);
    snprintf(input_desc, sizeof(input_desc), "length(\"%s\")", str_str ? str_str : "NULL");
    size_t actual_length = func(str);
    char expected_str[32];
    char actual_str[32];
    snprintf(expected_str, sizeof(expected_str), "%zu", expected_length);
    snprintf(actual_str, sizeof(actual_str), "%zu", actual_length);
    int passed = (actual_length == expected_length);
    run_test(test_description, passed, input_desc, expected_str, actual_str);
    free(str_str);
}

void test_to_utf8(ToUtf8Func func, PolyString* str, const char* expected_output, const char* test_description) {
    char input_desc[512];
    char* str_str = poly_string_to_utf8(str);
    snprintf(input_desc, sizeof(input_desc), "to_utf8(\"%s\")", str_str ? str_str : "NULL");
    char* actual_output = func(str);
    int is_expected_null = (strcmp(expected_output, "NULL") == 0);
    int passed = 0;
    if (is_expected_null) {
        passed = (actual_output == NULL);
    } else {
        passed = (actual_output != NULL && strcmp(actual_output, expected_output) == 0);
    }
    const char* actual_display = actual_output ? actual_output : "NULL";
    run_test(test_description, passed, input_desc, expected_output, actual_display);
    free(str_str);
    free(actual_output);
}

void test_all_concatenations() {
    printf("\nТЕСТЫ КОНКАТЕНАЦИИ\n");
    PolyString* a1 = create_test_ascii("Hello ");
    PolyString* b1 = create_test_ascii("World");
    test_concat(concat_poly_strings, a1, b1, "Hello World", "Конкатенация ASCII строк");
    free_poly_string(a1);
    free_poly_string(b1);
    PolyString* a2 = create_test_wide("Привет, ");
    PolyString* b2 = create_test_wide("мир!");
    test_concat(concat_poly_strings, a2, b2, "Привет, мир!", "Конкатенация русских строк");
    free_poly_string(a2);
    free_poly_string(b2);
    PolyString* a3 = create_test_ascii("");
    PolyString* b3 = create_test_ascii("Только я");
    test_concat(concat_poly_strings, a3, b3, "Только я", "Конкатенация с пустой строкой (пустая первая)");
    free_poly_string(a3);
    free_poly_string(b3);
    PolyString* a4 = create_test_ascii("Только я");
    PolyString* b4 = create_test_ascii("");
    test_concat(concat_poly_strings, a4, b4, "Только я", "Конкатенация с пустой строкой (пустая вторая)");
    free_poly_string(a4);
    free_poly_string(b4);
    PolyString* a5 = create_test_ascii("Hello");
    PolyString* b5 = create_test_wide("Привет");
    test_concat(concat_poly_strings, a5, b5, "NULL", "Конкатенация разных типов");
    free_poly_string(a5);
    free_poly_string(b5);
}

void test_all_substrings() {
    printf("\nТЕСТЫ ПОДСТРОК\n");
    PolyString* s1 = create_test_ascii("Hello World");
    test_substring(substring_poly_string, s1, 0, 4, "Hello", "Подстрока с начала строки");
    free_poly_string(s1);
    PolyString* s2 = create_test_ascii("Hello World");
    test_substring(substring_poly_string, s2, 6, 10, "World", "Подстрока с конца строки");
    free_poly_string(s2);
    PolyString* s3 = create_test_wide("Привет мир");
    test_substring(substring_poly_string, s3, 0, 5, "Привет", "Подстрока русской строки");
    free_poly_string(s3);
    PolyString* s4 = create_test_ascii("Hello");
    test_substring(substring_poly_string, s4, 2, 2, "l", "Подстрока из одного символа");
    free_poly_string(s4);
    PolyString* s5 = create_test_ascii("Hello");
    test_substring(substring_poly_string, s5, -3, 4, "Hello", "Подстрока с отрицательным стартом");
    free_poly_string(s5);
    PolyString* s6 = create_test_ascii("Hello");
    test_substring(substring_poly_string, s6, 5, 2, "NULL", "Некорректный диапазон (start > end)");
    free_poly_string(s6);
}

void test_all_finds() {
    printf("\nТЕСТЫ ПОИСКА\n");
    PolyString* t1 = create_test_ascii("Hello World");
    PolyString* p1 = create_test_ascii("World");
    test_find(find_in_poly_string, t1, p1, 1, 6, "Чувствительный поиск (должен найти)");
    free_poly_string(t1);
    free_poly_string(p1);
    PolyString* t2 = create_test_ascii("Hello World");
    PolyString* p2 = create_test_ascii("world");
    test_find(find_in_poly_string, t2, p2, 1, -1, "Чувствительный поиск (не должен найти из-за регистра)");
    free_poly_string(t2);
    free_poly_string(p2);
    PolyString* t3 = create_test_ascii("Hello World");
    PolyString* p3 = create_test_ascii("world");
    test_find(find_in_poly_string, t3, p3, 0, 6, "Нечувствительный поиск (должен найти)");
    free_poly_string(t3);
    free_poly_string(p3);
    PolyString* t4 = create_test_wide("Привет мир");
    PolyString* p4 = create_test_wide("мир");
    test_find(find_in_poly_string, t4, p4, 1, 7, "Поиск в русской строке");
    free_poly_string(t4);
    free_poly_string(p4);
    PolyString* t5 = create_test_ascii("Hello");
    PolyString* p5 = create_test_ascii("");
    test_find(find_in_poly_string, t5, p5, 1, 0, "Поиск пустой строки");
    free_poly_string(t5);
    free_poly_string(p5);
}

void test_all_lengths() {
    printf("\nТЕСТЫ ДЛИНЫ\n");
    PolyString* s1 = create_test_ascii("Hello");
    test_length(poly_string_length, s1, 5, "Длина ASCII строки");
    free_poly_string(s1);
    PolyString* s2 = create_test_wide("Привет");
    test_length(poly_string_length, s2, 6, "Длина русской строки");
    free_poly_string(s2);
    PolyString* s3 = create_test_ascii("");
    test_length(poly_string_length, s3, 0, "Длина пустой строки");
    free_poly_string(s3);
    PolyString* s4 = create_test_ascii("Hello World");
    test_length(poly_string_length, s4, 11, "Длина строки с пробелом");
    free_poly_string(s4);
}

void test_all_utf8_conversions() {
    printf("\nТЕСТЫ КОНВЕРТАЦИИ В UTF-8\n");
    PolyString* s1 = create_test_ascii("Hello");
    test_to_utf8(poly_string_to_utf8, s1, "Hello", "Конвертация ASCII строки");
    free_poly_string(s1);
    PolyString* s2 = create_test_wide("Привет");
    test_to_utf8(poly_string_to_utf8, s2, "Привет", "Конвертация русской строки");
    free_poly_string(s2);
    PolyString* s3 = create_test_ascii("");
    test_to_utf8(poly_string_to_utf8, s3, "", "Конвертация пустой строки");
    free_poly_string(s3);
}

void test_corner_cases() {
    printf("\nГРАНИЧНЫЕ СЛУЧАИ\n");
    char long_str[1000];
    memset(long_str, 'A', 999);
    long_str[999] = '\0';
    PolyString* s1 = create_test_ascii(long_str);
    test_length(poly_string_length, s1, 999, "Длина очень длинной строки");
    free_poly_string(s1);
    PolyString* s2 = create_test_ascii("!@#$%^&*()_+");
    test_length(poly_string_length, s2, 12, "Длина строки со спецсимволами");
    free_poly_string(s2);
    PolyString* s3 = create_test_ascii("1234567890");
    test_length(poly_string_length, s3, 10, "Длина строки с числами");
    free_poly_string(s3);
}

void run_all_tests() {
    tests_passed = 0;
    tests_failed = 0;
    tests_total = 0;
    printf("\nПОЛНОЕ ТЕСТИРОВАНИЕ ВСЕХ ФУНКЦИЙ\n");
    test_all_concatenations();
    test_all_substrings();
    test_all_finds();
    test_all_lengths();
    test_all_utf8_conversions();
    test_corner_cases();
    printf("\nИТОГИ ТЕСТИРОВАНИЯ\n");
    printf("Всего тестов: %d\n", tests_total);
    printf("Пройдено: %d\n", tests_passed);
    printf("Провалено: %d\n", tests_failed);
    printf("Успешность: %.1f%%\n", tests_total > 0 ? (tests_passed * 100.0 / tests_total) : 0);
}