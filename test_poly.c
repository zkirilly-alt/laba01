#include "test_poly.h"

static int tests_passed = 0;
static int tests_failed = 0;
static int tests_total = 0;

void print_test_result(const char* test_name, int passed, const char* expected, const char* got) {
    tests_total++;
    if (passed) {
        tests_passed++;
        printf("ВЫПОЛНЕНО %s\n", test_name);
    } else {
        tests_failed++;
        printf("ОШИБКА %s\n", test_name);
        printf("  ожидалось: %s\n", expected);
        printf("  получено: %s\n", got);
    }
}

void test_ascii_creation() {
    printf("\nТЕСТ Создание ASCII строк\n");
    
    PolyString* str1 = create_poly_string_ascii("Hello");
    int passed1 = (str1 != NULL && strcmp((char*)str1->data, "Hello") == 0);
    print_test_result("create_poly_string_ascii нормальная строка", passed1, "не NULL и строка Hello", 
                      passed1 ? "Hello" : (str1 ? (char*)str1->data : "NULL"));
    
    PolyString* str2 = create_poly_string_ascii("");
    int passed2 = (str2 != NULL && strlen((char*)str2->data) == 0);
    print_test_result("create_poly_string_ascii пустая строка", passed2, "не NULL и длина 0", 
                      passed2 ? "пустая строка" : (str2 ? "строка не пустая" : "NULL"));
    
    PolyString* str3 = create_poly_string_ascii("123!@#");
    int passed3 = (str3 != NULL && strcmp((char*)str3->data, "123!@#") == 0);
    print_test_result("create_poly_string_ascii спецсимволы", passed3, "123!@#", 
                      passed3 ? "123!@#" : (str3 ? (char*)str3->data : "NULL"));
    
    PolyString* str4 = create_poly_string_ascii(NULL);
    int passed4 = (str4 == NULL);
    print_test_result("create_poly_string_ascii NULL указатель", passed4, "NULL", 
                      passed4 ? "NULL" : "не NULL");
    
    free_poly_string(str1);
    free_poly_string(str2);
    free_poly_string(str3);
}

void test_wide_creation() {
    printf("\nТЕСТ Создание Wide строк\n");
    
    PolyString* str1 = create_poly_string_wide_from_utf8("Привет");
    int passed1 = (str1 != NULL);
    print_test_result("create_poly_string_wide русские буквы", passed1, "не NULL", 
                      passed1 ? "строка создана" : "NULL");
    
    PolyString* str2 = create_poly_string_wide_from_utf8("Hello Привет");
    int passed2 = (str2 != NULL);
    print_test_result("create_poly_string_wide смешанная строка", passed2, "не NULL", 
                      passed2 ? "строка создана" : "NULL");
    
    PolyString* str3 = create_poly_string_wide_from_utf8("");
    int passed3 = (str3 != NULL);
    print_test_result("create_poly_string_wide пустая строка", passed3, "не NULL", 
                      passed3 ? "строка создана" : "NULL");
    
    PolyString* str4 = create_poly_string_wide_from_utf8("Euro: €, Copyright: ©");
    int passed4 = (str4 != NULL);
    print_test_result("create_poly_string_wide Unicode символы", passed4, "не NULL", 
                      passed4 ? "строка создана" : "NULL");
    
    PolyString* str5 = create_poly_string_wide_from_utf8(NULL);
    int passed5 = (str5 == NULL);
    print_test_result("create_poly_string_wide NULL указатель", passed5, "NULL", 
                      passed5 ? "NULL" : "не NULL");
    
    free_poly_string(str1);
    free_poly_string(str2);
    free_poly_string(str3);
    free_poly_string(str4);
}

