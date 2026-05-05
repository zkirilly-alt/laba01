#include "inout.h"
#include <wchar.h>
#include <locale.h>

const size_t INITIAL_STRING_ARRAY_CAPACITY = 10;
const size_t READ_STRING_BUFFER_SIZE = 4096;
const size_t READ_STRING_INITIAL_SIZE = 128;
const size_t FILENAME_BUFFER_SIZE = 256;
const size_t LINE_BUFFER_SIZE = 1024;
const size_t MAX_STRING_LENGTH = 256;

int read_ascii_string(char **result) {
    char *text = NULL;
    size_t size = READ_STRING_INITIAL_SIZE;
    size_t len = 0;
    int c;
    
    text = malloc(size);
    if (text == NULL) {
        printf("Ошибка выделения памяти\n");
        return 0;
    }
    
    while ((c = getchar()) != EOF) {
        if (c == '\n') break;
        if (c > 127) {
            free(text);
            while ((c = getchar()) != EOF && c != '\n');
            return 0;
        }
        text[len++] = (char)c;
        if (len >= size) {
            size *= 2;
            char* new_text = realloc(text, size);
            if (new_text == NULL) {
                printf("Ошибка перевыделения памяти\n");
                free(text);
                return 0;
            }
            text = new_text;
        }
    }
    
    if (len == 0) {
        free(text);
        return -1;
    }
    
    text[len] = '\0';
    *result = text;
    return 1;
}

void save_to_char_file(const char *text) {
    FILE* char_file = fopen("output_char.txt", "a");
    if (char_file == NULL) {
        printf("Ошибка открытия файла output_char.txt\n");
        return;
    }
    fprintf(char_file, "%s\n", text);
    fclose(char_file);
    printf("\nСтрока добавлена в файл output_char.txt\n");
}

void save_to_char16_file(const char *text) {
    FILE* char16_file = fopen("output_char16.txt", "ab");
    if (char16_file == NULL) {
        printf("Ошибка открытия файла output_char16.txt\n");
        return;
    }
    
    fseek(char16_file, 0, SEEK_END);
    long file_size = ftell(char16_file);
    
    if (file_size == 0) {
        unsigned char bom[] = {0xFF, 0xFE};
        fwrite(bom, 1, 2, char16_file);
    }
    
    size_t len = mbstowcs(NULL, text, 0);
    if (len == (size_t)-1) {
        printf("Ошибка конвертации строки\n");
        fclose(char16_file);
        return;
    }
    
    wchar_t *wstr = malloc((len + 1) * sizeof(wchar_t));
    if (!wstr) {
        printf("Ошибка выделения памяти\n");
        fclose(char16_file);
        return;
    }
    
    mbstowcs(wstr, text, len + 1);
    
    for (size_t i = 0; i < len; i++) {
        unsigned short code = (unsigned short)wstr[i];
        unsigned char low = code & 0xFF;
        unsigned char high = (code >> 8) & 0xFF;
        fwrite(&low, 1, 1, char16_file);
        fwrite(&high, 1, 1, char16_file);
    }
    
    unsigned short newline = (unsigned short)'\n';
    unsigned char nl_low = newline & 0xFF;
    unsigned char nl_high = (newline >> 8) & 0xFF;
    fwrite(&nl_low, 1, 1, char16_file);
    fwrite(&nl_high, 1, 1, char16_file);
    
    fclose(char16_file);
    free(wstr);
    printf("Строка сохранена в output_char16.txt (%zu символов)\n", len);
}

