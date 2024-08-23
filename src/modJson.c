#include "modJson.h"
#include <stdlib.h>
#include <string.h>

void freeJsonObject(jsonObject object)
{
	for(int i = 0; i < object.elementCount; ++i)
	{
		free(object.elements[i].name);
		free(object.elements[i].value);
	}
	free(object.elements);
}

jsonObject ParseJsonData(char *data)
{
	jsonObject object = { 0 };
	object.elementCount = 0;
	object.elementCapacity = 1;
	object.elements = malloc(sizeof(jsonElement));
	if(!object.elements)
		return (jsonObject){ 0 };

	for(char *c = data; *c != '\0'; ++c)
	{
		while(*c != '\"') ++c;
		c++;
		jsonElement *element = &object.elements[object.elementCount++];

		char *end = c;
		while(*end != '\"') ++end;
		element->name = malloc((end-c)+1);

		if(!element->name)
		{
			freeJsonObject(object);
			return (jsonObject){ 0 };
		}

		memcpy(element->name, c, end-c);
		c = end+2;
		end = c;
		if(*end == '\"')
		{
			c++;
			end++;
		}

		while(*end != ',' && *end != '\"' && *end != '}') ++end;
		element->value = malloc((end-c)+2);

		if(!element->value)
		{
			freeJsonObject(object);
			return (jsonObject){ 0 };
		}
		memcpy(element->value, c, end-c);

		if(object.elementCount == object.elementCapacity)
		{
			jsonElement *tmp = realloc(object.elements, (object.elementCapacity << 1)*sizeof(jsonElement));
			if(!tmp)
			{
				freeJsonObject(object);
				return (jsonObject){ 0 };
			}
			object.elementCapacity <<= 1;
			object.elements = tmp;
		}
		if(*end == '\"')
			end++;
		c = end;
	}

	return object;
}
