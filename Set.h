#pragma once
#include "linear_list.h"

struct setData {
    FNode** buckets;
    int capacity;
    int count;
};

void SETINIT(setData* set);
bool SETADD(setData* set, const string& str);
bool SETDEL(setData* set, const string& str);
bool SET_AT(setData* set, const string& str);
void SETCLEAR(setData* set);