void test_ascii_concatenation() {
    printf("\nТЕСТ Конкатенация ASCII строк\n");
    
    PolyString* a1 = create_poly_string_ascii("Hello ");
    PolyString* b1 = create_poly_string_ascii("World");
    PolyString* r1 = concat_poly_strings(a1, b1);
    int passed1 = (r1 != NULL && strcmp((char*)r1->data, "Hello World") == 0);
    print_test_result("concat_ascii_normal", passed1, "Hello World", 
                      passed1 ? "Hello World" : (r1 ? (char*)r1->data : "NULL"));
    free_poly_string(r1);
    
    PolyString* a2 = create_poly_string_ascii("Hello");
    PolyString* b2 = create_poly_string_ascii("");
    PolyString* r2 = concat_poly_strings(a2, b2);
    int passed2 = (r2 != NULL && strcmp((char*)r2->data, "Hello") == 0);
    print_test_result("concat_ascii_с пустой строкой", passed2, "Hello", 
                      passed2 ? "Hello" : (r2 ? (char*)r2->data : "NULL"));
    free_poly_string(r2);
    
    PolyString* r3 = concat_poly_strings(a2, NULL);
    int passed3 = (r3 == NULL);
    print_test_result("concat_ascii_с NULL", passed3, "NULL", 
                      passed3 ? "NULL" : "не NULL");
    
    PolyString* wide = create_poly_string_wide_from_utf8("Привет");
    PolyString* r4 = concat_poly_strings(a2, wide);
    int passed4 = (r4 == NULL);
    print_test_result("concat_ascii_разные типы", passed4, "NULL", 
                      passed4 ? "NULL" : "не NULL");
    
    free_poly_string(a1);
    free_poly_string(b1);
    free_poly_string(a2);
    free_poly_string(b2);
    free_poly_string(wide);
}

void test_wide_concatenation() {
    printf("\nТЕСТ Конкатенация Wide строк\n");
    
    PolyString* a1 = create_poly_string_wide_from_utf8("Привет, ");
    PolyString* b1 = create_poly_string_wide_from_utf8("мир!");
    PolyString* r1 = concat_poly_strings(a1, b1);
    int passed1 = (r1 != NULL);
    print_test_result("concat_wide_русские строки", passed1, "не NULL", 
                      passed1 ? "строка создана" : "NULL");
    free_poly_string(r1);
    
    PolyString* a2 = create_poly_string_wide_from_utf8("Цена: ");
    PolyString* b2 = create_poly_string_wide_from_utf8("100 €");
    PolyString* r2 = concat_poly_strings(a2, b2);
    int passed2 = (r2 != NULL);
    print_test_result("concat_wide_Unicode символы", passed2, "не NULL", 
                      passed2 ? "строка создана" : "NULL");
    free_poly_string(r2);
    
    PolyString* a3 = create_poly_string_wide_from_utf8("Привет");
    PolyString* b3 = create_poly_string_wide_from_utf8("");
    PolyString* r3 = concat_poly_strings(a3, b3);
    int passed3 = (r3 != NULL);
    print_test_result("concat_wide_с пустой строкой", passed3, "не NULL", 
                      passed3 ? "строка создана" : "NULL");
    free_poly_string(r3);
    
    free_poly_string(a1);
    free_poly_string(b1);
    free_poly_string(a2);
    free_poly_string(b2);
    free_poly_string(a3);
    free_poly_string(b3);
}

