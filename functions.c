#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

#include "functions.h"
#include "structure.h"

#define CORRECT_INPUT 2
#define END_STRING '\n'
#define BACKSPACE_KEY 8
#define START_CHAR_RANGE 32
#define END_CHAR_RANGE 126
#define MAX_LEN 100 // фиксированная длина записей
#define FILE_NAME "data.bin" // имя файла с данными

int CheckingInput(int lowerBound, int count)
{
    int userInput = 1;
    char inputChar = '\0';

    int input = scanf("%d%c", &userInput, &inputChar);

    if (count)
    {
        while (input != CORRECT_INPUT || inputChar != END_STRING
               || userInput < lowerBound || userInput > count)
        {
            if (userInput >= lowerBound && userInput <= count)
            {
                while ((inputChar = getchar()) != '\n');
            }
            userInput = 1;
            printf("Неверный ввод. Попробуйте снова.\nВведите номер: ");
            input = scanf("%d%c", &userInput, &inputChar);
        }
    }
    else
    {
        while (input != CORRECT_INPUT || inputChar != END_STRING
               || userInput <= lowerBound)
        {
            if (userInput > lowerBound)
            {
                while ((inputChar = getchar()) != '\n');
            }
            userInput = 1;
            printf("Неверный ввод. Попробуйте снова.\nВведите номер: ");
            input = scanf("%d%c", &userInput, &inputChar);
        }
    }
    return userInput;
}

char* StringInput(void)
{
    char* userStr = (char*)malloc(1 * sizeof(char));
    userStr[0] = '\0';
    char curChar = 0;
    int curSize = 1;

    while(curChar != '\n')
    {
        curChar = getchar();

        int deltaVal = 0; // Определяет, на сколько изменится длина массива
        int lengthDif = 0;

        // Если мы стираем символы, а не пишем их,
        if (curChar == BACKSPACE_KEY)
        {
            deltaVal = -1; // то будем уменьшать длину массива
            lengthDif = 1; // и копировать строку до предпоследнего символа
        }

            // Иначе проверяем, входит ли введённый символ в диапазон печатных
        else
        {
            if (curChar >= START_CHAR_RANGE && curChar <= END_CHAR_RANGE)
            {
                deltaVal = 1; // Если да, то будем увеличивать длину на 1
                lengthDif = 2; // Не заполняем последние 2 символа -
                // оставляем место для введённого символа и \0
            }
            else
            {
                continue; // Если это не печатный символ, то пропускаем его
            }
        }

        // Если стирать больше нечего, но пользователь
        // всё равно жмёт Backspace.
        int newSize = curSize + deltaVal;
        if (newSize == 0)
        {
            continue; // то мы переходим на следующую итерацию - ждём '\n'
        }

        char* tmpStr = (char*)malloc(newSize * sizeof(char));
        if (tmpStr) // Проверяем, выделилась ли память
        {
            // Идём до предпоследнего символа, т.к. надо в конец записать '\0'
            for (int i = 0;
                 i < newSize - lengthDif;
                 ++i)
            {
                tmpStr[i] = userStr[i];
            }

            if (curChar != BACKSPACE_KEY) // Если введён печатный символ,
            {
                tmpStr[newSize - 2] = curChar; // Добавляем его в строку
                tmpStr[newSize - 1] = '\0';
            }
            free(userStr);
            userStr = tmpStr;
            curSize = newSize;
        }
        else
        {
            printf("Не могу выделить память под обновлённую строку!");
            break;
        }
    }

    return userStr;
}

float FloatInput(void)
{
    float number = 0;
    while (number == 0 || number < 0)
    {
        char* InputValue = StringInput();
        number = strtof(InputValue, NULL);

        if (number == 0 || number < 0)
        {
            printf("Неверный ввод. Попробуйте снова.\n"
                   "Повторите ввод: ");
        }
        free(InputValue);
    }
    return number;
}

