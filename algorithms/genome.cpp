#include "../headers/Set.h"

using namespace std;

class GenomeComparator
{
private:
    string firstGenome;
    string secondGenome;

public:
    GenomeComparator(const string& first, const string& second)
    {
        firstGenome = first;
        secondGenome = second;
    }

    int closeness()
    {
        setData pairs;
        SETINIT(&pairs);

        for (size_t i = 0; i + 1 < secondGenome.length(); i++)
        {
            string pair = secondGenome.substr(i, 2);
            SETADD(&pairs, pair);
        }

        int result = 0;

        for (size_t i = 0; i + 1 < firstGenome.length(); i++)
        {
            string pair = firstGenome.substr(i, 2);

            if (SET_AT(&pairs, pair))
            {
                result++;
            }
        }

        SETCLEAR(&pairs);
        return result;
    }
};
