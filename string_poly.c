#include "string_poly.h"
#include "inout.h"

const size_t POLY_STRING_BUFFER_SIZE = 4096;
const size_t POLY_STRING_ARRAY_INITIAL_CAPACITY = 10;

int is_ascii_only(const char* str) {
    if (!str) return 1;
    while (*str) {
        if ((unsigned char)*str > 127) return 0;
        str++;
    }
    return 1;
}

char* wchar_to_utf8(const wchar_t* wstr) {
    if (!wstr) return NULL;
    
    size_t len = wcstombs(NULL, wstr, 0);
    if (len == (size_t)-1) return NULL;
    
    char* utf8 = malloc(len + 1);
    if (!utf8) return NULL;
    
    wcstombs(utf8, wstr, len + 1);
    return utf8;
}

wchar_t* utf8_to_wchar(const char* utf8) {
    if (!utf8) return NULL;
    
    size_t len = mbstowcs(NULL, utf8, 0);
    if (len == (size_t)-1) return NULL;
    
    wchar_t* wstr = malloc((len + 1) * sizeof(wchar_t));
    if (!wstr) return NULL;
    
    mbstowcs(wstr, utf8, len + 1);
    return wstr;
}

void ascii_print(const void* data) {
    if (!data) return;
    printf("%s", (const char*)data);
}

PolyString* ascii_concat(const PolyString* a, const PolyString* b) {
    if (!a || !b || !a->data || !b->data) return NULL;
    
    size_t len_a = strlen((const char*)a->data);
    size_t len_b = strlen((const char*)b->data);
    
    char* result = malloc(len_a + len_b + 1);
    if (!result) return NULL;
    
    strcpy(result, (const char*)a->data);
    strcat(result, (const char*)b->data);
    
    return create_poly_string_ascii(result);
}

PolyString* ascii_substring(const PolyString* str, int start, int end) {
    if (!str || !str->data) return NULL;
    
    const char* data = (const char*)str->data;
    size_t len = strlen(data);
    
    if (start < 0) start = 0;
    if (end < 0 || end >= (int)len) end = len - 1;
    if (start > end) return NULL;
    
    size_t sub_len = end - start + 1;
    char* result = malloc(sub_len + 1);
    if (!result) return NULL;
    
    strncpy(result, data + start, sub_len);
    result[sub_len] = '\0';
    
    return create_poly_string_ascii(result);
}

int ascii_find(const PolyString* text, const PolyString* pattern, int sensitive) {
    if (!text || !pattern || !text->data || !pattern->data) return -1;
    
    const char* t = (const char*)text->data;
    const char* p = (const char*)pattern->data;
    
    size_t tlen = strlen(t);
    size_t plen = strlen(p);
    
    if (plen == 0) return 0;
    if (plen > tlen) return -1;
    
    for (size_t i = 0; i <= tlen - plen; i++) {
        int found = 1;
        for (size_t j = 0; j < plen; j++) {
            char tc = t[i + j];
            char pc = p[j];
            
            if (!sensitive) {
                if (tc >= 'A' && tc <= 'Z') tc = tc - 'A' + 'a';
                if (pc >= 'A' && pc <= 'Z') pc = pc - 'A' + 'a';
            }
            
            if (tc != pc) {
                found = 0;
                break;
            }
        }
        if (found) return i;
    }
    
    return -1;
}

size_t ascii_length(const void* data) {
    if (!data) return 0;
    return strlen((const char*)data);
}

void ascii_free(void* data) {
    free(data);
}

char* ascii_to_file_string(const void* data) {
    if (!data) return NULL;
    
    const char* str = (const char*)data;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result) {
        strcpy(result, str);
    }
    return result;
}

void wide_print(const void* data) {
    if (!data) return;
    char* utf8 = wchar_to_utf8((const wchar_t*)data);
    if (utf8) {
        printf("%s", utf8);
        free(utf8);
    }
}

