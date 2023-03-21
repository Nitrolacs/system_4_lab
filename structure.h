/*!
 * \file structure.h
 * \brief Заголовочный файл с описанием структуры
 *
 * Данный файл содержит в себе определение основных
 * структур, используемых в программе.
*/

#ifndef INC_4_LAB_STRUCTURE_H
#define INC_4_LAB_STRUCTURE_H

#define MAX_LEN 100 // фиксированная длина записей

typedef struct Enterprise Enterprise; //!< Структура предприятия

/*!
 * \brief Предприятие-изготовитель
 */
struct Enterprise
{
    char company[MAX_LEN]; /*!< Предприятие изготовитель */
    char type[MAX_LEN]; /*!< Тип */
    double price; /*!< Цена */
    double performance; /*!< Производительность */
};

#endif //INC_4_LAB_STRUCTURE_H
