#include "../headers/interface.h"

#include "../headers/EmployeeCounter.h"
#include "../headers/Huffman.h"
#include "../headers/LRUCache.h"
#include "../headers/Set.h"
#include "../headers/StressChecker.h"
#include "../headers/postfixConverter.h"
#include "../algorithms/genome.cpp"
#include "../../Sem3Laba1/json_file.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

enum Task {
    POSTFIX_TASK,
    GENOM_TASK,
    STRESS_TASK,
    HUFFMAN_TASK,
    MANAGER_TASK,
    LRU_TASK,
    UNKNOWN_TASK
};

static Task GetTask(const string& name)
{
    if (name == "POSTFIX") return POSTFIX_TASK;
    if (name == "GENOM") return GENOM_TASK;
    if (name == "STRESS") return STRESS_TASK;
    if (name == "HUFFMAN") return HUFFMAN_TASK;
    if (name == "MANAGER") return MANAGER_TASK;
    if (name == "LRU") return LRU_TASK;
    return UNKNOWN_TASK;
}

static int RunPostfix()
{
    string expression;
    cout << "-> ";
    if (!getline(cin, expression)) return 1;

    postfixConverter converter;
    string postfix = converter.convert(expression);

    try
    {
        cout << "-> " << postfix << " = " << converter.evaluate(postfix) << '\n';
    }
    catch (const exception&)
    {
        cerr << "Ошибка в выражении.\n";
        return 1;
    }
    return 0;
}

static int RunGenom()
{
    string first;
    string second;
    cerr << "-> ";
    if (!(cin >> first)) return 1;
    cerr << "-> ";
    if (!(cin >> second)) return 1;

    GenomeComparator comparator(first, second);
    cout << comparator.closeness() << '\n';
    return 0;
}

