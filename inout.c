#include "inout.h"
#include <wchar.h>
#include <locale.h>

const size_t INITIAL_STRING_ARRAY_CAPACITY = 10;
const size_t READ_STRING_BUFFER_SIZE = 4096;
const size_t READ_STRING_INITIAL_SIZE = 128;
const size_t FILENAME_BUFFER_SIZE = 256;
const size_t LINE_BUFFER_SIZE = 1024;

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
        if (c == '\n') {
            break;
        }
        
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
    
    char buffer[LINE_BUFFER_SIZE];
    size_t capacity = INITIAL_STRING_ARRAY_CAPACITY;
    array->strings = malloc(capacity * sizeof(char*));
    
    if (array->strings == NULL) {
        printf("Ошибка выделения памяти\n");
        free(array);
        fclose(file);
        return NULL;
    }
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strlen(buffer) == 0) continue;
        
        if (array->count >= capacity) {
            capacity *= 2;
            char** new_strings = realloc(array->strings, capacity * sizeof(char*));
            if (new_strings == NULL) {
                printf("Ошибка перевыделения памяти\n");
                break;
            }
            array->strings = new_strings;
        }
        
        array->strings[array->count] = malloc(strlen(buffer) + 1);
        if (array->strings[array->count] != NULL) {
            strcpy(array->strings[array->count], buffer);
            array->count++;
        }
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
    
    for (size_t i = 0; i < array->count; i++) {
        free(array->strings[i]);
    }
    free(array->strings);
    free(array);
}

void display_strings(const StringArray* array) {
    if (array == NULL || array->count == 0) {
        printf("Нет строк для отображения\n");
        return;
    }
    
    printf("\n=== СТРОКИ ИЗ ФАЙЛА ===\n");
    for (size_t i = 0; i < array->count; i++) {
        printf("%zu. %s\n", i + 1, array->strings[i]);
    }
    printf("========================\n");
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
    
    char* selected = malloc(strlen(array->strings[choice - 1]) + 1);
    if (selected != NULL) {
        strcpy(selected, array->strings[choice - 1]);
    }
    
    return selected;
}

int input_from_file() {
    char filename[FILENAME_BUFFER_SIZE];
    StringArray* array = NULL;
    char* selected_string = NULL;
    int choice;
    
    printf("\n=== ВВОД СТРОКИ ИЗ ФАЙЛА ===\n");
    printf("Введите имя файла (по умолчанию 'input.txt'): ");
    
    if (fgets(filename, sizeof(filename), stdin) != NULL) {
        filename[strcspn(filename, "\n")] = 0;
        if (strlen(filename) == 0) {
            strcpy(filename, "input.txt");
        }
    }
    
    array = read_strings_from_file(filename);
    if (array == NULL) {
        return 0;
    }
    
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
            case 1:
                input_char_string();
                break;
            case 2:
                input_char16_string();
                break;
            case 3:
                input_from_file();
                break;
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