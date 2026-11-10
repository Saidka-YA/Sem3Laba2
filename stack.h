#ifndef STACK_H
#define STACK_H
#pragma once
#include <iostream>
#include <string>
using namespace std;

struct StackNode
{
	string data;
	StackNode* nextEl;
};

struct StackData
{
	StackNode* head;
};

void SINIT(StackData* stack);
bool SISEMPTY(StackData* stack);
void SPUSH(StackData* stack, const string& str);
string SPOP(StackData* stack);
void SPRINT(StackData* stack);
string SGET(StackData* stack);
void SCLEAR(StackData* stack);

#endif