#ifndef MOD_MANAGER_HTTPS_H_
#define MOD_MANAGER_HTTPS_H_

int HttpsInit(char *apikey);
char *PerformGet(char *url, int argc, char **argv);
char *PerformPost(char *args);

#endif
