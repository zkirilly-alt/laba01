#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "string_poly.h"
#include "test_poly.h"

void allinout();

int main() {
    setlocale(LC_ALL, "");
        
    int choice;
    
    while (1) {
        printf("\n=== ГЛАВНОЕ МЕНЮ ===\n");
        printf("1 - Ручной ввод (сохранение в файл)\n");
        printf("2 - Работа со строками (полиморфная)\n");
        printf("3 - Запустить тесты\n");
        printf("0 - Выход\n");
        printf("Ваш выбор: ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        switch(choice) {
            case 1: allinout(); break;
            case 2: run_string_operations_menu(); break;
            case 3: run_all_tests(); break;
            case 0: 
                printf("Программа завершена. До свидания!\n");
                return 0;
            default:
                printf("Неверный выбор\n");
        }
    }
    
    return 0;
}