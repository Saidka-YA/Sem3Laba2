#pragma once
#include <iostream>
#include <string>
using namespace std;

struct FNode
{
	string data;
	FNode* nextEl;
};

void FPRINT(FNode* element);
void FINSERTAFTER(FNode* element, const string& str);
void FDELAFTER(FNode* element);
void FPUSHHEAD(FNode*& head, const string& str);
void FDELHEAD(FNode*& head);
void FINSERTBEFORE(FNode*& head, FNode* element, const string& str);
void FPUSHTAIL(FNode*& head, const string& str);
void FDELBEFORE(FNode*& head, FNode* element);
void FDELTAIL(FNode*& head);
FNode* FGET(FNode* head, const string& str);
void FDELVALUE(FNode*& head, const string& str);
void FPRINTREVERSE(FNode* head);
