#include "../headers/stack.h"

void SINIT(StackData* stack)
{
	stack->head = nullptr;
}

bool SISEMPTY(StackData* stack)
{
	return stack->head == nullptr;
}

void SPUSH(StackData* stack, const string& str)
{
	StackNode* newEl = new StackNode{str, nullptr};
	newEl->nextEl = stack->head;
	stack->head = newEl;
}

string SPOP(StackData* stack)
{
	if (SISEMPTY(stack))
	{
		cout << "Стек пустой удалять нечего!\n";
		return "";
	}
	StackNode* tmp = stack->head;
	string value = tmp->data;
	stack->head = stack->head->nextEl;
	delete tmp;
	return value;
}

void SPRINT(StackData* stack)
{
	if (SISEMPTY(stack))
	{
		cout << "Стек пустой выводить нечего!\n";
		return;
	}
	cout << "Умненькие и отвественные\n";
	StackNode* curr = stack->head;
	while (curr != nullptr)
	{
		cout << curr->data << endl;
		curr = curr->nextEl;
	}
}

string SGET(StackData* stack)
{
	if (SISEMPTY(stack))
	{
		cout << "Стек пустой читать нечего!\n";
		return "";
	}
	return stack->head->data;
}

void SCLEAR(StackData* stack)
{
	while (!SISEMPTY(stack))
	{
		SPOP(stack);
	}
}