// функция для добавления одной записи в файл
void add_record() {
    Enterprise p; // структура для хранения данных о продукте
    struct iovec iov[4]; // массив структур iovec для векторной записи

    printf("Введите предприятие-изготовитель: ");
    scanf("%s", p.manufacturer);
    printf("Введите тип: ");
    scanf("%s", p.type);
    printf("Введите цену: ");
    scanf("%lf", &p.price);
    printf("Введите производительность: ");
    scanf("%lf", &p.performance);

    iov[0].iov_base = p.manufacturer; // указатель на данные о предприятии-изготовителе
    iov[0].iov_len = MAX_LEN; // размер данных о предприятии-изготовителе в байтах
    iov[1].iov_base = p.type; // указатель на данные о типе
    iov[1].iov_len = MAX_LEN; // размер данных о типе в байтах
    iov[2].iov_base = &p.price; // указатель на данные о цене
    iov[2].iov_len = sizeof(double); // размер данных о цене в байтах
    iov[3].iov_base = &p.performance; // указатель на данные о производительности
    iov[3].iov_len = sizeof(double); // размер данных о производительности в байтах

    int fd = open(FILE_NAME, O_WRONLY | O_APPEND | O_CREAT, 0666); // открыть файл для записи в конец или создать его при необходимости с правами 0666

    if (fd == -1) { // проверить успешность открытия файла
        perror("Ошибка открытия файла");
        exit(1);
    }

    if (writev(fd, iov, 4) == -1) { // выполнить векторную запись из массива структур iovec в файл и проверить успешность операции
        perror("Ошибка записи в файл");
        exit(1);
    }

    close(fd); // закрыть файл

    printf("Запись успешно добавлена.\n");
}

// функция для модификации одной записи в файле по номеру
void modify_record() {
    int n; // номер записи для модификации
    int choice; // выбор пользователя для модификации полей
    Enterprise p; // структура для хранения данных о продукте
    struct iovec iov[4]; // массив структур iovec для векторной записи

    printf("Введите номер записи для модификации: ");
    scanf("%d", &n);

    if (n < 1) { // проверить корректность номера записи
        printf("Неверный номер записи.\n");
        return;
    }

    int fd = open(FILE_NAME, O_RDWR); // открыть файл для чтения и записи

    iov[0].iov_base = p.manufacturer; // указатель на данные о предприятии-изготовителе
    iov[0].iov_len = MAX_LEN; // размер данных о предприятии-изготовителе в байтах
    iov[1].iov_base = p.type; // указатель на данные о типе
    iov[1].iov_len = MAX_LEN; // размер данных о типе в байтах
    iov[2].iov_base = &p.price; // указатель на данные о цене
    iov[2].iov_len = sizeof(double); // размер данных о цене в байтах
    iov[3].iov_base = &p.performance; // указатель на данные о производительности
    iov[3].iov_len = sizeof(double); // размер данных о производительности в байтах

    if (fd == -1) { // проверить успешность открытия файла
        perror("Ошибка открытия файла");
        exit(1);
    }

    if (lseek(fd, (n - 1) * (MAX_LEN * 2 + sizeof(double) * 2), SEEK_SET) == -1) { // переместить указатель позиции в файле на начало нужной записи и проверить успешность операции
        perror("Ошибка перемещения в файле");
        exit(1);
    }

    if (readv(fd, iov, 4) == -1) { // выполнить векторное чтение из файла в массив структур iovec и проверить успешность операции
        perror("Ошибка чтения из файла");
        exit(1);
    }

    close(fd); // закрыть файл

    printf("Выберите поля для модификации:\n");
    printf("1. Предприятие-изготовитель\n");
    printf("2. Тип\n");
    printf("3. Цена\n");
    printf("4. Производительность\n");
    printf("5. Все поля\n");

    scanf("%d", &choice);

    switch(choice) {
        case 1: // модификация поля предприятие-изготовитель
            printf("Введите новое значение предприятие-изготовитель: ");
            scanf("%s", p.manufacturer);
            iov[0].iov_base = p.manufacturer; // указатель на данные о предприятие-изготовителе
            iov[0].iov_len = MAX_LEN; // размер данных о предприяте-изготовителе в байтах
            break;
        case 2: // модификация поля тип
            printf("Введите новое значение тип: ");
            scanf("%s", p.type);
            iov[1].iov_base = p.type; // указатель на данные о типе
            iov[1].iov_len = MAX_LEN; // размер данных о типе в байтах
            break;
        case 3: // модификация поля цена
            printf("Введите новое значение цена: ");
            scanf("%lf", &p.price);
            iov[2].iov_base = &p.price; // указатель на данные о цене
            iov[2].iov_len = sizeof(double); // размер данных о цене в байтах
            break;
        case 4: // модификация поля производительность
            printf("Введите новое значение производительность: ");
            scanf("%lf", &p.performance);
            iov[3].iov_base = &p.performance; // указатель на данные о производительности
            iov[3].iov_len = sizeof(double); // размер данных о производительности в байтах
            break;
        case 5: // модификация всех полей
            printf("Введите новое значение предприятие-изготовитель: ");
            scanf("%s", p.manufacturer);
            printf("Введите новое значение тип: ");
            scanf("%s", p.type);
            printf("Введите новое значение цена: ");
            scanf("%lf", &p.price);
            printf("Введите новое значение производительность: ");
            scanf("%lf", &p.performance);

            iov[0].iov_base = p.manufacturer; // указатель на данные о предприятие-изготовителе
            iov[0].iov_len = MAX_LEN; // размер данных о предприяте-изготовителе в байтах
            iov[1].iov_base = p.type; // указатель на данные о типе
            iov[1].iov_len = MAX_LEN; // размер данных о типе в байтах
            iov[2].iov_base = &p.price; // указатель на данные о цене
            iov[2].iov_len = sizeof(double); // размер данных о цене в байтах
            iov[3].iov_base = &p.performance; // указатель на данные о производительности
            iov[3].iov_len = sizeof(double); // размер данных о производительности в байтах

            break;
        default: // неверный выбор пользователя
            printf("Неверный выбор.\n");
            return;
    }

    fd = open(FILE_NAME, O_WRONLY); // открыть файл для записи

    if (fd == -1) { // проверить успешность открытия файла
        perror("Ошибка открытия файла");
        exit(1);
    }

    if (lseek(fd, (n - 1) * (MAX_LEN * 2 + sizeof(double) * 2), SEEK_SET) == -1) { // переместить указатель позиции в файле на начало нужной записи и проверить успешность операции
        perror("Ошибка перемещения в файле");
        exit(1);
    }

    if (writev(fd, iov, 4) == -1) { // выполнить векторную запись из массива структур iovec в файл и проверить успешность операции
        perror("Ошибка записи в файл");
        exit(1);
    }

    close(fd); // закрыть файл

    printf("Запись успешно модифицирована.\n");
}

