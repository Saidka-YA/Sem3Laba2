#include "linear_list.h"

void FPRINT(FNode* element)
{
	while (element != nullptr)
	{
		cout << element->data << endl;
		element = element->nextEl;
	}
}

void FINSERTAFTER(FNode* element, const string& str)
{
	FNode* newEl = new FNode;
	newEl->data = str;
	newEl->nextEl = nullptr;
	newEl->nextEl = element->nextEl;
	element->nextEl = newEl;
}

void FDELAFTER(FNode* element)
{
	FNode* tmp = element->nextEl;
	element->nextEl = element->nextEl->nextEl;
	delete tmp;
}

void FPUSHHEAD(FNode*& head, const string& str)
{
	FNode* newEl = new FNode;
	newEl->data = str;
	newEl->nextEl = head;
	head = newEl;
}

void FDELHEAD(FNode*& head)
{
	if (head == nullptr) return;
	FNode* tmp = head;
	head = head->nextEl;
	delete tmp;
}

void FINSERTBEFORE(FNode*& head, FNode* element, const string& str)
{
	if (element == nullptr)
	{
		cout << "Элемент не найден!\n";
		return;
	}
	if (element == head)
	{
		FPUSHHEAD(head, str);
		return;
	}
	FNode* prev = head;
	while (prev != nullptr && prev->nextEl != element)
	{
		prev = prev->nextEl;
	}
	if (prev == nullptr)
	{
		cout << "Элемент не найден!\n";
		return;
	}
	FINSERTAFTER(prev, str);
}

void FPUSHTAIL(FNode*& head, const string& str)
{
	FNode* newEl = new FNode{str, nullptr};
	if (head == nullptr)
	{
		head = newEl;
		return;
	}
	FNode* curr = head;
	while (curr->nextEl != nullptr)
	{
		curr = curr->nextEl;
	}
	curr->nextEl = newEl;
}

void FDELBEFORE(FNode*& head, FNode* element)
{
	if (head == nullptr || element == nullptr || head == element)
	{
		cout << "Перед элементом нечего удалять!\n";
		return;
	}
	if (head->nextEl == element)
	{
		FDELHEAD(head);
		return;
	}
	FNode* prevPrev = head;
	while (prevPrev->nextEl != nullptr && prevPrev->nextEl->nextEl != element)
	{
		prevPrev = prevPrev->nextEl;
	}
	if (prevPrev->nextEl == nullptr || prevPrev->nextEl->nextEl != element)
	{
		cout << "Элемент не найден!\n";
		return;
	}
	FNode* tmp = prevPrev->nextEl;
	prevPrev->nextEl = element;
	delete tmp;
}

void FDELTAIL(FNode*& head)
{
	if (head == nullptr) return;
	if (head->nextEl == nullptr)
	{
		FDELHEAD(head);
		return;
	}
	FNode* curr = head;
	while (curr->nextEl->nextEl != nullptr)
	{
		curr = curr->nextEl;
	}
	delete curr->nextEl;
	curr->nextEl = nullptr;
}

FNode* FGET(FNode* head, const string& str)
{
	while (head != nullptr)
	{
		if (head->data == str) return head;
		head = head->nextEl;
	}
	return nullptr;
}

void FDELVALUE(FNode*& head, const string& str)
{
	if (head == nullptr) return;
	if (head->data == str)
	{
		FDELHEAD(head);
		return;
	}
	FNode* prev = head;
	while (prev->nextEl != nullptr && prev->nextEl->data != str)
	{
		prev = prev->nextEl;
	}
	if (prev->nextEl == nullptr)
	{
		cout << "Элемент не найден!\n";
		return;
	}
	FDELAFTER(prev);
}

void FPRINTREVERSE(FNode* head)
{
	if (head == nullptr) return;
	FPRINTREVERSE(head->nextEl);
	cout << head->data << endl;
}