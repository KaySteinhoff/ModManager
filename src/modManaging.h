#ifndef MOD_MANAGER_MANAGING_H_
#define MOD_MANAGER_MANAGING_H_
#include <stddef.h>

typedef struct
{
	char *modDomain;
	char *name;
	char *version;
}Mod;

typedef struct
{
	char *domain;
	char *name;
	Mod *mods;
	size_t numMods;
}Game;

typedef struct
{
	int ID;
	char *name;
	char *email;
	Game *games;
	size_t numGames;
}User;

User ExtractUserFromJson(char *jsonData);

#endif
