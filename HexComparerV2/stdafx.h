#pragma once



#include <windows.h>
#include "resource.h"

// Максимальная длина строк (буфферных) в программе
#define MAX_SIZE_STRING 100

// Максимальная длина строк (буфферных) в программе
#define LENGTH_PATH 1024

// Высота символа
#define FONT_SIZE_HEIGHT 15

// Ширина символа
#define FONT_SIZE_WIDTH 6

// Шрифт
#define FONT_FAMILY L"Lucida Console"
//#define FONT_FAMILY L"Times new Roman Cyr"
//#define FONT_FAMILY L"Courier new"
//#define FONT_FAMILY L"Times new Roman"
//#define FONT_FAMILY L"Consolas"
//#define FONT_FAMILY L"Symbol"

// Расстояние между строк (в пикселях)
#define INTERLACED_SPACE 4;

// Количество байтов, отображаемых на одной строке
#define LENGTH_OF_BYTE_STRINGS 8

// Количество сравниваемых файлов
#define COUNT_OF_FILES 2

// Минимальная высота окна
#define MIN_HEIGHT_WINDOW 450

// Минимальная ширина окна
#define MIN_WIDTH_WINDOW 850

// Высота меню
#define HEIGHT_MENU 30

// Высота кнопок
#define HEIGHT_BUTTONS 20

// Ширина кнопок
#define WIDTH_BUTTONS 30

// Цвет различающихся байтов
#define TEXT_COLOR_SELECT RGB(255, 0, 0)

// Отступ сверху
#define INDENT_OF_TOP 0

// Отступы перед столбцом с номером x (ширина в символах)
#define INDENT1 1	
#define INDENT2 5
#define INDENT3 5

// Количество символов для отображения байта
#define CharsForByte 2

// Отступы между байтами 2 столбца (в символах)
#define INDENT_BETWEEN_BYTES2 2

// Ширина scrollBar
#define WIDTH_SCROLLBAR 20

// Отступы между буквами 3 столбца (в символах)
#define INDENT_BETWEEN_LETTERS3 1

// Заголовок окна программы
#define TITLE L"Сравнение файлов"

// Класс окна программы
#define WINDOWCLASS L"HexComparerV1"