// функция для удаления одной записи из файла по номеру
void delete_record() {
    int n; // номер записи для удаления
    int count; // количество записей в файле
    char buffer[MAX_LEN * 2 + sizeof(double) * 2]; // буфер для хранения данных о продукте
    struct iovec iov[1]; // массив структур iovec для векторной чтения/записи
    printf("Введите номер записи для удаления: ");
    scanf("%d", &n);
    if (n < 1) { // проверить корректность номера записи
        printf("Неверный номер записи.\n");
        return;
    }
    int fd = open(FILE_NAME, O_RDWR); // открыть файл для чтения и записи
    if (fd == -1) { // проверить успешность открытия файла
        perror("Ошибка открытия файла");
        exit(1);
    }

    count = lseek(fd,0,SEEK_END)/(MAX_LEN *2 + sizeof(double)*2); // определить количество записей в файле

    if (n > count) { // проверить корректность номера записи
        printf("Неверный номер записи.\n");
        return;
    }

    for(int i=n;i<count;i++) { // цикл по всем последующим записям

        if (lseek(fd,i*(MAX_LEN*2+sizeof(double)*2),SEEK_SET)==-1){ // переместить указатель позиции в файле на начало текущей записи и проверить успешность операции
            perror("Ошибка перемещения в файле");
            exit(1);
        }

        iov[0].iov_base = buffer; // указатель на данные в буфере
        iov[0].iov_len = MAX_LEN*2+sizeof(double)*2; // размер данных в буфере в байтах

        if(readv(fd,iov,1)==-1){ // выполнить векторное чтение из файла в массив структур iovec и проверить успешность операции
            perror("Ошибка чтения из файла");
            exit(1);
        }

        if(lseek(fd,(i-1)*(MAX_LEN*2+sizeof(double)*2),SEEK_SET)==-1){ // переместить указатель позиции в файле на начало предыдущей позиции и проверить успешность операции
            perror("Ошибка перемещения в файле");
            exit(1);
        }

        if(writev(fd,iov,1)==-1){ // выполнить векторную запись из массива структур iovec в файл и проверить успешность операции
            perror("Ошибка записи в файл");
            exit(1);
        }
    }

    if(ftruncate(fd,(count-1)*(MAX_LEN*2+sizeof(double)*2))==-1){ // уменьшить размер файла на одну запись и проверить успешность операции
        perror("Ошибка изменения размера файла");
        exit(1);
    }

    close(fd); // закрыть файл

    printf("Запись успешно удалена.\n");
}

