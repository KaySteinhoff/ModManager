#ifndef MOD_MANAGER_JSON_H_
#define MOD_MANAGER_JSON_H_

#include <stddef.h>

typedef struct
{
	char *name;
	char *value;
}jsonElement;

typedef struct
{
	jsonElement *elements;
	size_t elementCount;
	size_t elementCapacity;
}jsonObject;

void freeJsonObject(jsonObject object);
jsonObject ParseJsonData(char *data);

#endif
