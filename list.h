#ifndef LIST_H
#define LIST_H

#include <stdint.h>

const size_t LIST_ERROR = -1;

struct ListNode {
	size_t prev, next;
};

struct List
{
	void* values;
	ListNode* data;
	size_t capacity;
	size_t size;
	size_t tail;
	size_t ftail;
	size_t value_size;
};

int ListCtor(List * list, size_t item_size);
int ListDtor(List * list);

size_t ListAddNode (List* list);
int  ListEraseNode (List* list, size_t index);

int ListSetValue   (List* list, size_t index, void * value);
int ListGetValue   (List* list, size_t index, void * value);
int ListErase      (List* list, size_t index);
int ListPushBefore (List* list, size_t index, size_t new_index);
int ListPushAfter  (List* list, size_t index, size_t new_index);

size_t ListPushBefore (List* list, size_t index, void * value);
size_t ListPushAfter  (List* list, size_t index, void * value);

size_t ListPushBack  (List* list, void* value);
size_t ListPushFront (List* list, void* value);

int ListGetBack  (List* list, void* value);
int ListGetFront (List* list, void* value);

int ListEraseBack  (List* list);
int ListEraseFront (List* list);

int ListEnlarge(List* list, size_t cap);

void ListDump(List* list);

#endif
