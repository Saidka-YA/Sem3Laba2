#pragma once
#include "Array.h"
#include "HashTable.h"

class EmployeeCounter {
private:
    HashTable managers;
    HashTable counts;
    MArray employees;
    bool counted;

    void addEmployee(const string& name);
    void calculate();

public:
    EmployeeCounter(HashMethod method = LINEAR_PROBING);
    ~EmployeeCounter();

    EmployeeCounter(const EmployeeCounter&) = delete;
    EmployeeCounter& operator=(const EmployeeCounter&) = delete;

    void addPair(const string& employee, const string& manager);
    int getCount(const string& manager);
    void printCounts();
};