PolyString* wide_concat(const PolyString* a, const PolyString* b) {
    if (!a || !b || !a->data || !b->data) return NULL;
    
    const wchar_t* wa = (const wchar_t*)a->data;
    const wchar_t* wb = (const wchar_t*)b->data;
    
    size_t len_a = wcslen(wa);
    size_t len_b = wcslen(wb);
    
    wchar_t* result = malloc((len_a + len_b + 1) * sizeof(wchar_t));
    if (!result) return NULL;
    
    wcscpy(result, wa);
    wcscat(result, wb);
    
    return create_poly_string_wide(result);
}

PolyString* wide_substring(const PolyString* str, int start, int end) {
    if (!str || !str->data) return NULL;
    
    const wchar_t* data = (const wchar_t*)str->data;
    size_t len = wcslen(data);
    
    if (start < 0) start = 0;
    if (end < 0 || end >= (int)len) end = len - 1;
    if (start > end) return NULL;
    
    size_t sub_len = end - start + 1;
    wchar_t* result = malloc((sub_len + 1) * sizeof(wchar_t));
    if (!result) return NULL;
    
    wcsncpy(result, data + start, sub_len);
    result[sub_len] = L'\0';
    
    return create_poly_string_wide(result);
}

int wide_find(const PolyString* text, const PolyString* pattern, int sensitive) {
    if (!text || !pattern || !text->data || !pattern->data) return -1;
    
    const wchar_t* t = (const wchar_t*)text->data;
    const wchar_t* p = (const wchar_t*)pattern->data;
    
    size_t tlen = wcslen(t);
    size_t plen = wcslen(p);
    
    if (plen == 0) return 0;
    if (plen > tlen) return -1;
    
    for (size_t i = 0; i <= tlen - plen; i++) {
        int found = 1;
        for (size_t j = 0; j < plen; j++) {
            wchar_t tc = t[i + j];
            wchar_t pc = p[j];
            
            if (!sensitive) {
                if (tc >= L'A' && tc <= L'Z') tc = tc - L'A' + L'a';
                if (pc >= L'A' && pc <= L'Z') pc = pc - L'A' + L'a';
            }
            
            if (tc != pc) {
                found = 0;
                break;
            }
        }
        if (found) return i;
    }
    
    return -1;
}

size_t wide_length(const void* data) {
    if (!data) return 0;
    return wcslen((const wchar_t*)data);
}

void wide_free(void* data) {
    free(data);
}

char* wide_to_file_string(const void* data) {
    if (!data) return NULL;
    return wchar_to_utf8((const wchar_t*)data);
}

StringTypeInfo AsciiType = {
    .print = ascii_print,
    .concat = ascii_concat,
    .substring = ascii_substring,
    .find = ascii_find,
    .length = ascii_length,
    .free = ascii_free,
    .to_file_string = ascii_to_file_string
};

StringTypeInfo WideType = {
    .print = wide_print,
    .concat = wide_concat,
    .substring = wide_substring,
    .find = wide_find,
    .length = wide_length,
    .free = wide_free,
    .to_file_string = wide_to_file_string
};

PolyString* create_poly_string_ascii(const char* str) {
    if (!str) return NULL;
    
    PolyString* poly = malloc(sizeof(PolyString));
    if (!poly) return NULL;
    
    poly->data = malloc(strlen(str) + 1);
    if (!poly->data) {
        free(poly);
        return NULL;
    }
    
    strcpy((char*)poly->data, str);
    poly->type = &AsciiType;
    
    return poly;
}

PolyString* create_poly_string_wide(const wchar_t* str) {
    if (!str) return NULL;
    
    PolyString* poly = malloc(sizeof(PolyString));
    if (!poly) return NULL;
    
    poly->data = malloc((wcslen(str) + 1) * sizeof(wchar_t));
    if (!poly->data) {
        free(poly);
        return NULL;
    }
    
    wcscpy((wchar_t*)poly->data, str);
    poly->type = &WideType;
    
    return poly;
}

PolyString* create_poly_string_wide_from_utf8(const char* utf8_str) {
    if (!utf8_str) return NULL;
    
    wchar_t* wstr = utf8_to_wchar(utf8_str);
    if (!wstr) return NULL;
    
    PolyString* poly = create_poly_string_wide(wstr);
    free(wstr);
    
    return poly;
}

void free_poly_string(PolyString* str) {
    if (str) {
        if (str->type && str->type->free) {
            str->type->free(str->data);
        }
        free(str);
    }
}

