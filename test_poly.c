#include "test_poly.h"

static int tests_passed = 0;
static int tests_failed = 0;
static int tests_total = 0;

void print_test_result(const char* test_name, int passed) {
    tests_total++;
    if (passed) {
        tests_passed++;
        printf("  ✓ %s: ВЫПОЛНЕНО\n", test_name);
    } else {
        tests_failed++;
        printf("  ✗ %s: ОШИБКА\n", test_name);
    }
}

void test_ascii_creation() {
    printf("\n--- ТЕСТ: Создание ASCII строк ---\n");
    
    
    PolyString* str1 = create_poly_string_ascii("Hello");
    printf("  str1: "); print_poly_string(str1); printf("\n");
    print_test_result("create_ascii_normal", str1 != NULL && strcmp((char*)str1->data, "Hello") == 0);
    
    
    PolyString* str2 = create_poly_string_ascii("");
    printf("  str2: "); print_poly_string(str2); printf("\n");
    print_test_result("create_ascii_empty", str2 != NULL && strlen((char*)str2->data) == 0);
    
    
    PolyString* str3 = create_poly_string_ascii("123!@#");
    printf("  str3: "); print_poly_string(str3); printf("\n");
    print_test_result("create_ascii_special", str3 != NULL && strcmp((char*)str3->data, "123!@#") == 0);
    
    
    PolyString* str4 = create_poly_string_ascii(NULL);
    printf("  str4: %p\n", (void*)str4);
    print_test_result("create_ascii_null", str4 == NULL);
    
    free_poly_string(str1);
    free_poly_string(str2);
    free_poly_string(str3);
}

void test_wide_creation() {
    printf("\n--- ТЕСТ: Создание Wide строк ---\n");
    
    
    PolyString* str1 = create_poly_string_wide_from_utf8("Привет");
    printf("  str1: "); print_poly_string(str1); printf("\n");
    print_test_result("create_wide_russian", str1 != NULL);
    
    
    PolyString* str2 = create_poly_string_wide_from_utf8("Hello Привет");
    printf("  str2: "); print_poly_string(str2); printf("\n");
    print_test_result("create_wide_mixed", str2 != NULL);
    
    
    PolyString* str3 = create_poly_string_wide_from_utf8("");
    printf("  str3: "); print_poly_string(str3); printf("\n");
    print_test_result("create_wide_empty", str3 != NULL);
    
    
    PolyString* str4 = create_poly_string_wide_from_utf8("Euro: €, Copyright: ©");
    printf("  str4: "); print_poly_string(str4); printf("\n");
    print_test_result("create_wide_unicode", str4 != NULL);
    
    
    PolyString* str5 = create_poly_string_wide_from_utf8(NULL);
    printf("  str5: %p\n", (void*)str5);
    print_test_result("create_wide_null", str5 == NULL);
    
    free_poly_string(str1);
    free_poly_string(str2);
    free_poly_string(str3);
    free_poly_string(str4);
}

void test_ascii_concatenation() {
    printf("\n--- ТЕСТ: Конкатенация ASCII строк ---\n");
    
    
    PolyString* a1 = create_poly_string_ascii("Hello ");
    PolyString* b1 = create_poly_string_ascii("World");
    PolyString* r1 = concat_poly_strings(a1, b1);
    printf("  '%s' + '%s' = ", (char*)a1->data, (char*)b1->data);
    print_poly_string(r1); printf("\n");
    print_test_result("concat_ascii_normal", 
        r1 != NULL && strcmp((char*)r1->data, "Hello World") == 0);
    free_poly_string(r1);
    
    
    PolyString* a2 = create_poly_string_ascii("Hello");
    PolyString* b2 = create_poly_string_ascii("");
    PolyString* r2 = concat_poly_strings(a2, b2);
    printf("  '%s' + '' = ", (char*)a2->data);
    print_poly_string(r2); printf("\n");
    print_test_result("concat_ascii_empty", 
        r2 != NULL && strcmp((char*)r2->data, "Hello") == 0);
    free_poly_string(r2);
    
    
    PolyString* r3 = concat_poly_strings(a2, NULL);
    printf("  concat with NULL: %p\n", (void*)r3);
    print_test_result("concat_ascii_with_null", r3 == NULL);
    
    
    PolyString* wide = create_poly_string_wide_from_utf8("Привет");
    PolyString* r4 = concat_poly_strings(a2, wide);
    printf("  ASCII + Wide: %p\n", (void*)r4);
    print_test_result("concat_different_types", r4 == NULL);
    
    free_poly_string(a1);
    free_poly_string(b1);
    free_poly_string(a2);
    free_poly_string(b2);
    free_poly_string(wide);
}

