/*!
 * \file functions.h
 * \brief Заголовочный файл с описанием функций
 *
 * Данный файл содержит в себе определения основных
 * функций, используемых в программе.
*/

#ifndef INC_4_LAB_FUNCTIONS_H
#define INC_4_LAB_FUNCTIONS_H

/*!
 * \brief Считывает из консоли одно целое число
 * \param[in] lowerBound Нижняя граница чисел
 * \param[in] count Верхняя граница чисел
 * \return Считанное число
 */
int CheckingInput(int lowerBound, int count);

/*!
 * \brief Функция ввода строки
 * \return Введённая пользователем строка
 */
char* StringInput(void);

/*!
 * \brief Функция ввода числа с плавающей точкой
 * \return Введённое пользователем число
 */
float FloatInput(void);

/*!
 * \brief Функция для добавления одной записи в файл
 */
void AddRecord(void);

/*!
 * \brief Функция для модификации одной записи в файле по номеру
 */
void ModifyRecord();

/*!
 * \brief Функция подсчёта количества записей в файле
 * \return Количество записей
 */
long CountRecords(char* fileName);

/*!
 * \brief Функция для удаления одной записи из файла по номеру
 */
void DeleteRecord();

/*!
 * \brief Функция для чтения одной записи из файла по номеру
 */
void ReadRecord();

/*!
 * \brief Функция для вывода всех записей из файла
 */
void PrintAllRecords();

/*!
 * \brief Функция для нахождения списка предприятий с наилучшим соотношением
 */
void FindBestRatio();

/*!
 * \brief Функция для нахождения диапазона цен
 */
void FindPriceRange();

#endif //INC_4_LAB_FUNCTIONS_H
