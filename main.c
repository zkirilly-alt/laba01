#include <stdio.h>
#include <stdlib.h>
#include "inout.h"
#include "string_poly.h"

int main() {
    int choice;
    while (1) {
        printf("\nГЛАВНОЕ МЕНЮ\n");
        printf("1 - Ручной ввод (сохранение в файл)\n");
        printf("2 - Работа со строками (полиморфная)\n");
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
            case 0: printf("Программа завершена. До свидания!\n"); return 0;
            default: printf("Неверный выбор\n");
        }
    }
    
    return 0;
}