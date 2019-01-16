#pragma once
#include "stdafx.h"
#include "FileMapping.h"

#define DIFFERENCE_NOT_FOUND -1
#define SEARCH_CANCELED -2

/// <summary>
/// Состояния байтов
/// </summary>
enum StateOfByte
{
	FileNtLoaded,		// Файл не загружен
	FileEnded,			// Файл закончился
	ByteEqual,			// Байт равен остальным
	ByteNotEqual		// Байт не равен
};

/// <summary>
/// Координирует работу с файлами и их сравнение
/// </summary>
class FileCommander
{

public:

	/// <summary>
	/// Открывает файл
	/// </summary>
	/// <param name="indexFile">Индекс, для которого необходимо открыть файл</param>
	/// <param name="fileName">Путь к файлу и его имя</param>
	/// <returns>Успешность открытия</returns>
	BOOL			LoadFile(INT indexFile, LPCWSTR fileName);

	/// <summary>
	/// Проверяет загружен ли файл
	/// </summary>
	/// <param name="indexfile">Индекс файла для проверки</param>
	/// <returns>TRUE, если файл загружен</returns>
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
	/// Поиск нового (отделенного совпадениями) различия в файлах с заданной позиции и заданным шагом
	/// </summary>
	/// <param name="beginOfSearch">Позиция начала поиска</param>
	/// <param name="step">Шаг, с которым идет поиск (предполагается 1 или -1)</param>
	/// <param name="cancel">Если равен false, поиск отменяется</param>
	/// <returns>Позиция найденного различия, либо -1 если различий не найдено, либо -2 если остановлен</returns>
	INT64			FindDifference(INT64 beginOfSearch, INT step, BOOL & cancel);

	/// <summary>
	/// Возвращает размер файла
	/// </summary>
	/// <returns>Размер файла</returns>
	INT64			GetMaxSize();

private:
	/// <summary>
	/// Массив объектов, работающих с файлами
	/// </summary>
	FileMapping		m_FileMappings[COUNT_OF_FILES];

	/// <summary>
	/// Номер текущего байта
	/// </summary>
	INT64			m_CurrentNumberByte				= -1;

	/// <summary>
	/// Значения текущих байтов для всех файлов
	/// </summary>
	BYTE			m_CurrentBytes[COUNT_OF_FILES]	= { 0 };

	/// <summary>
	/// Загружены ли файлы
	/// </summary>
	BOOL			m_IsLoadedFiles[COUNT_OF_FILES]	= { 0 };

	/// <summary>
	/// Состояния текущих байтов
	/// </summary>
	StateOfByte		m_CurrentStateOfBytes[COUNT_OF_FILES];

	/// <summary>
	/// Равны ли текущие байты
	/// </summary>
	BOOL			m_CurrentEqual					= FALSE;

	/// <summary>
	/// Сравнивает байты, заполняет m_CurrentStateOfBytes и m_CurrentBytes для текущего номера байта
	/// </summary>
	/// <param name="numberOfByte">Номер байта</param>
	/// <returns>Равны ли байты</returns>
	BOOL			Compare(INT64 numberOfByte);

};

