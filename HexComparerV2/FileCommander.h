#pragma once
#include "stdafx.h"
#include "FileMapping.h"

/// <summary>
/// Состояния байтов
/// </summary>
typedef enum StateOfByte
{
	FileNtLoaded,		// Файл не загружен
	FileEnded,			// Файл закончился
	ByteEqual,			// Байт равен остальным
	ByteNotEqual		// Байт не равен
};

class FileCommander
{
private:
	/// <summary>
	/// Массив объектов, работающих с файлами
	/// </summary>
	FileMapping		m_FileMappings[COUNT_OF_FILES];

	/// <summary>
	/// Номер текущего байта
	/// </summary>
	INT64			m_CurrentNumberByte						= -1;
	/// <summary>
	/// Значения текущих байтов для всех файлов
	/// </summary>
	BYTE			m_CurrentBytes[COUNT_OF_FILES]			= { 0 };
	/// <summary>
	/// Загружены ли файлы
	/// </summary>
	BOOL			m_IsLoadedFiles[COUNT_OF_FILES]			= { 0 };
	/// <summary>
	/// Состояния текущих байтов
	/// </summary>
	StateOfByte		m_CurrentStateOfBytes[COUNT_OF_FILES];

	/// <summary>
	/// Сравнивает байты, заполняет m_CurrentStateOfBytes и m_CurrentBytes для текущего номера байта
	/// </summary>
	/// <param name="numberOfByte">Номер байта</param>
	void			Compare(INT64 numberOfByte);
public:

	/// <summary>
	/// Открывает файл
	/// </summary>
	/// <param name="indexFile">Индекс, для которого необходимо открыть файл</param>
	/// <param name="fileName">Путь к файлу и его имя</param>
	/// <returns>Успешность открытия</returns>
	BOOL			LoadFile(INT indexFile, LPCWSTR fileName);

	// <summary>
	// Проверяет загружен ли файл
	// </summary>
	// <param name="indexfile">Индекс файла для проверки</param>
	// <returns>TRUE, если файл загружен</returns>
	BOOL			IsLoadedFile(INT indexfile);

	/// <summary>
	/// Закрывает файл по индексу
	/// </summary>
	/// <param name="indexFile">Индекс закрываемого файла</param>
	void			CloseFile(INT indexFile);

	/// <summary>
	/// Получение значения байта и его состояния
	/// </summary>
	/// <param name="indexFile">Индекс файла, байт которого необходимо получить</param>
	/// <param name="numberOfByte">Номер байта</param>
	/// <param name="b">Переменная, в которую записывается значение байта</param>
	/// <returns>Состояние байта</returns>
	StateOfByte		GetByte(INT indexFile, INT64 numberOfByte, OUT BYTE & b);

	/// <summary>
	/// Возвращает размер файла
	/// </summary>
	/// <returns>Размер файла</returns>
	INT64			GetMaxSize();
};

