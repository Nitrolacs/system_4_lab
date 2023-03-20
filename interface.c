#include <stdio.h>

#include "interface.h"

// функция для вывода меню выбора действия
void PrintMenu() {
    printf("-------------------\n");
    printf("| Меню программы: |\n");
    printf("-------------------\n");
    printf("1 - Добавить предприятие в файл.\n");
    printf("2 - Модифицировать предприятие в файле.\n");
    printf("3 - Удалить предприятие из файла.\n");
    printf("4 - Вывести предприятие (по индексу).\n");
    printf("5 - Отобразить все предприятия.\n");
    printf("6 - Cписок предприятий с наилучшим соотношением "
           "цена/производительность.\n");
    printf("7 - Диапазон цен.\n");
    printf("8 - Выход.\n");
}