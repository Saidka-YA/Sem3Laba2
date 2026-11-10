#pragma once
#include "Array.h"
#include <string>

class StressChecker
{
private:
    MArray dictionary;
    bool sorted;

    static string normalize(const string& word);
    static int countStresses(const string& word);
    static bool dictionaryLess(const string& left, const string& right);
    void sortDictionary();
    bool isError(const string& word);

public:
    StressChecker();
    ~StressChecker();

    StressChecker(const StressChecker&) = delete;
    StressChecker& operator=(const StressChecker&) = delete;

    void addDictionaryWord(const string& word);
    int countErrors(const string& text);
};
