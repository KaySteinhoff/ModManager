#include "httpsRequests.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <curl/curl.h>

struct curl_fetch_st
{
	char *payload;
	size_t size;
};

CURL *curl = NULL;
char userKey[108] = { 'a', 'p', 'i', 'k', 'e', 'y', ':', ' ' };

size_t curl_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct curl_fetch_st *p = (struct curl_fetch_st*)userp;

	p->payload = realloc(p->payload, p->size + realsize + 1);
	if(!p->payload)
	{
		free(p->payload);
		return -1;
	}

	memcpy(p->payload + p->size, contents, realsize);
	p->size += realsize;
	p->payload[p->size] = '\0';

	return realsize;
}

CURLcode curl_fetch_url(const char *url, struct curl_fetch_st *fetch)
{
	fetch->payload = malloc(sizeof(fetch->payload));

	if(!fetch->payload)
		return CURLE_FAILED_INIT;

	fetch->size = 0;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fetch);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 1);

	return curl_easy_perform(curl);
}

int HttpsInit(char *apikey)
{
	curl = curl_easy_init();
	if(!curl)
		return 1;

	memcpy(userKey + 8, apikey, 100);

	return 0;
}

char* PerformGet(char *url, int argc, char **argv)
{
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "accept: application/json");
	headers = curl_slist_append(headers, userKey);

	for(int i = 0; i < argc; ++i)
		headers = curl_slist_append(headers, argv[i]);

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	struct curl_fetch_st response;
	CURLcode res = curl_fetch_url(url, &response);

	if(res != CURLE_OK)
		return NULL;

	return response.payload;
}

char *PerformPost(char *args)
{
	return NULL;
}