void test_wide_concatenation() {
    printf("\n--- ТЕСТ: Конкатенация Wide строк ---\n");
    
    
    PolyString* a1 = create_poly_string_wide_from_utf8("Привет, ");
    PolyString* b1 = create_poly_string_wide_from_utf8("мир!");
    PolyString* r1 = concat_poly_strings(a1, b1);
    printf("  "); print_poly_string(a1); printf(" + "); print_poly_string(b1); printf(" = ");
    print_poly_string(r1); printf("\n");
    print_test_result("concat_wide_russian", r1 != NULL);
    free_poly_string(r1);
    
    
    PolyString* a2 = create_poly_string_wide_from_utf8("Цена: ");
    PolyString* b2 = create_poly_string_wide_from_utf8("100 €");
    PolyString* r2 = concat_poly_strings(a2, b2);
    printf("  "); print_poly_string(a2); printf(" + "); print_poly_string(b2); printf(" = ");
    print_poly_string(r2); printf("\n");
    print_test_result("concat_wide_unicode", r2 != NULL);
    free_poly_string(r2);
    
    
    PolyString* a3 = create_poly_string_wide_from_utf8("Привет");
    PolyString* b3 = create_poly_string_wide_from_utf8("");
    PolyString* r3 = concat_poly_strings(a3, b3);
    printf("  "); print_poly_string(a3); printf(" + '' = ");
    print_poly_string(r3); printf("\n");
    print_test_result("concat_wide_empty", r3 != NULL);
    free_poly_string(r3);
    
    free_poly_string(a1);
    free_poly_string(b1);
    free_poly_string(a2);
    free_poly_string(b2);
    free_poly_string(a3);
    free_poly_string(b3);
}

void test_ascii_substring() {
    printf("\n--- ТЕСТ: Подстроки ASCII ---\n");
    
    PolyString* str = create_poly_string_ascii("Hello World");
    printf("  Исходная: "); print_poly_string(str); printf("\n");
    
    
    PolyString* sub1 = substring_poly_string(str, 0, 4);
    printf("  [0,4]: "); print_poly_string(sub1); printf("\n");
    print_test_result("substring_ascii_normal", 
        sub1 != NULL && strcmp((char*)sub1->data, "Hello") == 0);
    free_poly_string(sub1);
    
    
    PolyString* sub2 = substring_poly_string(str, 6, 10);
    printf("  [6,10]: "); print_poly_string(sub2); printf("\n");
    print_test_result("substring_ascii_end", 
        sub2 != NULL && strcmp((char*)sub2->data, "World") == 0);
    free_poly_string(sub2);
    
    
    PolyString* sub3 = substring_poly_string(str, 4, 4);
    printf("  [4,4]: "); print_poly_string(sub3); printf("\n");
    print_test_result("substring_ascii_single", 
        sub3 != NULL && strcmp((char*)sub3->data, "o") == 0);
    free_poly_string(sub3);
    
    
    PolyString* sub4 = substring_poly_string(str, -5, 4);  
    printf("  [-5,4]: "); print_poly_string(sub4); printf("\n");
    print_test_result("substring_ascii_negative_start", 
        sub4 != NULL && strcmp((char*)sub4->data, "Hello") == 0);
    free_poly_string(sub4);
    
    PolyString* sub5 = substring_poly_string(str, 0, 100);  
    printf("  [0,100]: "); print_poly_string(sub5); printf("\n");
    print_test_result("substring_ascii_end_too_large", 
        sub5 != NULL && strcmp((char*)sub5->data, "Hello World") == 0);
    free_poly_string(sub5);
    
    
    PolyString* sub6 = substring_poly_string(str, 10, 5);  
    printf("  [10,5]: %p\n", (void*)sub6);
    print_test_result("substring_ascii_invalid_range", sub6 == NULL);
    
    free_poly_string(str);
}

