#include "../headers/Set.h"

void SETINIT(setData* set) {
    set->capacity = 16;
    set->count = 0;
    set->buckets = new FNode*[set->capacity];
    for(int i = 0; i < set->capacity; i++) {
        set->buckets[i] = nullptr;
    }
}

unsigned int HASH(const string& str){
    unsigned int hash = 0;
    for(char symbol : str) {
        hash = hash * 31 + static_cast<unsigned char>(symbol);
    }
    return hash;
}

bool SET_AT(setData* set, const string& str) {
    unsigned int hash = HASH(str);
    int index = hash % set->capacity;
    FNode* found = FGET(set->buckets[index], str);
    return found != nullptr;
}

void REHASH(setData* set)
{
    int newCapacity = set->capacity * 2;
    FNode** newBuckets = new FNode*[newCapacity];

    for (int i = 0; i < newCapacity; i++)
    {
        newBuckets[i] = nullptr;
    }

    for (int i = 0; i < set->capacity; i++)
    {
        FNode* current = set->buckets[i];

        while (current != nullptr)
        {
            FNode* next = current->nextEl;
            int newIndex = HASH(current->data) % newCapacity;

            current->nextEl = newBuckets[newIndex];
            newBuckets[newIndex] = current;

            current = next;
        }
    }

    delete[] set->buckets;
    set->buckets = newBuckets;
    set->capacity = newCapacity;
}

bool SETADD(setData* set, const string& str) {
    int index = HASH(str) % set->capacity;

    if(FGET(set->buckets[index], str) != nullptr) return false;

    if((set->count + 1) * 4 > set->capacity * 3) {
        REHASH(set);
        index = HASH(str) % set->capacity;
    }

    FPUSHHEAD(set->buckets[index], str);
    set->count++;
    return true;
}

bool SETDEL(setData* set, const string& str) {
    int index = HASH(str) % set->capacity;
    FNode* curr = set->buckets[index];
    FNode* prev = nullptr;

    while(curr != nullptr) {
        if(curr->data == str) {
            if(prev == nullptr) {
                FDELHEAD(set->buckets[index]);
            } else {
                FDELAFTER(prev);
            }
            set->count--;
            return true;
        }
        prev = curr;
        curr = curr->nextEl;
    }
    return false;
}

void SETCLEAR(setData* set) {
    for (int i = 0; i < set->capacity; i++) {
        while(set->buckets[i] != nullptr) {
            FDELHEAD(set->buckets[i]);
        }
    }

    delete[] set->buckets;
    set->buckets = nullptr;
    set->capacity = 0;
    set->count = 0; 
}