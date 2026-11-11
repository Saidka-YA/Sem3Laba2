#include "../headers/EmployeeCounter.h"

EmployeeCounter::EmployeeCounter(HashMethod method) : counted(false)
{
    HTINIT(&managers, method);
    HTINIT(&counts, method);
    MINIT(&employees);
}

EmployeeCounter::~EmployeeCounter()
{
    HTCLEAR(&managers);
    HTCLEAR(&counts);
    MCLEAR(&employees);
}

void EmployeeCounter::addEmployee(const string& name)
{
    if (HT_AT(&counts, name)) return;

    MPUSH(&employees, name);
    HTADD(&counts, name, "0");
}

void EmployeeCounter::addPair(const string& employee, const string& manager)
{
    addEmployee(employee);
    addEmployee(manager);
    HTADD(&managers, employee, manager);
    counted = false;
}

void EmployeeCounter::calculate()
{
    for (int i = 0; i < MLENGTH(&employees); i++)
    {
        HTADD(&counts, MGET(&employees, i), "0");
    }

    for (int i = 0; i < MLENGTH(&employees); i++)
    {
        string current = MGET(&employees, i);
        string manager;

        while (HTGET(&managers, current, &manager))
        {
            if (manager == current) break;

            string count;
            HTGET(&counts, manager, &count);
            HTADD(&counts, manager, to_string(stoi(count) + 1));
            current = manager;
        }
    }

    counted = true;
}

int EmployeeCounter::getCount(const string& manager)
{
    if (!counted) calculate();

    string count;
    if (!HTGET(&counts, manager, &count)) return -1;
    return stoi(count);
}

void EmployeeCounter::printCounts()
{
    if (!counted) calculate();

    for (int i = 0; i < MLENGTH(&employees); i++)
    {
        string employee = MGET(&employees, i);
        cout << employee << '-' << getCount(employee);
        if (i + 1 < MLENGTH(&employees)) cout << ", ";
    }
    cout << '\n';
}