void print_poly_string(const PolyString* str) {
    if (!str || !str->type || !str->type->print) {
        printf("NULL");
        return;
    }
    str->type->print(str->data);
}

PolyString* concat_poly_strings(const PolyString* a, const PolyString* b) {
    if (!a || !b || !a->type || !b->type || a->type != b->type) {
        return NULL;
    }
    
    return a->type->concat(a, b);
}

PolyString* substring_poly_string(const PolyString* str, int start, int end) {
    if (!str || !str->type || !str->type->substring) {
        return NULL;
    }
    
    return str->type->substring(str, start, end);
}

int find_in_poly_string(const PolyString* text, const PolyString* pattern, int sensitive) {
    if (!text || !pattern || !text->type || !pattern->type || 
        text->type != pattern->type || !text->type->find) {
        return -1;
    }
    return text->type->find(text, pattern, sensitive);
}

size_t poly_string_length(const PolyString* str) {
    if (!str || !str->type || !str->type->length) return 0;
    return str->type->length(str->data);
}

char* poly_string_to_file_format(const PolyString* str) {
    if (!str || !str->type || !str->type->to_file_string) return NULL;
    return str->type->to_file_string(str->data);
}

PolyString* get_poly_string_from_source(const char* prompt) {
    int source_choice;
    char buffer[POLY_STRING_BUFFER_SIZE];
    
    printf("\n%s\n", prompt);
    printf("Выберите источник:\n");
    printf("1 - Ввести с клавиатуры\n");
    printf("2 - Выбрать из файла input.txt\n");
    printf("Ваш выбор: ");
    
    if (scanf("%d", &source_choice) != 1) {
        clear_input_buffer();
        return NULL;
    }
    clear_input_buffer();
    
    if (source_choice == 1) {
        printf("Введите строку:\n>>> ");
        
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            
            if (is_ascii_only(buffer)) {
                return create_poly_string_ascii(buffer);
            } else {
                return create_poly_string_wide_from_utf8(buffer);
            }
        }
    }
    else if (source_choice == 2) {
        FILE* file = fopen("input.txt", "r");
        if (!file) {
            printf("Файл input.txt не найден\n");
            return NULL;
        }
        
        printf("\nСодержимое файла input.txt:\n");
        char** lines = NULL;
        int line_count = 0;
        int capacity = POLY_STRING_ARRAY_INITIAL_CAPACITY;
        int* line_types = NULL;
        
        lines = malloc(capacity * sizeof(char*));
        line_types = malloc(capacity * sizeof(int));
        if (!lines || !line_types) {
            fclose(file);
            free(lines);
            free(line_types);
            return NULL;
        }
        
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) == 0) continue;
            
            if (line_count >= capacity) {
                capacity *= 2;
                char** new_lines = realloc(lines, capacity * sizeof(char*));
                int* new_types = realloc(line_types, capacity * sizeof(int));
                if (!new_lines || !new_types) break;
                lines = new_lines;
                line_types = new_types;
            }
            
            lines[line_count] = malloc(strlen(buffer) + 1);
            if (lines[line_count]) {
                strcpy(lines[line_count], buffer);
                line_types[line_count] = is_ascii_only(buffer) ? 0 : 1;
                printf("%d. %s %s\n", line_count + 1, buffer, 
                       line_types[line_count] ? "(wide)" : "(ascii)");
                line_count++;
            }
        }
        fclose(file);
        
        if (line_count == 0) {
            printf("Файл пуст\n");
            free(lines);
            free(line_types);
            return NULL;
        }
        
        int choice;
        printf("Выберите номер строки (1-%d): ", line_count);
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            for (int i = 0; i < line_count; i++) free(lines[i]);
            free(lines);
            free(line_types);
            return NULL;
        }
        clear_input_buffer();
        
        if (choice < 1 || choice > line_count) {
            printf("Неверный выбор\n");
            for (int i = 0; i < line_count; i++) free(lines[i]);
            free(lines);
            free(line_types);
            return NULL;
        }
        
        PolyString* result;
        if (line_types[choice - 1] == 0) {
            result = create_poly_string_ascii(lines[choice - 1]);
        } else {
            result = create_poly_string_wide_from_utf8(lines[choice - 1]);
        }
        
        for (int i = 0; i < line_count; i++) free(lines[i]);
        free(lines);
        free(line_types);
        
        return result;
    }
    
    return NULL;
}

