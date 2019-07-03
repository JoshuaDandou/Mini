#include <unistd.h>
#include <sys/wait.h>

char **argv_tab(char *function, struct ast *param)
{
  char **res = calloc(1, sizeof(char*));
  int idx = 0;
  int size = 1;
  res[idx] = function;
  size++;
  res = realloc(res, size);
  idx++;
  while (param)
  {
    res[idx] = param->value;
    size++;
    res = realloc(res, size);
    idx++;
    param = param->right;
  }
  res[idx] = NULL;
  return res;
}

int execution_forked(char *buff, struct ast *param)
{
  int ret_exec = 0;
  int status;

  if (fork())
  {
    wait(&status);
  }
  else
  {
    char **arg = argv_tab(buff, param);
    ret_exec = execvp(buff, arg);
  }
  return ret_exec;
}

int exit_exec(struct ast *param)
{
  if (param)
  {
    int res = atoi(param->value);
    if ((res >= 0) && (res <= 255))
      return res;
  }
  return -1;
}