StringArray* read_strings_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла %s\n", filename);
        return NULL;
    }
    
    StringArray* array = malloc(sizeof(StringArray));
    if (array == NULL) {
        printf("Ошибка выделения памяти\n");
        fclose(file);
        return NULL;
    }
    
    array->strings = NULL;
    array->count = 0;
    array->capacity = 0;
    
    char buffer[LINE_BUFFER_SIZE];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) continue;
        
        if (array->count == 0) {
            array->capacity = INITIAL_STRING_ARRAY_CAPACITY;
            array->strings = malloc(array->capacity * MAX_STRING_LENGTH);
            if (array->strings == NULL) {
                printf("Ошибка выделения памяти\n");
                free(array);
                fclose(file);
                return NULL;
            }
            memset(array->strings, 0, array->capacity * MAX_STRING_LENGTH);
        }
        
        if (array->count >= array->capacity) {
            size_t new_capacity = array->capacity * 2;
            char* new_strings = realloc(array->strings, new_capacity * MAX_STRING_LENGTH);
            if (new_strings == NULL) {
                printf("Ошибка перевыделения памяти\n");
                break;
            }
            array->strings = new_strings;
            memset(array->strings + (array->capacity * MAX_STRING_LENGTH), 0, 
                   (new_capacity - array->capacity) * MAX_STRING_LENGTH);
            array->capacity = new_capacity;
        }
        
        char* dest = array->strings + (array->count * MAX_STRING_LENGTH);
        strncpy(dest, buffer, MAX_STRING_LENGTH - 1);
        dest[MAX_STRING_LENGTH - 1] = '\0';
        array->count++;
    }
    
    fclose(file);
    
    if (array->count == 0) {
        printf("Файл не содержит строк\n");
        free_string_array(array);
        return NULL;
    }
    
    return array;
}

void free_string_array(StringArray* array) {
    if (array == NULL) return;
    free(array->strings);
    free(array);
}

void display_strings(const StringArray* array) {
    if (array == NULL || array->count == 0) {
        printf("Нет строк для отображения\n");
        return;
    }
    
    printf("\nСТРОКИ ИЗ ФАЙЛА\n");
    for (size_t i = 0; i < array->count; i++) {
        char* str = array->strings + (i * MAX_STRING_LENGTH);
        printf("%zu. %s\n", i + 1, str);
    }
    printf("\n");
}

char* select_string(const StringArray* array) {
    if (array == NULL || array->count == 0) return NULL;
    
    int choice;
    printf("\nВыберите номер строки (1-%zu): ", array->count);
    
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return NULL;
    }
    clear_input_buffer();
    
    if (choice < 1 || choice > (int)array->count) {
        printf("Неверный выбор\n");
        return NULL;
    }
    
    char* source = array->strings + ((choice - 1) * MAX_STRING_LENGTH);
    char* selected = malloc(strlen(source) + 1);
    if (selected != NULL) {
        strcpy(selected, source);
    }
    
    return selected;
}

int input_from_file() {
    char filename[FILENAME_BUFFER_SIZE];
    StringArray* array = NULL;
    char* selected_string = NULL;
    int choice;
    
    printf("\nВВОД СТРОКИ ИЗ ФАЙЛА\n");
    printf("Введите имя файла (по умолчанию 'input.txt'): ");
    
    if (fgets(filename, sizeof(filename), stdin) != NULL) {
        filename[strcspn(filename, "\n")] = 0;
        if (strlen(filename) == 0) {
            strcpy(filename, "input.txt");
        }
    }
    
    array = read_strings_from_file(filename);
    if (array == NULL) return 0;
    
    display_strings(array);
    
    selected_string = select_string(array);
    if (selected_string == NULL) {
        free_string_array(array);
        return 0;
    }
    
    printf("\nВыбрана строка: \"%s\"\n", selected_string);
    
    printf("\nВыберите формат сохранения:\n");
    printf("1 - char (1 байт на символ)\n");
    printf("2 - char16_t (2 байта на символ)\n");
    printf("Ваш выбор: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Ошибка ввода\n");
        free(selected_string);
        free_string_array(array);
        return 0;
    }
    clear_input_buffer();
    
    if (choice == 1) {
        save_to_char_file(selected_string);
    } else if (choice == 2) {
        save_to_char16_file(selected_string);
    } else {
        printf("Неверный выбор\n");
    }
    
    free(selected_string);
    free_string_array(array);
    
    return 1;
}

int input_char_string() {
    int result;
    char *text = NULL;
    
    do {
        printf("\nВведите строку (разрешены только 1-байтовые символы):\n");
        printf(">>> ");
        result = read_ascii_string(&text);
        if (result == 0) {
            printf("Ошибка: обнаружены многобайтовые символы! Попробуйте снова.\n");
        } else if (result == -1) {
            printf("Строка пуста. Попробуйте снова.\n");
        }
    } while (result != 1);
    
    save_to_char_file(text);
    free(text);
    return 1;
}

