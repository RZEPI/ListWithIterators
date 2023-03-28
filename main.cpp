#include <iostream>
#include "Node.h"

#define AMOUNT_OF_ITERATORS 10
#define AVG_LEN_OF_COMMAND 10

#define COMMAND_INIT_I "i"
#define COMMAND_MOVE_FORWARD_I "+"
#define COMMAND_MOVE_BACKWARD_I "-"
#define COMMAND_REMOVE "R"
#define COMMAND_ADD_PREV ".A"
#define COMMAND_ADD_NEXT "A."
#define COMMAND_PRINT "P"

#define COMMAND_BEG "BEG"
#define COMMAND_END "END"
#define COMMAND_ALL "ALL"

#define BEG -1
#define END 10
#define ALL 11

#define BEG_REMOVE 1
#define END_REMOVE 0

#define ZERO_ASCII 48

using namespace std;

typedef struct Iterator
{
	bool init = 0;
	Node* pointer;
}Iterator;

typedef struct GroupOfIterators{
	Iterator beg;//begining of list
	Iterator iterators[AMOUNT_OF_ITERATORS];
	Iterator end;//end of list
}GroupOfIterators;

void PrintWholeList(Iterator begOfList)
{
	if (begOfList.init != NULL)
	{
		Node* tmpNode = begOfList.pointer;
		while (tmpNode != NULL)
		{
			if (tmpNode->GetValue() >= 0)
			{
				cout << tmpNode->GetValue() << " ";
				tmpNode = tmpNode->GetNext();
			}
			else
				break;
		}
		cout << '\n';
	}
}

Node* GetPointerForIterator(int posOfIterator, Node* begOfList)
{
	Node* tmpNode = begOfList;
	while (posOfIterator)
	{
		tmpNode = tmpNode->GetNext();
		posOfIterator--;
	}
	return tmpNode;
}

void InitIterators(int numberOfIterator, long long int posOfIterator, GroupOfIterators* group)
{
	switch (posOfIterator)
	{
	case BEG:
		group->iterators[numberOfIterator].pointer = group->beg.pointer;
		group->iterators[numberOfIterator].init = true;
		break;
	case END:
		group->iterators[numberOfIterator].pointer = group->end.pointer;
		group->iterators[numberOfIterator].init = true;
		break;
	default:
		group->iterators[numberOfIterator].pointer = GetPointerForIterator(posOfIterator, group->beg.pointer);
		group->iterators[numberOfIterator].init = true;
		break;
	}
}

void AddNodeForAFirstTime(long long int valueToAdd, GroupOfIterators* group)
{
	group->beg.pointer = group->beg.pointer->InitList(valueToAdd);
	group->beg.init = true;
	group->end = group->beg;
}

void MoveIteratorForward(int numberOfIterator, GroupOfIterators* group)
{
	if(group->iterators[numberOfIterator].init != NULL && group->iterators[numberOfIterator].pointer != group->end.pointer)
		group->iterators[numberOfIterator].pointer = group->iterators[numberOfIterator].pointer->GetNext();
}

void MoveIteratorBackward(int numberOfIterator, GroupOfIterators* group)
{
	if (group->iterators[numberOfIterator].init != NULL && group->iterators[numberOfIterator].pointer != group->beg.pointer)
		group->iterators[numberOfIterator].pointer = group->iterators[numberOfIterator].pointer->GetPrev();
}

void AddNodePrev(long long int valueToAdd, int numberOfIterator, GroupOfIterators* group)
{
	switch (numberOfIterator)
	{
	case BEG:
		if (group->beg.init != NULL)
			group->beg.pointer = group->beg.pointer->AddFirst(valueToAdd, &(group->beg.pointer));
		else
			AddNodeForAFirstTime(valueToAdd, group);
		break;
	case END:
		if (group->beg.init == NULL)
			group->beg.pointer = group->beg.pointer->AddFirst(valueToAdd, &(group->beg.pointer));
		else if(group->beg.pointer == group->end.pointer)
			AddNodeForAFirstTime(valueToAdd, group);
		else
			group->end.pointer->AddPrev(valueToAdd);
		break;
	default:
		if (group->iterators[numberOfIterator].pointer->GetPrev() == NULL)
			group->beg.pointer = group->iterators[numberOfIterator].pointer->AddFirst(valueToAdd, &(group->beg.pointer));
		else
			group->iterators[numberOfIterator].pointer->AddPrev(valueToAdd);
		break;
	}
}
void AddNodeNext(long long int valueToAdd, int numberOfIterator, GroupOfIterators* group)
{
	switch (numberOfIterator)
	{
	case BEG:
		if (group->beg.init == NULL)
			AddNodeForAFirstTime(valueToAdd, group);
		else if (group->beg.pointer == group->end.pointer)
			group->end.pointer = group->beg.pointer->AddLast(valueToAdd, &(group->end.pointer));
		else
			group->beg.pointer->AddNext(valueToAdd);
		break;
	case END:
		if (group->beg.init == NULL)
			AddNodeForAFirstTime(valueToAdd, group);
		else
			group->end.pointer = group->end.pointer->AddLast(valueToAdd, &(group->end.pointer));
		break;
	default:
		if (group->iterators[numberOfIterator].pointer->GetNext() == NULL)
			group->end.pointer = group->iterators[numberOfIterator].pointer->AddLast(valueToAdd, &(group->end.pointer));
		else
			group->iterators[numberOfIterator].pointer->AddNext(valueToAdd);
		break;
	}
}