void test_ascii_substring() {
    printf("\nТЕСТ Подстроки ASCII\n");
    
    PolyString* str = create_poly_string_ascii("Hello World");
    
    PolyString* sub1 = substring_poly_string(str, 0, 4);
    int passed1 = (sub1 != NULL && strcmp((char*)sub1->data, "Hello") == 0);
    print_test_result("substring_ascii_начало строки", passed1, "Hello", 
                      passed1 ? "Hello" : (sub1 ? (char*)sub1->data : "NULL"));
    free_poly_string(sub1);
    
    PolyString* sub2 = substring_poly_string(str, 6, 10);
    int passed2 = (sub2 != NULL && strcmp((char*)sub2->data, "World") == 0);
    print_test_result("substring_ascii_конец строки", passed2, "World", 
                      passed2 ? "World" : (sub2 ? (char*)sub2->data : "NULL"));
    free_poly_string(sub2);
    
    PolyString* sub3 = substring_poly_string(str, 4, 4);
    int passed3 = (sub3 != NULL && strcmp((char*)sub3->data, "o") == 0);
    print_test_result("substring_ascii_один символ", passed3, "o", 
                      passed3 ? "o" : (sub3 ? (char*)sub3->data : "NULL"));
    free_poly_string(sub3);
    
    PolyString* sub4 = substring_poly_string(str, -5, 4);
    int passed4 = (sub4 != NULL && strcmp((char*)sub4->data, "Hello") == 0);
    print_test_result("substring_ascii_отрицательный старт", passed4, "Hello", 
                      passed4 ? "Hello" : (sub4 ? (char*)sub4->data : "NULL"));
    free_poly_string(sub4);
    
    PolyString* sub5 = substring_poly_string(str, 0, 100);
    int passed5 = (sub5 != NULL && strcmp((char*)sub5->data, "Hello World") == 0);
    print_test_result("substring_ascii_конец больше длины", passed5, "Hello World", 
                      passed5 ? "Hello World" : (sub5 ? (char*)sub5->data : "NULL"));
    free_poly_string(sub5);
    
    PolyString* sub6 = substring_poly_string(str, 10, 5);
    int passed6 = (sub6 == NULL);
    print_test_result("substring_ascii_старт больше конца", passed6, "NULL", 
                      passed6 ? "NULL" : "не NULL");
    
    free_poly_string(str);
}

void test_wide_substring() {
    printf("\nТЕСТ Подстроки Wide\n");
    
    PolyString* str = create_poly_string_wide_from_utf8("Привет мир");
    
    PolyString* sub1 = substring_poly_string(str, 0, 5);
    int passed1 = (sub1 != NULL);
    print_test_result("substring_wide_начало строки", passed1, "не NULL", 
                      passed1 ? "подстрока создана" : "NULL");
    free_poly_string(sub1);
    
    PolyString* sub2 = substring_poly_string(str, 7, 10);
    int passed2 = (sub2 != NULL);
    print_test_result("substring_wide_конец строки", passed2, "не NULL", 
                      passed2 ? "подстрока создана" : "NULL");
    free_poly_string(sub2);
    
    PolyString* sub3 = substring_poly_string(str, 3, 8);
    int passed3 = (sub3 != NULL);
    print_test_result("substring_wide_середина строки", passed3, "не NULL", 
                      passed3 ? "подстрока создана" : "NULL");
    free_poly_string(sub3);
    
    PolyString* sub4 = substring_poly_string(str, 5, 4);
    int passed4 = (sub4 == NULL);
    print_test_result("substring_wide_некорректный диапазон", passed4, "NULL", 
                      passed4 ? "NULL" : "не NULL");
    
    free_poly_string(str);
}

