#pragma once
#include <iostream>

class Node
{
private:
	long long int value;
	Node* prev;
	Node* next;
public:
	Node(long long int value, Node* prev, Node* next);
	Node* GetPrev() const;
	Node* GetNext() const;
	long long int GetValue() const;
	void SetPrev(Node* prev);
	void SetNext(Node* next);
	Node* InitList(long long int value);
	Node* AddFirst(long long int value, Node** head);
	void AddNext(long long int value);
	void AddPrev(long long int value);
	Node* AddLast(long long int value, Node** tail);
	void RemoveThis();
};

