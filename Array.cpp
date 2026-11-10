#include "Array.h"

void MINIT(MArray* arr)
{
	arr->size = 0;
	arr->capacity = 4;
	arr->data = new string[arr->capacity];
}

void MRESIZE(MArray* arr)
{
	int newCapacity = arr->capacity * 2;
	string* newData = new string[newCapacity];
	for (int i = 0; i < arr->size; i++)
	{
		newData[i] = arr->data[i];
	}
	delete[] arr->data;
	arr->data = newData;
	arr->capacity = newCapacity;
}

void MPUSH(MArray* arr, const string& value)
{
	if (arr->size == arr->capacity)
	{
		MRESIZE(arr);
	}
	arr->data[arr->size] = value;
	arr->size++;
}

void MINSERT(MArray* arr, int index, const string& value)
{
	if (index < 0 || index > arr->size)
	{
		cout << "Неверный индекс!\n";
		return;
	}
	if (arr->size == arr->capacity)
	{
		MRESIZE(arr);
	}
	for (int i = arr->size; i > index; i--)
	{
		arr->data[i] = arr->data[i - 1];
	}
	arr->data[index] = value;
	arr->size++;
}

string MGET(MArray* arr, int index)
{
	if (index < 0 || index >= arr->size)
	{
		cout << "Неверный индекс!\n";
		return "";
	}
	return arr->data[index];
}

void MREPLACE(MArray* arr, int index, const string& value)
{
	if (index < 0 || index >= arr->size)
	{
		cout << "Неверный индекс!\n";
		return;
	}
	arr->data[index] = value;
}

void MDEL(MArray* arr, int index)
{
	if (index < 0 || index >= arr->size)
	{
		cout << "Неверный индекс!\n";
		return;
	}
	for (int i = index; i < arr->size - 1; i++)
	{
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
}

int MLENGTH(MArray* arr)
{
	return arr->size;
}

void MPRINT(MArray* arr)
{
	if (arr->size == 0)
	{
		cout << "Массив пуст выводить нечего!\n";
		return;
	}
	for (int i = 0; i < arr->size; i++)
	{
		cout << arr->data[i] << ' ';
	}
	cout << endl;
}

void MCLEAR(MArray* arr)
{
	delete[] arr->data;
	arr->data = nullptr;
	arr->size = 0;
	arr->capacity = 0;
}