void SetIterators(int numberOfIterator, GroupOfIterators* group)
{
	for (int i = 0; i < AMOUNT_OF_ITERATORS; i++)
	{
		if (i != numberOfIterator && group->iterators[i].pointer == group->iterators[numberOfIterator].pointer)
		{
			if (group->end.pointer != group->iterators[i].pointer)
				group->iterators[i].pointer = group->iterators[i].pointer->GetNext();
			else
				group->iterators[i].pointer = group->iterators[i].pointer->GetPrev();
		}
	}
}

void CheckIfIteratorIsEmpty(int numberOfIterator, GroupOfIterators* group)
{
	if (group->iterators[numberOfIterator].pointer == NULL)
		group->iterators[numberOfIterator].init = false;
}

void RemoveBegOrEnd(Iterator& pointA, Iterator& pointB, bool side)
{
	if (pointA.init != NULL)
		if (pointB.pointer == pointA.pointer)
		{
			pointA.pointer->RemoveThis();
			pointA.init = false;
			pointB = pointA;
		}
		else
		{
			Node* tmp;
			if(side)
				tmp = pointA.pointer->GetNext();
			else
				tmp = pointA.pointer->GetPrev();
			pointA.pointer->RemoveThis();
			pointA.pointer = tmp;
		}
}

void RemoveIterator(int numberOfIterator, GroupOfIterators* group, Iterator &sidePointer)
{
	sidePointer.pointer = group->iterators[numberOfIterator].pointer->GetPrev();
	group->iterators[numberOfIterator].pointer->RemoveThis();
	CheckIfIteratorIsEmpty(numberOfIterator, group);
	group->iterators[numberOfIterator].pointer = sidePointer.pointer;
}

void RemoveIteratorControll(int numberOfIterator, GroupOfIterators* group, bool side)
{
	SetIterators(numberOfIterator, group);
	if (side)
		RemoveIterator(numberOfIterator, group, group->beg);
	else
		RemoveIterator(numberOfIterator, group, group->end);
}

void RemoveNode(int numberOfIterator, GroupOfIterators* group)
{
	Node* tmp;
	switch (numberOfIterator)
	{
	case BEG:
		RemoveBegOrEnd(group->beg, group->end, BEG_REMOVE);
		break;
	case END:
		RemoveBegOrEnd(group->end, group->beg, END_REMOVE);
		break;
	default:
		if(group->iterators[numberOfIterator].init != NULL)
			if(group->beg.pointer == group->iterators[numberOfIterator].pointer)
			{
				if (group->beg.pointer == group->end.pointer)
				{
					group->beg.init = false;
					group->beg.pointer->RemoveThis();
					group->end = group->beg;
				}
				else
					RemoveIteratorControll(numberOfIterator, group, BEG_REMOVE);
			}
			else if (group->end.pointer == group->iterators[numberOfIterator].pointer)
				RemoveIteratorControll(numberOfIterator, group, END_REMOVE);
			else
			{
				tmp = group->iterators[numberOfIterator].pointer->GetNext();
				SetIterators(numberOfIterator, group);
				group->iterators[numberOfIterator].pointer->RemoveThis();
				if (tmp == NULL)
					tmp = group->end.pointer;
				group->iterators[numberOfIterator].pointer = tmp;
			}
		break;
	}
}

void PrintList(int numberOfIterator, GroupOfIterators* group)
{
	switch (numberOfIterator)
	{
	case ALL:
		PrintWholeList(group->beg);
		break;
	default:
		if(group->iterators[numberOfIterator].pointer->GetValue() >= 0)
			cout << group->iterators[numberOfIterator].pointer->GetValue() << '\n';
		break;
	}
}

long long int CharToInt(char* position)
{
	int i = 0;
	long long int number = 0;
	while (position[i] != '\0')
		i++;
	for (int j = 0; j < i; j++)
		number += (position[j] - ZERO_ASCII) * pow(10, i -1 - j);
	return number;
}

long long int DecodePos(char* position)
{
	if (!strcmp(position, COMMAND_BEG))
		return BEG;
	else if (!strcmp(position, COMMAND_END))
		return END;
	else if (!strcmp(position, COMMAND_ALL))
		return ALL;
	else
		return CharToInt(position);
}

void ParseCommand(char* command, GroupOfIterators* group)
{
	char numOfIteratorToParse[AVG_LEN_OF_COMMAND*2];
	cin >> numOfIteratorToParse;
	int numOfIterator = (int)DecodePos(numOfIteratorToParse);
	if (!strcmp(command, COMMAND_INIT_I))
	{
		char positionOfIterator[AVG_LEN_OF_COMMAND/2];
		cin >> positionOfIterator;
		InitIterators(numOfIterator, DecodePos(positionOfIterator), group);
	}
	else if (!strcmp(command, COMMAND_MOVE_FORWARD_I))
		MoveIteratorForward(numOfIterator, group);
	else if (!strcmp(command, COMMAND_MOVE_BACKWARD_I))
		MoveIteratorBackward(numOfIterator, group);
	else if (!strcmp(command, COMMAND_REMOVE))
		RemoveNode(numOfIterator, group);
	else if (!strcmp(command, COMMAND_ADD_PREV))
	{
		int valueToAdd;
		cin >> valueToAdd;
		AddNodePrev(valueToAdd, numOfIterator, group);
	} else if (!strcmp(command, COMMAND_ADD_NEXT))
	{
		long long int valueToAdd;
		cin >> valueToAdd;
		AddNodeNext(valueToAdd, numOfIterator, group);
	}
	else if (!strcmp(command, COMMAND_PRINT))
		PrintList(numOfIterator, group);
}

int main()
{
	GroupOfIterators group;
	char command[AVG_LEN_OF_COMMAND];
	while (cin >> command)
	{
		ParseCommand(command, &group);
	}
	return 0; 
}