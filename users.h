#ifndef USERS_H
#define USERS_H

#include <stdbool.h>

struct list {
   char *pJid;
   struct list *pNext;
};

bool isInList(struct list *pList, const char *pJid);
void addToList(struct list **users, const char *pJid);
void freeList(struct list *pList);

#endif // USERS_H