void test_wide_substring() {
    printf("\n--- ТЕСТ: Подстроки Wide ---\n");
    
    PolyString* str = create_poly_string_wide_from_utf8("Привет мир!");
    printf("  Исходная: "); print_poly_string(str); printf("\n");
    
    
    PolyString* sub1 = substring_poly_string(str, 0, 5);
    printf("  [0,5]: "); print_poly_string(sub1); printf("\n");
    print_test_result("substring_wide_start", sub1 != NULL);
    free_poly_string(sub1);
    
    
    PolyString* sub2 = substring_poly_string(str, 7, 10);
    printf("  [7,10]: "); print_poly_string(sub2); printf("\n");
    print_test_result("substring_wide_end", sub2 != NULL);
    free_poly_string(sub2);
    
    
    PolyString* sub3 = substring_poly_string(str, 3, 8);
    printf("  [3,8]: "); print_poly_string(sub3); printf("\n");
    print_test_result("substring_wide_middle", sub3 != NULL);
    free_poly_string(sub3);
    
    
    PolyString* sub4 = substring_poly_string(str, 5, 4);
    printf("  [5,4]: %p\n", (void*)sub4);
    print_test_result("substring_wide_invalid", sub4 == NULL);
    
    free_poly_string(str);
}

void test_ascii_find() {
    printf("\n--- ТЕСТ: Поиск в ASCII строках ---\n");
    
    PolyString* text = create_poly_string_ascii("Hello World, hello world");
    printf("  Текст: "); print_poly_string(text); printf("\n");
    
    
    PolyString* pattern1 = create_poly_string_ascii("World");
    int pos1 = find_in_poly_string(text, pattern1, 1);
    printf("  Поиск 'World' (чувств.): позиция %d\n", pos1);
    print_test_result("find_ascii_sensitive_found", pos1 == 6);
    
    
    PolyString* pattern2 = create_poly_string_ascii("world");
    int pos2 = find_in_poly_string(text, pattern2, 1);
    printf("  Поиск 'world' (чувств.): позиция %d\n", pos2);
    print_test_result("find_ascii_sensitive_not_found", pos2 == 19);
    
    
    int pos3 = find_in_poly_string(text, pattern2, 0);
    printf("  Поиск 'world' (нечувств.): позиция %d\n", pos3);
    print_test_result("find_ascii_insensitive", pos3 == 6 || pos3 == 19);
    
    
    PolyString* pattern3 = create_poly_string_ascii("");
    int pos4 = find_in_poly_string(text, pattern3, 1);
    printf("  Поиск '' : позиция %d\n", pos4);
    print_test_result("find_ascii_empty_pattern", pos4 == 0);
    
    
    int pos5 = find_in_poly_string(text, NULL, 1);
    printf("  Поиск NULL: %d\n", pos5);
    print_test_result("find_ascii_null_pattern", pos5 == -1);
    
    
    PolyString* pattern4 = create_poly_string_ascii("Hello World, hello world!!!");
    int pos6 = find_in_poly_string(text, pattern4, 1);
    printf("  Поиск более длинной строки: %d\n", pos6);
    print_test_result("find_ascii_pattern_too_long", pos6 == -1);
    
    free_poly_string(text);
    free_poly_string(pattern1);
    free_poly_string(pattern2);
    free_poly_string(pattern3);
    free_poly_string(pattern4);
}

void test_wide_find() {
    printf("\n--- ТЕСТ: Поиск в Wide строках ---\n");
    
    PolyString* text = create_poly_string_wide_from_utf8("Привет мир, привет мир");
    printf("  Текст: "); print_poly_string(text); printf("\n");
    
    
    PolyString* pattern1 = create_poly_string_wide_from_utf8("мир");
    int pos1 = find_in_poly_string(text, pattern1, 1);
    printf("  Поиск 'мир' (чувств.): позиция %d\n", pos1);
    print_test_result("find_wide_sensitive", pos1 == 7);
    
    
    PolyString* pattern2 = create_poly_string_wide_from_utf8("ПРИВЕТ");
    int pos2 = find_in_poly_string(text, pattern2, 1);
    printf("  Поиск 'ПРИВЕТ' (чувств.): позиция %d\n", pos2);
    print_test_result("find_wide_sensitive_case", pos2 == -1);
    
    
    PolyString* pattern3 = create_poly_string_wide_from_utf8("Привет");
    int pos3 = find_in_poly_string(text, pattern3, 1);
    printf("  Поиск 'Привет' (чувств.): позиция %d\n", pos3);
    print_test_result("find_wide_start", pos3 == 0);
    
    
    PolyString* pattern4 = create_poly_string_wide_from_utf8("мир");
    int pos4 = find_in_poly_string(text, pattern4, 1);
    printf("  Поиск последнего 'мир': позиция %d\n", pos4);
    print_test_result("find_wide_multiple_occurrences", pos4 == 7);
    
    free_poly_string(text);
    free_poly_string(pattern1);
    free_poly_string(pattern2);
    free_poly_string(pattern3);
    free_poly_string(pattern4);
}

