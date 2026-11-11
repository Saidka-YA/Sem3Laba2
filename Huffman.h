#pragma once
#include "Array.h"
#include <stdlib.h>

struct Node {
    char symbol;
    int freq;
    Node* left;
    Node* right;
    Node(char symbol, int freq)
        : symbol(symbol), freq(freq), left(nullptr), right(nullptr)
    {} 
    Node(char symbol, int freq, Node* left, Node* right)
        : symbol(symbol), freq(freq), left(left), right(right)
    {}
};

class EncodeHuffman {
private:
    string input;
    Node* root;
    Node* nodes[511];
    int nodeCount;
    string codes[256];
    void buildTree();
    int takeMin(MArray* freeNodes);
    void buildCodes(Node* curr, string path);
    void insertSorted(MArray* array, int newNode);
public:
    EncodeHuffman(string text);
    ~EncodeHuffman();
    void printCodes();

    string encode();
    string decode(string bits);
};

