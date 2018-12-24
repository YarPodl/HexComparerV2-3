#pragma once



#include <windows.h>
#include "resource.h"

// ������������ ����� ����� (��������) � ���������
#define MAX_SIZE_STRING 100

// ������������ ����� ����� (��������) � ���������
#define LENGTH_PATH 1024

// ������ �������
#define MAX_FONT_SIZE_HEIGHT 15

// ��������� ������ ������� � ��� ������
#define FONT_SIZE_RELATION 0.45

// ������ �������
//#define FONT_SIZE_WIDTH 6

// �����
#define FONT_FAMILY L"Lucida Console"
//#define FONT_FAMILY L"Times new Roman Cyr"
//#define FONT_FAMILY L"Courier new"
//#define FONT_FAMILY L"Consolas"
//#define FONT_FAMILY L"Symbol"



// ���������� ����� ����� (� ��������)
#define INTERLACED_SPACE 2;

// ���������� ������, ������������ �� ����� ������
#define LENGTH_OF_BYTE_STRINGS 8

// ���������� ������������ ������
#define COUNT_OF_FILES 3

// ��������� ������ ����
#define HEIGHT_WINDOW 600

#define BACKGROUND_WINDOW (HBRUSH)(COLOR_WINDOW + 1)

// ��������� ������ ����
#define WIDTH_WINDOW 1050

// ����������� ������ ����
#define MIN_HEIGHT_WINDOW 150

// ����������� ������ ����
#define MIN_WIDTH_WINDOW 400

// ������ ����
#define HEIGHT_MENU 20

// ������ ������
#define HEIGHT_BUTTONS 20

// ������ ������
#define WIDTH_BUTTONS 30

// ���� ������������� ������
#define TEXT_COLOR_SELECT RGB(255, 0, 0)

// ������ ������
#define INDENT_OF_TOP 3

// ������� ����� �������� � ������� x (������ � ��������)
#define INDENT1 1	
#define INDENT2 4
#define INDENT3 4
#define INDENT4 2

// ���������� �������� ��� ����������� �����
#define CharsForByte 2

// ������� ����� ������� 2 ������� (� ��������)
#define INDENT_BETWEEN_BYTES2 1

// ������ scrollBar
#define WIDTH_SCROLLBAR 20

// ������� ����� ������� 3 ������� (� ��������)
#define INDENT_BETWEEN_LETTERS3 0

// ��������� ���� ���������
#define TITLE "��������� ������"

// ����� ���� ���������
#define WINDOWCLASS L"HexComparerV2"