void test_ascii_find() {
    printf("\nТЕСТ Поиск в ASCII строках\n");
    
    PolyString* text = create_poly_string_ascii("Hello World, hello world");
    
    PolyString* pattern1 = create_poly_string_ascii("World");
    int pos1 = find_in_poly_string(text, pattern1, 1);
    char expected1[50];
    char got1[50];
    sprintf(expected1, "%d", 6);
    sprintf(got1, "%d", pos1);
    int passed1 = (pos1 == 6);
    print_test_result("find_ascii_чувствительный найден", passed1, expected1, got1);
    
    PolyString* pattern2 = create_poly_string_ascii("world");
    int pos2 = find_in_poly_string(text, pattern2, 1);
    char expected2[50];
    char got2[50];
    sprintf(expected2, "%d", -1);
    sprintf(got2, "%d", pos2);
    int passed2 = (pos2 == -1);
    print_test_result("find_ascii_чувствительный не найден", passed2, expected2, got2);
    
    int pos3 = find_in_poly_string(text, pattern2, 0);
    char expected3[50];
    char got3[50];
    sprintf(expected3, "%d", 6);
    sprintf(got3, "%d", pos3);
    int passed3 = (pos3 == 6 || pos3 == 19);
    print_test_result("find_ascii_нечувствительный", passed3, expected3, got3);
    
    PolyString* pattern3 = create_poly_string_ascii("");
    int pos4 = find_in_poly_string(text, pattern3, 1);
    char expected4[50];
    char got4[50];
    sprintf(expected4, "%d", 0);
    sprintf(got4, "%d", pos4);
    int passed4 = (pos4 == 0);
    print_test_result("find_ascii_пустой образец", passed4, expected4, got4);
    
    int pos5 = find_in_poly_string(text, NULL, 1);
    char expected5[50];
    char got5[50];
    sprintf(expected5, "%d", -1);
    sprintf(got5, "%d", pos5);
    int passed5 = (pos5 == -1);
    print_test_result("find_ascii_NULL образец", passed5, expected5, got5);
    
    PolyString* pattern4 = create_poly_string_ascii("Hello World, hello world!!!");
    int pos6 = find_in_poly_string(text, pattern4, 1);
    char expected6[50];
    char got6[50];
    sprintf(expected6, "%d", -1);
    sprintf(got6, "%d", pos6);
    int passed6 = (pos6 == -1);
    print_test_result("find_ascii_образец длиннее текста", passed6, expected6, got6);
    
    free_poly_string(text);
    free_poly_string(pattern1);
    free_poly_string(pattern2);
    free_poly_string(pattern3);
    free_poly_string(pattern4);
}

void test_wide_find() {
    printf("\nТЕСТ Поиск в Wide строках\n");
    
    PolyString* text = create_poly_string_wide_from_utf8("Привет мир, привет мир");
    
    PolyString* pattern1 = create_poly_string_wide_from_utf8("мир");
    int pos1 = find_in_poly_string(text, pattern1, 1);
    char expected1[50];
    char got1[50];
    sprintf(expected1, "%d", 7);
    sprintf(got1, "%d", pos1);
    int passed1 = (pos1 == 7);
    print_test_result("find_wide_русское слово", passed1, expected1, got1);
    
    PolyString* pattern2 = create_poly_string_wide_from_utf8("ПРИВЕТ");
    int pos2 = find_in_poly_string(text, pattern2, 1);
    char expected2[50];
    char got2[50];
    sprintf(expected2, "%d", -1);
    sprintf(got2, "%d", pos2);
    int passed2 = (pos2 == -1);
    print_test_result("find_wide_регистрозависимость", passed2, expected2, got2);
    
    PolyString* pattern3 = create_poly_string_wide_from_utf8("Привет");
    int pos3 = find_in_poly_string(text, pattern3, 1);
    char expected3[50];
    char got3[50];
    sprintf(expected3, "%d", 0);
    sprintf(got3, "%d", pos3);
    int passed3 = (pos3 == 0);
    print_test_result("find_wide_начало строки", passed3, expected3, got3);
    
    free_poly_string(text);
    free_poly_string(pattern1);
    free_poly_string(pattern2);
    free_poly_string(pattern3);
}

