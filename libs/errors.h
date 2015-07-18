/**
 * @file errors.h
 * Тип и общие коды ошибок.
 */

#ifndef ERRORS_H
#define	ERRORS_H

#include <stdint.h>

//! Тип ошибки.
typedef uint8_t err_t;

//! Отсутствие ошибки.
#define E_NO_ERROR 0
//! Нулевой указатель.
#define E_NULL_POINTER 1
//! Неправильное значение.
#define E_INVALID_VALUE 2
//! Выход за допустимые пределы.
#define E_OUT_OF_RANGE 3
//! Занято.
#define E_BUSY 4
//! Ошибка ввода-вывода.
#define E_IO_ERROR 5

//! Начальный код для пользовательских ошибок.
#define E_USER 100

#endif	/* ERRORS_H */

