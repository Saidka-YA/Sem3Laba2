#include "../headers/StressChecker.h"

#include <algorithm>
#include <sstream>

StressChecker::StressChecker() : sorted(true)
{
    MINIT(&dictionary);
}

StressChecker::~StressChecker()
{
    MCLEAR(&dictionary);
}

string StressChecker::normalize(const string& word)
{
    string result = word;
    for (char& symbol : result)
    {
        if (symbol >= 'A' && symbol <= 'Z')
        {
            symbol = static_cast<char>(symbol - 'A' + 'a');
        }
    }
    return result;
}

int StressChecker::countStresses(const string& word)
{
    int count = 0;
    for (char symbol : word)
    {
        if (symbol >= 'A' && symbol <= 'Z')
        {
            count++;
        }
    }
    return count;
}

bool StressChecker::dictionaryLess(const string& left, const string& right)
{
    string leftNormal = normalize(left);
    string rightNormal = normalize(right);

    if (leftNormal != rightNormal)
    {
        return leftNormal < rightNormal;
    }
    return left < right;
}

void StressChecker::sortDictionary()
{
    if (!sorted)
    {
        sort(dictionary.data, dictionary.data + MLENGTH(&dictionary), dictionaryLess);
        sorted = true;
    }
}

void StressChecker::addDictionaryWord(const string& word)
{
    MPUSH(&dictionary, word);
    sorted = false;
}

bool StressChecker::isError(const string& word)
{
    string normalWord = normalize(word);
    int left = 0;
    int right = MLENGTH(&dictionary);

    while (left < right)
    {
        int middle = left + (right - left) / 2;
        if (normalize(MGET(&dictionary, middle)) < normalWord)
        {
            left = middle + 1;
        }
        else
        {
            right = middle;
        }
    }

    bool foundInDictionary = false;
    for (int i = left; i < MLENGTH(&dictionary); i++)
    {
        string dictionaryWord = MGET(&dictionary, i);
        if (normalize(dictionaryWord) != normalWord)
        {
            break;
        }

        foundInDictionary = true;
        if (dictionaryWord == word)
        {
            return false;
        }
    }

    if (foundInDictionary)
    {
        return true;
    }
    return countStresses(word) != 1;
}

int StressChecker::countErrors(const string& text)
{
    sortDictionary();

    istringstream input(text);
    string word;
    int errors = 0;

    while (input >> word)
    {
        if (isError(word))
        {
            errors++;
        }
    }
    return errors;
}