void test_ascii_length() {
    printf("\nТЕСТ Длина ASCII строк\n");
    
    PolyString* str1 = create_poly_string_ascii("Hello");
    size_t len1 = poly_string_length(str1);
    char expected1[50];
    char got1[50];
    sprintf(expected1, "%zu", 5);
    sprintf(got1, "%zu", len1);
    int passed1 = (len1 == 5);
    print_test_result("length_ascii_normal", passed1, expected1, got1);
    free_poly_string(str1);
    
    PolyString* str2 = create_poly_string_ascii("");
    size_t len2 = poly_string_length(str2);
    char expected2[50];
    char got2[50];
    sprintf(expected2, "%zu", 0);
    sprintf(got2, "%zu", len2);
    int passed2 = (len2 == 0);
    print_test_result("length_ascii_пустая строка", passed2, expected2, got2);
    free_poly_string(str2);
    
    PolyString* str3 = create_poly_string_ascii("Hello World! 123");
    size_t len3 = poly_string_length(str3);
    char expected3[50];
    char got3[50];
    sprintf(expected3, "%zu", 16);
    sprintf(got3, "%zu", len3);
    int passed3 = (len3 == 16);
    print_test_result("length_ascii_с пробелами", passed3, expected3, got3);
    free_poly_string(str3);
    
    size_t len4 = poly_string_length(NULL);
    char expected4[50];
    char got4[50];
    sprintf(expected4, "%zu", 0);
    sprintf(got4, "%zu", len4);
    int passed4 = (len4 == 0);
    print_test_result("length_ascii_NULL", passed4, expected4, got4);
}

void test_wide_length() {
    printf("\nТЕСТ Длина Wide строк\n");
    
    PolyString* str1 = create_poly_string_wide_from_utf8("Привет");
    size_t len1 = poly_string_length(str1);
    char expected1[50];
    char got1[50];
    sprintf(expected1, "%zu", 6);
    sprintf(got1, "%zu", len1);
    int passed1 = (len1 == 6);
    print_test_result("length_wide_русские буквы", passed1, expected1, got1);
    free_poly_string(str1);
    
    PolyString* str2 = create_poly_string_wide_from_utf8("Hello Привет");
    size_t len2 = poly_string_length(str2);
    char expected2[50];
    char got2[50];
    sprintf(expected2, "%zu", 11);
    sprintf(got2, "%zu", len2);
    int passed2 = (len2 == 11);
    print_test_result("length_wide_смешанная строка", passed2, expected2, got2);
    free_poly_string(str2);
    
    PolyString* str3 = create_poly_string_wide_from_utf8("Euro: €");
    size_t len3 = poly_string_length(str3);
    char expected3[50];
    char got3[50];
    sprintf(expected3, "%zu", 7);
    sprintf(got3, "%zu", len3);
    int passed3 = (len3 == 7);
    print_test_result("length_wide_Unicode символы", passed3, expected3, got3);
    free_poly_string(str3);
    
    PolyString* str4 = create_poly_string_wide_from_utf8("");
    size_t len4 = poly_string_length(str4);
    char expected4[50];
    char got4[50];
    sprintf(expected4, "%zu", 0);
    sprintf(got4, "%zu", len4);
    int passed4 = (len4 == 0);
    print_test_result("length_wide_пустая строка", passed4, expected4, got4);
    free_poly_string(str4);
}

void test_file_conversion() {
    printf("\nТЕСТ Конвертация для файла\n");
    
    PolyString* ascii = create_poly_string_ascii("ASCII text");
    char* ascii_file = poly_string_to_file_format(ascii);
    int passed1 = (ascii_file != NULL && strcmp(ascii_file, "ASCII text") == 0);
    print_test_result("file_ascii_конвертация", passed1, "ASCII text", 
                      passed1 ? "ASCII text" : (ascii_file ? ascii_file : "NULL"));
    free(ascii_file);
    
    PolyString* wide1 = create_poly_string_wide_from_utf8("Русский текст");
    char* wide_file1 = poly_string_to_file_format(wide1);
    int passed2 = (wide_file1 != NULL);
    print_test_result("file_wide_русские буквы", passed2, "не NULL", 
                      passed2 ? wide_file1 : "NULL");
    free(wide_file1);
    
    PolyString* wide2 = create_poly_string_wide_from_utf8("Цена 100 €");
    char* wide_file2 = poly_string_to_file_format(wide2);
    int passed3 = (wide_file2 != NULL);
    print_test_result("file_wide_Unicode символы", passed3, "не NULL", 
                      passed3 ? wide_file2 : "NULL");
    free(wide_file2);
    
    PolyString* empty = create_poly_string_ascii("");
    char* empty_file = poly_string_to_file_format(empty);
    int passed4 = (empty_file != NULL && strlen(empty_file) == 0);
    print_test_result("file_empty_пустая строка", passed4, "пустая строка", 
                      passed4 ? "пустая строка" : (empty_file ? "не пустая" : "NULL"));
    free(empty_file);
    
    char* null_file = poly_string_to_file_format(NULL);
    int passed5 = (null_file == NULL);
    print_test_result("file_null_NULL указатель", passed5, "NULL", 
                      passed5 ? "NULL" : "не NULL");
    
    free_poly_string(ascii);
    free_poly_string(wide1);
    free_poly_string(wide2);
    free_poly_string(empty);
}