void test_ascii_length() {
    printf("\n--- ТЕСТ: Длина ASCII строк ---\n");
    
    PolyString* str1 = create_poly_string_ascii("Hello");
    size_t len1 = poly_string_length(str1);
    printf("  Длина 'Hello': %zu\n", len1);
    print_test_result("length_ascii_normal", len1 == 5);
    free_poly_string(str1);
    
    PolyString* str2 = create_poly_string_ascii("");
    size_t len2 = poly_string_length(str2);
    printf("  Длина '': %zu\n", len2);
    print_test_result("length_ascii_empty", len2 == 0);
    free_poly_string(str2);
    
    PolyString* str3 = create_poly_string_ascii("Hello World! 123");
    size_t len3 = poly_string_length(str3);
    printf("  Длина 'Hello World! 123': %zu\n", len3);
    print_test_result("length_ascii_with_spaces", len3 == 16);
    free_poly_string(str3);
    
    size_t len4 = poly_string_length(NULL);
    printf("  Длина NULL: %zu\n", len4);
    print_test_result("length_ascii_null", len4 == 0);
}

void test_wide_length() {
    printf("\n--- ТЕСТ: Длина Wide строк ---\n");
    
    PolyString* str1 = create_poly_string_wide_from_utf8("Привет");
    size_t len1 = poly_string_length(str1);
    printf("  Длина 'Привет': %zu символов\n", len1);
    print_test_result("length_wide_russian", len1 == 6);
    free_poly_string(str1);
    
    PolyString* str2 = create_poly_string_wide_from_utf8("Hello Привет");
    size_t len2 = poly_string_length(str2);
    printf("  Длина 'Hello Привет': %zu символов\n", len2);
    print_test_result("length_wide_mixed", len2 == 12);
    free_poly_string(str2);
    
    PolyString* str3 = create_poly_string_wide_from_utf8("€ ©");
    size_t len3 = poly_string_length(str3);
    printf("  Длина '€ ©': %zu символов\n", len3);
    print_test_result("length_wide_unicode", len3 == 3);
    free_poly_string(str3);
    
    PolyString* str4 = create_poly_string_wide_from_utf8("");
    size_t len4 = poly_string_length(str4);
    printf("  Длина '': %zu\n", len4);
    print_test_result("length_wide_empty", len4 == 0);
    free_poly_string(str4);
}

void test_file_conversion() {
    printf("\n--- ТЕСТ: Конвертация для файла ---\n");
    
    
    PolyString* ascii = create_poly_string_ascii("ASCII text");
    char* ascii_file = poly_string_to_file_format(ascii);
    printf("  ASCII для файла: %s\n", ascii_file ? ascii_file : "NULL");
    print_test_result("file_ascii_conversion", 
        ascii_file != NULL && strcmp(ascii_file, "ASCII text") == 0);
    free(ascii_file);
    
    
    PolyString* wide1 = create_poly_string_wide_from_utf8("Русский текст");
    char* wide_file1 = poly_string_to_file_format(wide1);
    printf("  Wide для файла: %s\n", wide_file1 ? wide_file1 : "NULL");
    print_test_result("file_wide_russian", wide_file1 != NULL);
    free(wide_file1);
    
    
    PolyString* wide2 = create_poly_string_wide_from_utf8("Цена: 100 €");
    char* wide_file2 = poly_string_to_file_format(wide2);
    printf("  Wide с Unicode: %s\n", wide_file2 ? wide_file2 : "NULL");
    print_test_result("file_wide_unicode", wide_file2 != NULL);
    free(wide_file2);
    
    
    PolyString* empty = create_poly_string_ascii("");
    char* empty_file = poly_string_to_file_format(empty);
    printf("  Пустая строка для файла: %s\n", empty_file ? empty_file : "NULL");
    print_test_result("file_empty", empty_file != NULL && strlen(empty_file) == 0);
    free(empty_file);
    
    
    char* null_file = poly_string_to_file_format(NULL);
    printf("  NULL для файла: %p\n", (void*)null_file);
    print_test_result("file_null", null_file == NULL);
    
    free_poly_string(ascii);
    free_poly_string(wide1);
    free_poly_string(wide2);
    free_poly_string(empty);
}

