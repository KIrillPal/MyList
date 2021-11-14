#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

#define LIST_NOLIST -2
#define PREV(index) list->data[index].prev
#define NEXT(index) list->data[index].next
#define VALUE(index) ((void*)((uint8_t*)list->values + index * list->value_size))

int ListCtor(List* list, size_t item_size)
{
	list->capacity = 0;

	list->size     = 0;
	list->tail     = LIST_NOLIST;
	list->ftail    = LIST_NOLIST;

	list->data     = NULL;
	list->values   = NULL;

	list->value_size = item_size;
	return 0;
}

int ListDtor(List* list)
{
	free(list->values);
	free(list->data);

	list->capacity = 0;
	list->size     = 0;
	list->tail  = LIST_NOLIST;
	list->ftail = LIST_NOLIST;
	list->value_size = 0;

	list->data   = NULL;
	list->values = NULL;

	return 0;
}

int ListEnlarge(List* list, size_t cap)
{
	if (list->capacity < cap)
	{
		list->data   = (ListNode*)realloc(list->data  , cap * sizeof(ListNode));
		list->values =     (void*)realloc(list->values, cap * list->value_size);

		if (list->ftail == LIST_NOLIST)
		{
			list->ftail = 0;

			NEXT(list->ftail) = list->ftail;
			PREV(list->ftail) = list->ftail;
			list->capacity++;
		}

		for (size_t i = list->capacity; i < cap; ++i)
		{
			PREV(i) = i - 1;
			NEXT(i) = i + 1;
		}
		if (list->capacity < cap)
		{
			PREV(list->capacity) = list->ftail;
			NEXT(cap - 1) = PREV(list->ftail);
			NEXT(PREV(list->ftail)) = list->capacity;
			PREV(list->ftail) = cap - 1;
		}

		list->capacity = cap;
	}

	return 0;
}

size_t ListAddNode(List* list)
{
	size_t st = 0;

	if (list->size + 1 == list->capacity)
		ListEnlarge(list, list->capacity * 2);

	size_t node = list->ftail;

	st |= ListErase(list, node);

	if (list->tail == LIST_NOLIST)
		list->tail = node;

	return st ? LIST_ERROR : node;
}

int ListEraseNode(List* list, size_t index)
{
	int st = 0;
	st |= ListErase(list, index);
	st |= ListPushBefore(list, list->ftail, index);
	return st;
}

int ListSetValue(List* list, size_t index, void* value)
{
	void * dest = memcpy((char*)list->values + index * list->value_size, value, list->value_size);
	return dest ? 0 : LIST_ERROR;
}

int ListGetValue(List* list, size_t index, void* value)
{
	void* dest = memcpy(value, (char*)list->values + index * list->value_size, list->value_size);
	return dest ? 0 : LIST_ERROR;
}

int ListErase(List* list, size_t index)
{
	size_t st = 0;

	if (index == list->tail)
		list->tail = NEXT(index);
	else if (index == list->ftail)
		list->ftail = NEXT(index);

	size_t prev = PREV(index);
	size_t next = NEXT(index);

	NEXT(PREV(index)) = next;
	PREV(NEXT(index)) = prev;

	PREV(index) = index;
	NEXT(index) = index;

	return st;
}

int ListPushBefore(List* list, size_t index, size_t new_index)
{
	PREV(new_index) = PREV(index);
	NEXT(new_index) = index;

	NEXT(PREV(index)) = new_index;
	PREV(index) = new_index;

	return 0;
}

int ListPushAfter(List* list, size_t index, size_t new_index)
{
	PREV(new_index) = index;
	NEXT(new_index) = NEXT(index);

	PREV(NEXT(index)) = new_index;
	NEXT(index) = new_index;

	return 0;
}

size_t ListPushBefore(List* list, size_t index, void* value)
{
	size_t st = 0;

	size_t node = ListAddNode(list);

	if (index != LIST_NOLIST)
		st |= ListPushBefore(list, index, node);

	st |= ListSetValue (list, node, value);
	list->size++;

	return st ? LIST_ERROR : node;
}

size_t ListPushAfter(List* list, size_t index, void* value)
{
	size_t st = 0;

	size_t node = ListAddNode(list);

	if (index != LIST_NOLIST)
		st |= ListPushAfter(list, index, node);

	st |= ListSetValue (list, node, value);
	list->size++;

	return st ? LIST_ERROR : node;
}

size_t ListPushBack(List* list, void* value)
{
	if (list->capacity == 0)
		ListEnlarge(list, 1);
	return ListPushBefore(list, list->tail, value);
}

size_t ListPushFront(List* list, void* value)
{
	if (list->capacity == 0)
		ListEnlarge(list, 1);
	size_t node = ListPushBefore(list, list->tail, value);
	list->tail = PREV(list->tail);
	return node;
}

int ListGetBack(List* list, void* value)
{
	return ListGetValue(list, PREV(list->tail), value);
}

int ListGetFront(List* list, void* value)
{
	return ListGetValue(list, list->tail, value);
}

int ListEraseBack(List* list)
{
	return ListEraseNode(list, PREV(list->tail));
}

int ListEraseFront(List* list)
{
	return ListEraseNode(list, list->tail);
}

void ListDump(List* list)
{
	printf("Dumping:\n");
	printf("Value size: %llu\n", list->value_size);
	printf("Size:       %llu\n", list->size);
	printf("Capacity:   %llu\n", list->capacity);
	printf("Tail:       %llu\n", list->tail);
	printf("Ftail:      %llu\n\n", list->ftail);
	printf("Data:       %p\n", list->data);
	printf("Values:     %p\n", list->values);
	printf("-------------------------\n");
	for (int i = 0; i < list->capacity; ++i)
	{
		printf("\n%llu\n", i);
		printf("| %4p |", VALUE(i));
		if (VALUE(i) == NULL)
		printf(" NULL |");
		else
		printf(" %4d |", *(int*)VALUE(i));
		printf(" %2llu | %2llu |\n", PREV(i), NEXT(i));
	}
	printf("-------------------------\n");
}