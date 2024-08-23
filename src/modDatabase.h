#ifndef MOD_MANAGER_DB_H_

#include "modManaging.h"
#include <stddef.h>

typedef struct
{
	void *data;
	size_t elementSize;
	size_t length;
	size_t capacity;
}valueList_t;

int AddValueToList(valueList_t *list, void *entry);

int GetUserID(User user);
void AddUser(User user);

valueList_t GetGamesManagedByUser(User user);
void AddManagedGameForUser(User user, Game game);
void RemoveManagedGameFromUser(User user, Game game);

valueList_t GetModsForGame(Game game);
void AddModForManagedGame(Game game, Mod mod);
void RemoveModFromManagedGame(Game game, Mod mod);

#endif
