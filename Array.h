#ifndef ARRAY_H
#define ARRAY_H
#pragma once
#include <iostream>
#include <string>

using namespace std;

struct MArray
{
	string* data;
	int size;
	int capacity;
};

void MINIT(MArray* arr);
void MRESIZE(MArray* arr);
void MPUSH(MArray* arr, const string& value);
void MINSERT(MArray* arr, int index, const string& value);
string MGET(MArray* arr, int index);
void MREPLACE(MArray* arr, int index, const string& value);
void MDEL(MArray* arr, int index);
int MLENGTH(MArray* arr);
void MPRINT(MArray* arr);
void MCLEAR(MArray* arr);

#endif