void test_edge_cases() {
    printf("\nТЕСТ Крайние случаи\n");
    
    char long_str[1000];
    for (int i = 0; i < 999; i++) {
        long_str[i] = 'A';
    }
    long_str[999] = '\0';
    
    PolyString* very_long = create_poly_string_ascii(long_str);
    size_t long_len = poly_string_length(very_long);
    char expected1[50];
    char got1[50];
    sprintf(expected1, "%zu", 999);
    sprintf(got1, "%zu", long_len);
    int passed1 = (long_len == 999);
    print_test_result("edge_очень длинная строка", passed1, expected1, got1);
    free_poly_string(very_long);
    
    char boundary[128];
    for (int i = 0; i < 127; i++) {
        boundary[i] = (char)(i + 1);
    }
    boundary[127] = '\0';
    
    PolyString* boundary_str = create_poly_string_ascii(boundary);
    int passed2 = (boundary_str != NULL);
    print_test_result("edge_все ASCII символы", passed2, "не NULL", 
                      passed2 ? "строка создана" : "NULL");
    free_poly_string(boundary_str);
    
    PolyString* base = create_poly_string_ascii("A");
    for (int i = 0; i < 10; i++) {
        PolyString* temp = concat_poly_strings(base, base);
        if (temp) {
            free_poly_string(base);
            base = temp;
        }
    }
    size_t final_len = poly_string_length(base);
    char expected3[50];
    char got3[50];
    sprintf(expected3, "%zu", 1024);
    sprintf(got3, "%zu", final_len);
    int passed3 = (final_len == 1024);
    print_test_result("edge_многократные конкатенации", passed3, expected3, got3);
    free_poly_string(base);
    
    PolyString* overlap = create_poly_string_ascii("aaaaa");
    PolyString* pattern = create_poly_string_ascii("aa");
    int pos = find_in_poly_string(overlap, pattern, 1);
    char expected4[50];
    char got4[50];
    sprintf(expected4, "%d", 0);
    sprintf(got4, "%d", pos);
    int passed4 = (pos == 0);
    print_test_result("edge_перекрывающийся паттерн", passed4, expected4, got4);
    
    free_poly_string(overlap);
    free_poly_string(pattern);
}

void run_all_tests() {
    setlocale(LC_ALL, "");
    
    tests_passed = 0;
    tests_failed = 0;
    tests_total = 0;
    
    printf("\nЗАПУСК ТЕСТИРОВАНИЯ ВСЕХ ФУНКЦИЙ\n");
    
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
    test_edge_cases();
    
    printf("\nИТОГИ ТЕСТИРОВАНИЯ\n");
    printf("Всего тестов %d\n", tests_total);
    printf("Пройдено %d\n", tests_passed);
    printf("Провалено %d\n", tests_failed);
    printf("Успешность %.1f процентов\n", 
           tests_total > 0 ? (tests_passed * 100.0 / tests_total) : 0);
}