void demo_concatenation() {
    printf("\n--- КОНКАТЕНАЦИЯ СТРОК ---\n");
    
    PolyString* str1 = get_poly_string_from_source("Выбор первой строки:");
    if (!str1) return;
    
    PolyString* str2 = get_poly_string_from_source("Выбор второй строки:");
    if (!str2) {
        free_poly_string(str1);
        return;
    }
    
    PolyString* result = concat_poly_strings(str1, str2);
    
    printf("\nРЕЗУЛЬТАТ:\n");
    printf("Первая:  "); print_poly_string(str1); printf("\n");
    printf("Вторая:  "); print_poly_string(str2); printf("\n");
    printf("Результат: "); print_poly_string(result); printf("\n");
    
    char* file_str1 = poly_string_to_file_format(str1);
    char* file_str2 = poly_string_to_file_format(str2);
    char* file_result = poly_string_to_file_format(result);
    
    if (file_str1 && file_str2 && file_result) {
        printf("\nДля сохранения в файл:\n");
        printf("  %s + %s = %s\n", file_str1, file_str2, file_result);
    }
    
    free(file_str1);
    free(file_str2);
    free(file_result);
    
    free_poly_string(str1);
    free_poly_string(str2);
    if (result) free_poly_string(result);
}

void demo_substring() {
    printf("\n--- ПОЛУЧЕНИЕ ПОДСТРОКИ ---\n");
    
    PolyString* str = get_poly_string_from_source("Выбор строки:");
    if (!str) return;
    
    int i, j;
    printf("Начальная позиция (в символах): ");
    scanf("%d", &i);
    printf("Конечная позиция (в символах): ");
    scanf("%d", &j);
    clear_input_buffer();
    
    PolyString* sub = substring_poly_string(str, i, j);
    
    printf("\nРЕЗУЛЬТАТ:\n");
    printf("Исходная: "); print_poly_string(str); printf("\n");
    printf("Подстрока [%d-%d]: ", i, j);
    if (sub) {
        print_poly_string(sub);
        free_poly_string(sub);
    } else {
        printf("ОШИБКА");
    }
    printf("\n");
    
    free_poly_string(str);
}

void demo_search(int sensitive) {
    const char* mode = sensitive ? "ЧУВСТВИТЕЛЬНЫЙ" : "НЕЧУВСТВИТЕЛЬНЫЙ";
    printf("\n--- ПОИСК ПОДСТРОКИ (%s) ---\n", mode);
    
    PolyString* text = get_poly_string_from_source("Выбор строки для поиска:");
    if (!text) return;
    
    PolyString* pattern = get_poly_string_from_source("Выбор образца для поиска:");
    if (!pattern) {
        free_poly_string(text);
        return;
    }
    
    int pos = find_in_poly_string(text, pattern, sensitive);
    
    printf("\nРЕЗУЛЬТАТ:\n");
    printf("Строка:   "); print_poly_string(text); printf("\n");
    printf("Образец:  "); print_poly_string(pattern); printf("\n");
    
    if (pos >= 0) {
        printf("Найден на позиции: %d\n", pos);
    } else {
        printf("Не найден\n");
    }
    
    free_poly_string(text);
    free_poly_string(pattern);
}

void run_string_operations_menu() {
    int choice;
    
    do {
        printf("\n РАБОТА СО СТРОКАМИ \n");
        printf("1 - Конкатенация строк\n");
        printf("2 - Получение подстроки\n");
        printf("3 - Поиск подстроки (чувствительный)\n");
        printf("4 - Поиск подстроки (нечувствительный)\n");
        printf("0 - Вернуться в главное меню\n");
        printf("Ваш выбор: ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        switch(choice) {
            case 1: demo_concatenation(); break;
            case 2: demo_substring(); break;
            case 3: demo_search(1); break;
            case 4: demo_search(0); break;
            case 0: printf("Возврат...\n"); break;
            default: printf("Неверный выбор\n");
        }
    } while (choice != 0);
}