// функция для чтения одной записи из файла по номеру
void read_record() {
    int n; // номер записи для чтения
    Enterprise p; // структура для хранения данных о продукте
    struct iovec iov[4]; // массив структур iovec для векторного чтения

    printf("Введите номер записи для чтения: ");
    scanf("%d", &n);

    if (n < 1) { // проверить корректность номера записи
        printf("Неверный номер записи.\n");
        return;
    }

    int fd = open(FILE_NAME, O_RDONLY); // открыть файл для чтения

    if (fd == -1) { // проверить успешность открытия файла
        perror("Ошибка открытия файла");
        exit(1);
    }

    if (lseek(fd, (n - 1) * (MAX_LEN * 2 + sizeof(double) * 2), SEEK_SET) == -1) { // переместить указатель позиции в файле на начало нужной записи и проверить успешность операции
        perror("Ошибка перемещения в файле");
        exit(1);
    }

    iov[0].iov_base = p.manufacturer; // указатель на данные о предприятие-изготовителе
    iov[0].iov_len = MAX_LEN; // размер данных о предприяте-изготовителе в байтах
    iov[1].iov_base = p.type; // указатель на данные о типе
    iov[1].iov_len = MAX_LEN; // размер данных о типе в байтах
    iov[2].iov_base = &p.price; // указатель на данные о цене
    iov[2].iov_len = sizeof(double); // размер данных о цене в байтах
    iov[3].iov_base = &p.performance; // указатель на данные о производительности
    iov[3].iov_len = sizeof(double); // размер данных о производительности в байтах

    if (readv(fd, iov, 4) == -1) { // выполнить векторное чтение из файла в массив структур iovec и проверить успешность операции
        perror("Ошибка чтения из файла");
        exit(1);
    }

    close(fd); // закрыть файл

    printf("Предприятие-изготовитель: %s\n", p.manufacturer);
    printf("Тип: %s\n", p.type);
    printf("Цена: %.2f\n", p.price);
    printf("Производительность: %.2f\n", p.performance);
}

// функция для вывода всех записей из файла
void print_all_records() {
    Enterprise p; // структура для хранения данных о продукте
    struct iovec iov[4]; // массив структур iovec для векторного чтения
    int count = 0; // счетчик записей

    int fd = open(FILE_NAME, O_RDONLY); // открыть файл для чтения

    if (fd == -1) { // проверить успешность открытия файла
        perror("Ошибка открытия файла");
        exit(1);
    }

    while (1) { // бесконечный цикл

        iov[0].iov_base = p.manufacturer; // указатель на данные о предприятие-изготовителе
        iov[0].iov_len = MAX_LEN; // размер данных о предприяте-изготовителе в байтах
        iov[1].iov_base = p.type; // указатель на данные о типе
        iov[1].iov_len = MAX_LEN; // размер данных о типе в байтах
        iov[2].iov_base = &p.price; // указатель на данные о цене
        iov[2].iov_len = sizeof(double); // размер данных о цене в байтах
        iov[3].iov_base = &p.performance; // указатель на данные о производительности
        iov[3].iov_len = sizeof(double); // размер данных о производительности в байтах

        ssize_t bytes_read = readv(fd, iov, 4); // выполнить векторное чтение из файла в массив структур iovec и сохранить количество прочитанных байтов

        if (bytes_read == -1) { // проверить успешность операции чтения
            perror("Ошибка чтения из файла");
            exit(1);
        }

        if (bytes_read == 0) { // проверить конец файла
            break;
        }

        count++; // увеличить счетчик записей
        printf("Запись %d:\n", count);
        printf("Предприятие-изготовитель: %s\n", p.manufacturer);
        printf("Тип: %s\n", p.type);
        printf("Цена: %.2f\n", p.price);
        printf("Производительность: %.2f\n", p.performance);

    }

    close(fd); // закрыть файл

    if (count == 0) { // если счетчик равен нулю, то сообщить об отсутствии записей
        printf("Файл пуст.\n");
    }
}

