/*! Код основного процесса программы */

#include <stdio.h>

#include "interface.h"
#include "functions.h"

#define MAX_LEN 100
#define ADDING_ENTERPRISE 1
#define CHANGING_ENTERPRISE 2
#define DELETING_ENTERPRISE 3
#define PRINTING_ONE_ENTERPRISE 4
#define PRINTING_ALL_ENTERPRISES 5
#define ENTERPRISES_WITH_BEST_VALUE 6
#define PRICE_RANGE 7
#define EXIT 8

int main()
{
    printf("----------------------------------\n");
    printf("| Программа для работы с файлами |\n");
    printf("----------------------------------\n");

    int userChoice;

    do
    {
        PrintMenu(); // выводим меню на экран
        int menuItemsCount = 8; // кол-во пунктов меню
        // нижняя граница допустимых значений
        int lowerBound = 1;
        // получаем номер выбранного пункта меню
        printf("Введите номер пункта: ");
        userChoice = CheckingInput(lowerBound, menuItemsCount);

        switch (userChoice)
        {
            case ADDING_ENTERPRISE:
                AddRecord();
                break;

            case CHANGING_ENTERPRISE:
                ModifyRecord();
                break;

            case DELETING_ENTERPRISE:
                DeleteRecord();
                break;

            case PRINTING_ONE_ENTERPRISE:
                ReadRecord();
                break;

            case PRINTING_ALL_ENTERPRISES:
                PrintAllRecords();
                break;

            case ENTERPRISES_WITH_BEST_VALUE:
                FindBestRatio();
                break;

            case PRICE_RANGE:
                FindPriceRange();
                break;

            default:
                printf("Выход...\n");
        }
    } while (userChoice != EXIT);

    return 0;
}
