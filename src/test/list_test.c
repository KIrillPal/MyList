#include <stdio.h>
#include <string.h>
#include "../list.h"

typedef int cell_t;

int main()
{
	List list;
	ListCtor(&list, sizeof(cell_t));

	while (true)
	{
		char c[20];
		cell_t x, y;
		scanf("%s%d", &c, &x);
		if (strcmp(c, "+b") == 0)
		{
			printf("Added to %llu\n", ListPushBack(&list, &x));
		}
		if (strcmp(c, "+f") == 0)
		{
			printf("Added to %llu\n", ListPushFront(&list, &x));
		}
		if (strcmp(c, "+a") == 0)
		{
			scanf("%d", &y);
			printf("Added to %llu\n", ListPushAfter(&list, x, &y));
		}
		if (strcmp(c, "+d") == 0)
		{
			scanf("%d", &y);
			printf("Added to %llu\n", ListPushBefore(&list, x, &y));
		}
		if (strcmp(c, "-") == 0)
		{
			ListEraseNode(&list, x);
			printf("Erased\n");
		}
		if (strcmp(c, "g") == 0)
		{
			ListGetValue(&list, x, &y);
			printf("Value %llu\n", y);
		}
		if (strcmp(c, "d") == 0)
		{
			ListDump(&list);
		}
		if (strcmp(c, "o") == 0)
		{
			size_t p = list.tail;
			do
			{
				ListGetValue(&list, p, &x);
				printf("%d ", x);
				p = list.data[p].next;

			} while (p != list.tail);

			printf("\n");
		}
	}

	ListDtor(&list);
	return 0;
}

