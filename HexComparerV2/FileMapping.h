#pragma once
#include "stdafx.h"

/// <summary>
/// Содержит всю работу с файлом
/// </summary>
class FileMapping
{
private:
	/// <summary>
	/// Указатель на отображение файла
	/// </summary>
	LPVOID		m_pMapViewOfFile	=	NULL;

	/// <summary>
	/// Дескриптор файла
	/// </summary>
	HANDLE		m_hFile				=	NULL;

	/// <summary>
	/// Дескриптор маппинга
	/// </summary>
	HANDLE		m_hFileMapping		=	NULL;

	/// <summary>
	/// Размер файла
	/// </summary>
	INT64		m_SizeOfFile		=	-1;

public:

	/// <summary>
	/// Открывает файл
	/// </summary>
	/// <param name="fileName">Путь к файлу </param>
	/// <returns></returns>
	BOOL		OpenFile(LPCWSTR fileName);

	/// <summary>
	/// Получение байта по его номеру
	/// </summary>
	/// <param name="numberOfByte">Номер байта</param>
	/// <param name="Byte">Переменная, в которую записывается значение байта</param>
	/// <returns>Успешность доступа</returns>
	BOOL		getByte(INT64 numberOfByte, OUT BYTE & Byte);

	/// <summary>
	/// Возвращает размер файла
	/// </summary>
	/// <returns>Размер файла</returns>
	INT64		getSizeOfFile();

	/// <summary>
	/// Закрывает файл и все его дескрипторы
	/// </summary>
	void		CloseFile();

	/// <summary>
	/// Закрывает файл
	/// </summary>
				~FileMapping();
};