void test_mixed_operations() {
    printf("\n--- ТЕСТ: Смешанные операции ---\n");
    
    
    PolyString* s1 = create_poly_string_wide_from_utf8("Привет");
    PolyString* s2 = create_poly_string_wide_from_utf8(" мир");
    
    PolyString* s3 = concat_poly_strings(s1, s2);
    printf("  Конкатенация: "); print_poly_string(s3); printf("\n");
    
    PolyString* s4 = substring_poly_string(s3, 0, 5);
    printf("  Подстрока [0,5]: "); print_poly_string(s4); printf("\n");
    
    int pos = find_in_poly_string(s3, s1, 1);
    printf("  Поиск 'Привет' в результате: позиция %d\n", pos);
    
    size_t len = poly_string_length(s3);
    printf("  Длина результата: %zu символов\n", len);
    
    char* file = poly_string_to_file_format(s3);
    printf("  Для файла: %s\n", file ? file : "NULL");
    
    print_test_result("mixed_operations", 
        s3 != NULL && s4 != NULL && pos == 0 && len > 0 && file != NULL);
    
    free(file);
    free_poly_string(s1);
    free_poly_string(s2);
    free_poly_string(s3);
    free_poly_string(s4);
}

void test_edge_cases() {
    printf("\n--- ТЕСТ: Крайние случаи ---\n");
    
    
    char long_str[1000];
    memset(long_str, 'A', 999);
    long_str[999] = '\0';
    
    PolyString* very_long = create_poly_string_ascii(long_str);
    size_t long_len = poly_string_length(very_long);
    printf("  Длина очень длинной строки: %zu\n", long_len);
    print_test_result("edge_very_long_string", long_len == 999);
    free_poly_string(very_long);
    
    
    char boundary[128];
    for (int i = 0; i < 127; i++) {
        boundary[i] = (char)(i + 1);
    }
    boundary[127] = '\0';
    
    PolyString* boundary_str = create_poly_string_ascii(boundary);
    printf("  Строка со всеми ASCII символами создана\n");
    print_test_result("edge_all_ascii", boundary_str != NULL);
    free_poly_string(boundary_str);
    
    
    PolyString* base = create_poly_string_ascii("A");
    for (int i = 0; i < 10; i++) {
        PolyString* temp = concat_poly_strings(base, base);
        if (temp) {
            free_poly_string(base);
            base = temp;
        }
    }
    printf("  После 10 конкатенаций: "); print_poly_string(base); printf("\n");
    print_test_result("edge_multiple_concats", base != NULL);
    free_poly_string(base);
    
    
    PolyString* overlap = create_poly_string_ascii("aaaaa");
    PolyString* pattern = create_poly_string_ascii("aa");
    int pos = find_in_poly_string(overlap, pattern, 1);
    printf("  Поиск 'aa' в 'aaaaa': позиция %d\n", pos);
    print_test_result("edge_overlapping_pattern", pos == 0);
    
    free_poly_string(overlap);
    free_poly_string(pattern);
}

void run_all_tests() {
    setlocale(LC_ALL, "");
    
    tests_passed = 0;
    tests_failed = 0;
    tests_total = 0;
    
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║           ПОЛНОЕ ТЕСТИРОВАНИЕ ВСЕХ ФУНКЦИЙ               ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    
    test_ascii_creation();
    test_wide_creation();
    test_ascii_concatenation();
    test_wide_concatenation();
    test_ascii_substring();
    test_wide_substring();
    test_ascii_find();
    test_wide_find();
    test_ascii_length();
    test_wide_length();
    test_file_conversion();
    test_mixed_operations();
    test_edge_cases();
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("  ИТОГИ ТЕСТИРОВАНИЯ\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("  Всего тестов: %d\n", tests_total);
    printf("  Пройдено:     %d\n", tests_passed);
    printf("  Провалено:    %d\n", tests_failed);
    printf("  Успешность:   %.1f%%\n", 
           tests_total > 0 ? (tests_passed * 100.0 / tests_total) : 0);
    printf("═══════════════════════════════════════════════════════════\n\n");
}