#include "Node.h"

using namespace std;

Node::Node(long long int value, Node* prev, Node* next) : value(value), prev(prev), next(next)
{
	this->value = value;
	this->next = next;
	this->prev = prev;
}
Node* Node::GetPrev() const
{
	if (this != NULL)
		return this->prev;
	else
		return nullptr;
}
Node* Node::GetNext() const
{
	if (this != NULL)
		return this->next;
	else
		return nullptr;
}
long long int Node::GetValue() const
{
	if (this != NULL)
		return this->value;
	else
		return -1;
}
void Node::SetNext(Node* next)
{
	this->next = next;
}
void Node::SetPrev(Node* prev)
{
	this->prev = prev;
}
Node* Node::InitList(long long int value)
{
	Node* newNode = new Node(value, NULL, NULL);
	return newNode;
}
Node* Node::AddFirst(long long int value, Node** head)
{
	Node* newNode = new Node(value, NULL, *head);
	(*head)->SetPrev(newNode);
	return newNode;
}
void Node::AddNext(long long int value)
{
	Node* newNode = new Node(value, this, this->GetNext());
	this->GetNext()->SetPrev(newNode);
	this->SetNext(newNode);
}
void Node::AddPrev(long long int value)
{
	Node* newNode = new Node(value, this->GetPrev(), this);
	this->GetPrev()->SetNext(newNode);
	this->SetPrev(newNode);
}
Node* Node::AddLast(long long int value, Node** tail)
{
	Node* newNode = new Node(value, *tail, NULL);
	(*tail)->SetNext(newNode);
	return newNode;
}
void Node::RemoveThis()
{
	if(this != NULL)
		if (this->prev == NULL && this->next == NULL)
			delete this;
		else if (this->prev == NULL)
		{
			this->next->prev = nullptr;
			delete this;
		}
		else if (this->next == NULL)
		{
			this->prev->next = nullptr;
			delete this;
		}
		else
		{
			this->prev->next = this->next;
			this->next->prev = this->prev;
			delete this;
		}
}
