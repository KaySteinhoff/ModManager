#include <stdio.h>
#include <string.h>
#include <webui.h>
#include "httpsRequests.h"
#include "modManaging.h"
#include "modDatabase.h"
#include "modJson.h"

size_t winID = 0;
User user = { 0 };

void registerApikey(webui_event_t *e)
{
	if(HttpsInit((char*)webui_get_string(e)))
	{
		perror("Failed to initialize https request host!\n");
		return;
	}

	char *res = PerformGet("https://api.nexusmods.com/v1/users/validate.json", 0, NULL);
	if(!res)
	{
		perror("Failed to fetch request!\n");
		return;
	}

	user = ExtractUserFromJson(res);

	user.ID = GetUserID(user);
	if(user.ID < 0)
	{
		printf("Adding user %s...\n", user.name);
		AddUser(user);
		user.ID = GetUserID(user);
	}
	printf("User ID: %d\n", user.ID);
}

void windowEvent(webui_event_t *e)
{
	if(user.name)
		webui_run(winID, "EnableLogin();");
}

void hello()
{
	printf("hello\n");
}

int main(int argc, char **argv)
{
	winID = webui_get_new_window_id();
	webui_new_window_id(winID);

	webui_bind(winID, "registerKey", registerApikey);
	webui_bind(winID, "my_c_func", windowEvent);
	webui_bind(winID, "sayHello", hello);

	webui_show(winID, "index.html");

	webui_wait();

	return 0;
}
