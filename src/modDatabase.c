#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modDatabase.h"

#define USER_ENTRY_SIZE (4+64)
#define GAME_ENTRY_SIZE (4+4+128+128)
#define MOD_ENTRY_SIZE (4+4+16+128)

#define min(X, Y) (X>Y?Y:X)

int AddValueToList(valueList_t *list, void *entry)
{
	if(list->length >= list->capacity)
	{
		void *tmp = realloc(list->data, (list->capacity<<1)*list->elementSize);
		if(!tmp)
			return 1;
		list->capacity <<= 1;
		list->data = tmp;
	}

	memcpy(list->data+list->elementSize*list->length, entry, list->elementSize);
	list->length++;

	return 0;
}

int GetUserID(User user)
{
	FILE *fptr = fopen("users.mmdb", "rb");
	if(!fptr)
		return -1;

	uint8_t usrEntry[USER_ENTRY_SIZE] = { 0 };
	size_t numElementsRead = 0;
	do
	{
		memset(usrEntry, 0, USER_ENTRY_SIZE);
		numElementsRead = fread(usrEntry, 1, USER_ENTRY_SIZE, fptr);
	}while(numElementsRead > 0 && strcmp((char*)usrEntry+4, user.name));
	fclose(fptr);

	if(usrEntry[5] == '\0')
		return -1;

	return ((int*)usrEntry)[0];
}

void AddUser(User user)
{
	FILE *fptr = fopen("users.mmdb", "wb");
	if(!fptr)
		return;
	fseek(fptr, -USER_ENTRY_SIZE, SEEK_END);
	uint32_t lastID = 0;

	long fpos = ftell(fptr);
	if(fpos > 0)
		fread(&lastID, 1, 4, fptr);

	uint8_t usrEntry[USER_ENTRY_SIZE] = { 0 };
	((uint32_t*)usrEntry)[0] = ++lastID;
	memcpy(usrEntry + 4, user.name, min(strlen(user.name), 64));
	fwrite(usrEntry, 1, USER_ENTRY_SIZE, fptr);

	fclose(fptr);
}

valueList_t GetGamesManagedByUser(User user)
{
	valueList_t list = {
		.elementSize = sizeof(Game),
		.length = 0,
		.capacity = 1,
		.data = malloc(sizeof(Game))
	};

	FILE *fptr = fopen("games.mmdb", "rb");
	if(!fptr)
		return (valueList_t){ 0 };

	int userID = GetUserID(user);
	if(userID == -1)
	{
		fclose(fptr);
		return (valueList_t){ 0 };
	}

	uint8_t gameEntry[GAME_ENTRY_SIZE] = { 0 };
	size_t numElementsRead = 0;
	do
	{
		memset(gameEntry, 0, GAME_ENTRY_SIZE);
		numElementsRead = fread(gameEntry, 1, GAME_ENTRY_SIZE, fptr);
		if(((uint32_t*)gameEntry)[1] == userID)
		{
			size_t nameLength = strlen((char*)gameEntry + 8);
			size_t domainLength = strlen((char*)gameEntry + (GAME_ENTRY_SIZE-128));
			Game g = {
				.name = malloc(nameLength),
				.domain = malloc(domainLength),
				.mods = 0,
				.numMods = 0
			};
			memcpy(g.name, gameEntry + 8, nameLength);
			memcpy(g.domain, gameEntry + (GAME_ENTRY_SIZE-128), domainLength);
			g.name[nameLength] = '\0';
			g.domain[domainLength] = '\0';
			AddValueToList(&list, &g);
		}
	}while(numElementsRead > 0);
	fclose(fptr);

	return list;
}

void AddManagedGameForUser(User user, Game game)
{
	FILE *fptr = fopen("games.mmdb", "wb");
	if(!fptr)
		return;
	fseek(fptr, -GAME_ENTRY_SIZE, SEEK_END);
	long fpos = ftell(fptr);
	uint32_t lastID = 0;

	if(fpos > 0)
		fread(&lastID, 1, 4, fptr);

	uint8_t gameEntry[GAME_ENTRY_SIZE] = { 0 };
	((uint32_t*)gameEntry)[0] = ++lastID;
	((uint32_t*)gameEntry)[1] = user.ID;
	memcpy(gameEntry + 8, game.name, min(strlen(game.name), 128));
	memcpy(gameEntry + (GAME_ENTRY_SIZE-128), game.domain, min(strlen(game.domain), 128));

	fwrite(gameEntry, 1, GAME_ENTRY_SIZE, fptr);
	fclose(fptr);
}

void RemoveManagedGameFromUser(User user, Game game)
{
	
}

valueList_t GetModsForGame(Game game)
{
	return (valueList_t) { 0 };
}

void AddModForManagedGame(Game game, Mod mod)
{
	
}

void RemoveModeFromManagedGame(Game game, Mod mod)
{
	
}
