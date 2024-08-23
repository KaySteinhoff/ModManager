#include "modManaging.h"
#include "modJson.h"
#include <stdlib.h>
#include <string.h>

User ExtractUserFromJson(char *jsonData)
{
	User user = { 0 };

	jsonObject object = ParseJsonData(jsonData);

	size_t nameLength = strlen(object.elements[2].value), emailLength = strlen(object.elements[5].value);
	user.name = malloc(nameLength);
	if(!user.name)
		return user;
	user.email = malloc(emailLength);
	if(!user.email)
	{
		free(user.name);
		user.name = NULL;
		return user;
	}

	memcpy(user.name, object.elements[2].value, nameLength);
	memcpy(user.email, object.elements[5].value, emailLength);

	freeJsonObject(object);

	return user;
}
