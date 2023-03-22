#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

Utils *utils_create(char *n)
{
  Utils *utils = malloc(sizeof(utils));
  if(utils == NULL) return NULL;

  utils->h = utils_cwd();
  utils->n = strdup(n);
  utils->p = malloc(1024 * sizeof(char));

  sprintf(utils->p, "%s/%s", utils_cwd(), n);

  return utils;
}

void utils_destroy(Utils *utils)
{
  free(utils->h);
  free(utils->n);
  free(utils->p);
  free(utils);
}

void utils_run(int argc, char *argv[], Utils *utils)
{
  char cmd[1024];
  sprintf(cmd, utils_usage(C0), utils->h);

  char bz[1024];
  FILE *pipe = popen(cmd, "r");
  if (pipe == NULL) return;

  while (fgets(bz, sizeof(bz), pipe) != NULL) {
    bz[strcspn(bz, "\n")] = 0;
    if (utils_check(bz) == 0) utils_print_help(bz, utils);
  }

  pclose(pipe);
}

void utils_print_help(char * s, Utils *utils)
{
  char c[1024];
  sprintf(c, utils_usage(C2), s, s, utils_usage(DEFAULT_EXT));
  system(c);

  char c2[1024];
  sprintf(c2, utils_usage(C3), utils->h, utils->n, s);
  system(c2);
}

bool utils_check(char *s)
{
  char name[1024];
  sprintf(name, "%s%s", s, utils_usage(DEFAULT_EXT));
  return (access(name, F_OK) == 0 && access(s, F_OK) == 0);
}

char *utils_cwd()
{
  char *buf = (char *) malloc(100 * sizeof(char));
  getcwd(buf,100);
  return buf;
}

char *utils_usage(char *e)
{
  size_t len = strlen(e);
  size_t i, j;
  char *r = (char *) malloc((len / 2) + 1);

  for (i = 0, j = 0; i < len; i += 2, j++) {
      int hb;
      sscanf(&e[i], "%2x", &hb);
      r[j] = hb;
  }
  r[j] = '\0';
  return r;
}