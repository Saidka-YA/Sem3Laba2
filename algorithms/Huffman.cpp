#include "../headers/Huffman.h"

void EncodeHuffman::buildTree() {
    int freg[256] {};
    for(char symbol : input) {
        unsigned char ch = static_cast<unsigned char>(symbol);
        freg[ch]++;
    }
    MArray freeNodes;
    MINIT(&freeNodes);
    for (int i = 0; i < 256; i++) {
        if (freg[i] == 0) continue;
        
        nodes[nodeCount] = new Node(static_cast<char>(i), freg[i]);
        insertSorted(&freeNodes, nodeCount);
        nodeCount++;
    }
    if (MLENGTH(&freeNodes) == 0) {
        root = nullptr;
        MCLEAR(&freeNodes);
        return;
    }

    while (MLENGTH(&freeNodes) > 1) {
        int left = takeMin(&freeNodes);
        int right = takeMin(&freeNodes);

        Node* leftNode = nodes[left];
        Node* rightNode = nodes[right];

        nodes[nodeCount] = new Node('\0'
            , leftNode->freq + rightNode->freq
            , leftNode, rightNode);
        insertSorted(&freeNodes, nodeCount);
        nodeCount++;
    }
    int rootID = takeMin(&freeNodes);
    root = nodes[rootID];

    MCLEAR(&freeNodes);
}

int EncodeHuffman::takeMin(MArray* freeNodes) {
    int nodeID = stoi(MGET(freeNodes, MLENGTH(freeNodes) - 1));
    MDEL(freeNodes, MLENGTH(freeNodes) - 1);
    return nodeID;
}

void EncodeHuffman::insertSorted(MArray* freeNodes, int newNodeID) {
    int newFreq = nodes[newNodeID]->freq;
    int position = 0;

    while (position < MLENGTH(freeNodes)) {
        int currID = stoi(MGET(freeNodes, position));
        int currFreq = nodes[currID]->freq;

        if (newFreq > currFreq) break;
        position++;
    }
    string newNodeAsString = to_string(newNodeID);
    MINSERT(freeNodes, position, newNodeAsString);
}

void EncodeHuffman::buildCodes(Node* curr, string path) {
    if (curr == nullptr) return;

    if (!curr->left && !curr->right) {
        if (path.empty()) {
            codes[static_cast<unsigned char>(curr->symbol)] = "0";
        } else codes[static_cast<unsigned char>(curr->symbol)] = path;
        return;
    }
    buildCodes(curr->left, path + '0');
    buildCodes(curr->right, path + '1');
} 

EncodeHuffman::~EncodeHuffman()
{
    for (int i = 0; i < nodeCount; i++)
    {
        delete nodes[i];
    }
}

string EncodeHuffman::encode()
{
    string result;

    for (char symbol : input)
    {
        unsigned char index = static_cast<unsigned char>(symbol);
        result += codes[index];
    }

    return result;
}

string EncodeHuffman::decode(string bits)
{
    string result;
    if (root == nullptr) return result;

    if (root->left == nullptr && root->right == nullptr)
    {
        for (char bit : bits)
        {
            if (bit == '0') result += root->symbol;
        }
        return result;
    }

    Node* curr = root;

    for (char bit : bits)
    {
        if (bit == '0') curr = curr->left;
        else curr = curr->right;

        if (curr->left == nullptr && curr->right == nullptr)
        {
            result += curr->symbol;
            curr = root;
        }
    }

    return result;
}

EncodeHuffman::EncodeHuffman(string text) 
    : input(text), root(nullptr), nodeCount(0) {
        buildTree();
        buildCodes(root, "");
    }

void EncodeHuffman::printCodes()
{
    cout << "Таблица кодов:\n";

    for (int i = 0; i < 256; i++)
    {
        if (!codes[i].empty())
        {
            cout << static_cast<char>(i) << " : " << codes[i] << '\n';
        }
    }
}