int input_char16_string() {
    char buffer[READ_STRING_BUFFER_SIZE];
    
    printf("\nВведите строку для сохранения в формате char16_t:\n");
    printf(">>> ");
    
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) {
            printf("Строка пуста\n");
            return 0;
        }
        save_to_char16_file(buffer);
    }
    
    return 1;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

PolyString* get_poly_string_from_source(const char* prompt) {
    int source_choice;
    char buffer[4096];
    
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
        int capacity = 10;
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
    printf("\nКОНКАТЕНАЦИЯ СТРОК\n");
    
    PolyString* str1 = get_poly_string_from_source("Выбор первой строки:");
    if (!str1) return;
    
    PolyString* str2 = get_poly_string_from_source("Выбор второй строки:");
    if (!str2) {
        free_poly_string(str1);
        return;
    }
    
    PolyString* result = concat_poly_strings(str1, str2);
    char buffer[4096];
    
    printf("\nРЕЗУЛЬТАТ:\n");
    printf("Первая:  ");
    print_poly_string_to_buffer(str1, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    printf("Вторая:  ");
    print_poly_string_to_buffer(str2, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    printf("Результат: ");
    print_poly_string_to_buffer(result, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    
    free_poly_string(str1);
    free_poly_string(str2);
    if (result) free_poly_string(result);
}

void demo_substring() {
    printf("\nПОЛУЧЕНИЕ ПОДСТРОКИ\n");
    
    PolyString* str = get_poly_string_from_source("Выбор строки:");
    if (!str) return;
    
    int i, j;
    printf("Начальная позиция (в символах): ");
    scanf("%d", &i);
    printf("Конечная позиция (в символах): ");
    scanf("%d", &j);
    clear_input_buffer();
    
    PolyString* sub = substring_poly_string(str, i, j);
    char buffer[4096];
    
    printf("\nРЕЗУЛЬТАТ:\n");
    printf("Исходная: ");
    print_poly_string_to_buffer(str, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    printf("Подстрока [%d-%d]: ", i, j);
    if (sub) {
        print_poly_string_to_buffer(sub, buffer, sizeof(buffer));
        printf("%s\n", buffer);
        free_poly_string(sub);
    } else {
        printf("ОШИБКА\n");
    }
    
    free_poly_string(str);
}

void demo_search(int sensitive) {
    const char* mode = sensitive ? "ЧУВСТВИТЕЛЬНЫЙ" : "НЕЧУВСТВИТЕЛЬНЫЙ";
    printf("\nПОИСК ПОДСТРОКИ (%s)\n", mode);
    
    PolyString* text = get_poly_string_from_source("Выбор строки для поиска:");
    if (!text) return;
    
    PolyString* pattern = get_poly_string_from_source("Выбор образца для поиска:");
    if (!pattern) {
        free_poly_string(text);
        return;
    }
    
    int pos = find_in_poly_string(text, pattern, sensitive);
    char buffer[4096];
    
    printf("\nРЕЗУЛЬТАТ:\n");
    printf("Строка:   ");
    print_poly_string_to_buffer(text, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    printf("Образец:  ");
    print_poly_string_to_buffer(pattern, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    
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
        printf("\nРАБОТА СО СТРОКАМИ\n");
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

void allinout() {
    int choice;
    char continue_choice;
    
    do {
        printf("\nСОХРАНЕНИЕ СТРОКИ В ФАЙЛ\n");
        printf("Выберите источник ввода:\n");
        printf("1 - Ввод с клавиатуры (char)\n");
        printf("2 - Ввод с клавиатуры (char16_t)\n");
        printf("3 - Чтение из файла\n");
        printf("0 - Вернуться в главное меню\n");
        printf("Ваш выбор: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода\n");
            clear_input_buffer();
            continue;
        }
        
        clear_input_buffer();
        
        if (choice == 0) {
            printf("Возврат в главное меню...\n");
            return;
        }
        
        switch(choice) {
            case 1: input_char_string(); break;
            case 2: input_char16_string(); break;
            case 3: input_from_file(); break;
            default:
                printf("Неверный выбор. Пожалуйста, выберите 1, 2, 3 или 0.\n");
                continue;
        }
        
        printf("\nХотите сохранить еще одну строку? (y/n): ");
        continue_choice = getchar();
        clear_input_buffer();
        
    } while (continue_choice == 'y' || continue_choice == 'Y');
    
    printf("Возврат в главное меню...\n");
}