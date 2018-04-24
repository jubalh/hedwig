// vim: noexpandtab:ts=4:sts=4:sw=4

#include <stdlib.h>
#include <string.h>
#include "users.h"

bool isInList(struct list *pList, const char *pJid)
{
	struct list *p = pList;
	while (p) {
		if (0 == strcmp(p->pJid, pJid))
			return true;
		p = p->pNext;
	}
	return false;
}

void addToList(struct list **users, const char *pJid)
{
	if (0 == *users)
	{
		*users = malloc(sizeof(struct list));
		(*users)->pJid = strdup(pJid);
		(*users)->pNext = 0;
		return;
	}
	else
	{
		struct list *p = (*users);
		while (p->pNext != 0)
			p = p->pNext;

		p->pNext = malloc(sizeof(struct list));
		p->pNext->pJid = strdup(pJid);
		p->pNext->pNext = 0;
	}
}

void freeList(struct list *pList)
{
	if (pList)
	{
		struct list *p;
		do {
			p = pList->pNext;
			free(pList->pJid);
			free(pList);
			pList = p;
		} while (pList);
	}
}