static int RunStress()
{
    int count;
    cerr << "-> ";
    if (!(cin >> count) || count < 0) return 1;

    StressChecker checker;
    for (int i = 0; i < count; i++)
    {
        string word;
        cerr << "-> ";
        if (!(cin >> word)) return 1;
        checker.addDictionaryWord(word);
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string text;
    cerr << "-> ";
    if (!getline(cin, text)) return 1;
    cout << checker.countErrors(text) << '\n';
    return 0;
}

static int RunHuffman()
{
    string text;
    cerr << "-> ";
    if (!getline(cin, text)) return 1;

    EncodeHuffman huffman(text);
    cout << huffman.encode() << '\n';
    return 0;
}

static int RunManager()
{
    int count;
    cerr << "-> ";
    if (!(cin >> count) || count < 0) return 1;

    EmployeeCounter counter;
    for (int i = 0; i < count; i++)
    {
        string employee;
        string manager;
        cerr << "-> ";
        if (!(cin >> employee >> manager)) return 1;
        counter.addPair(employee, manager);
    }
    counter.printCounts();
    return 0;
}

static int RunLRU()
{
    int capacity;
    int count;
    cerr << "-> ";
    if (!(cin >> capacity >> count) || capacity < 0 || count < 0) return 1;

    LRUCache cache(capacity);
    for (int i = 0; i < count; i++)
    {
        string command;
        int key;
        cerr << "-> ";
        if (!(cin >> command >> key)) return 1;

        if (command == "SET")
        {
            int value;
            if (!(cin >> value)) return 1;
            cache.SET(key, value);
        }
        else if (command == "GET")
        {
            cout << cache.GET(key) << '\n';
        }
        else return 1;
    }
    return 0;
}

static void SkipSpaces(const string& text, size_t& position)
{
    while (position < text.size() &&
        (text[position] == ' ' || text[position] == '\n' ||
         text[position] == '\r' || text[position] == '\t')) position++;
}

static bool ReadSet(setData* set, const string& fileName, string& error)
{
    string json;
    if (!ensureJsonFile(fileName, error) || !readJsonFile(fileName, json, error)) return false;

    size_t position = 0;
    SkipSpaces(json, position);
    if (position >= json.size() || json[position++] != '[')
    {
        error = "Ожидался JSON-массив строк.";
        return false;
    }

    SkipSpaces(json, position);
    while (position < json.size() && json[position] != ']')
    {
        if (json[position++] != '"')
        {
            error = "Ожидалась строка в JSON-массиве.";
            return false;
        }

        string value;
        bool closed = false;
        while (position < json.size())
        {
            char symbol = json[position++];
            if (symbol == '"') { closed = true; break; }
            if (symbol == '\\')
            {
                if (position >= json.size()) break;
                symbol = json[position++];
                if (symbol == 'n') symbol = '\n';
                else if (symbol == 'r') symbol = '\r';
                else if (symbol == 't') symbol = '\t';
                else if (symbol != '"' && symbol != '\\' && symbol != '/') break;
            }
            value += symbol;
        }
        if (!closed)
        {
            error = "Незакрытая строка в JSON.";
            return false;
        }
        SETADD(set, value);

        SkipSpaces(json, position);
        if (position < json.size() && json[position] == ',')
        {
            position++;
            SkipSpaces(json, position);
        }
        else if (position >= json.size() || json[position] != ']')
        {
            error = "Ошибка между элементами JSON-массива.";
            return false;
        }
    }

    if (position >= json.size() || json[position++] != ']')
    {
        error = "Не закрыт JSON-массив.";
        return false;
    }
    SkipSpaces(json, position);
    if (position != json.size())
    {
        error = "Лишние данные после JSON-массива.";
        return false;
    }
    return true;
}

static void WriteString(ostringstream& json, const string& value)
{
    json << '"';
    for (char symbol : value)
    {
        if (symbol == '"' || symbol == '\\') json << '\\' << symbol;
        else if (symbol == '\n') json << "\\n";
        else if (symbol == '\r') json << "\\r";
        else if (symbol == '\t') json << "\\t";
        else json << symbol;
    }
    json << '"';
}

static bool SaveSet(setData* set, const string& fileName, string& error)
{
    ostringstream json;
    bool first = true;
    json << '[';
    for (int i = 0; i < set->capacity; i++)
    {
        for (FNode* node = set->buckets[i]; node != nullptr; node = node->nextEl)
        {
            if (!first) json << ',';
            WriteString(json, node->data);
            first = false;
        }
    }
    json << ']';
    return overwriteJsonFile(fileName, json.str(), error);
}

static int RunFile(int argc, char* argv[])
{
    if (argc < 5 || string(argv[1]) != "--file") return 1;

    string fileName = argv[2];
    if (fileName.size() < 5 || fileName.substr(fileName.size() - 5) != ".json")
    {
        cerr << "Нужен файл с расширением .json.\n";
        return 1;
    }
    for (int i = 3; i < argc; i += 2)
    {
        if (i + 1 >= argc || string(argv[i]) != "--query") return 1;
    }

    setData set;
    SETINIT(&set);
    string error;
    if (!ReadSet(&set, fileName, error))
    {
        cerr << "Ошибка: " << error << '\n';
        SETCLEAR(&set);
        return 1;
    }

    for (int i = 4; i < argc; i += 2)
    {
        istringstream query(argv[i]);
        string command;
        string value;
        query >> command;
        getline(query >> ws, value);
        if (value.empty())
        {
            cerr << "Не указан элемент множества.\n";
            SETCLEAR(&set);
            return 1;
        }

        bool changed = false;
        if (command == "SETADD") changed = SETADD(&set, value);
        else if (command == "SETDEL") changed = SETDEL(&set, value);
        else if (command == "SET_AT") SET_AT(&set, value);
        else
        {
            cerr << "Неизвестный запрос к множеству.\n";
            SETCLEAR(&set);
            return 1;
        }

        if (changed && !SaveSet(&set, fileName, error))
        {
            cerr << "Ошибка: " << error << '\n';
            SETCLEAR(&set);
            return 1;
        }
    }

    SETCLEAR(&set);
    return 0;
}

int RunInterface(int argc, char* argv[])
{
    if (argc > 1 && string(argv[1]) == "--file") return RunFile(argc, argv);
    if (argc != 2)
    {
        cerr << "Использование: ./LABA2 POSTFIX|GENOM|STRESS|HUFFMAN|MANAGER|LRU\n";
        return 1;
    }

    Task task = GetTask(argv[1]);
    if (task == POSTFIX_TASK) return RunPostfix();
    if (task == GENOM_TASK) return RunGenom();
    if (task == STRESS_TASK) return RunStress();
    if (task == HUFFMAN_TASK) return RunHuffman();
    if (task == MANAGER_TASK) return RunManager();
    if (task == LRU_TASK) return RunLRU();

    cerr << "Неизвестная команда.\n";
    return 1;
}
