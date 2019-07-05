#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>

void echo_exec(struct ast *param, FILE *fd)
{
  bool n_mode = false;
  if (param && (strcmp(param->value, "-n") == 0))
  {
    n_mode = true;
    param = param->right;
  }

  int fst_wrd = 0;
  while (param)
  {
    if (fst_wrd == 0)
      fst_wrd++;
    else
      fwrite(" ", sizeof(char), 1, fd);

    char *tmp = param->value;
    while (*tmp != '\0')
    {
      fwrite(tmp, sizeof(char), 1, fd);
      tmp += 1;
    }
    param = param->right;
  }
  if (!n_mode)
    fwrite("\n", sizeof(char), 1, fd);
}

int kill_exec(struct ast *param)
{
  int res = 0;
  int option = SIGTERM;

  if (strcmp(param->value, "-s") == 0 || 
      strcmp(param->value, "-TERM") == 0 || 
      strcmp(param->value, "-INT") == 0 || 
      strcmp(param->value, "-HUP") == 0 || 
      strcmp(param->value, "-KILL") == 0)
  {
    if (strcmp(param->value, "-TERM") == 0)
    {
      option = SIGTERM;
      param = param->right;
    }
    else if (strcmp(param->value, "-INT") == 0)
    {
      option = SIGINT;
      param = param->right;
    }
    else if (strcmp(param->value, "-HUP") == 0)
    {
      option = SIGHUP;
      param = param->right;
    }
    else if (strcmp(param->value, "-KILL") == 0)
    {
      option = SIGKILL;
      param = param->right;
    }
    else {
      param = param->right;
      if (strcmp(param->value, "TERM") == 0)
        option = SIGTERM;
      else if (strcmp(param->value, "INT") == 0)
        option = SIGINT;
      else if (strcmp(param->value, "HUP") == 0)
        option = SIGHUP;
      else if (strcmp(param->value, "KILL") == 0)
        option = SIGKILL;
      else
      {
        fprintf(stderr, "kill: %s: invalid signal specification\n"
            , (param->value + 1));
        return -1;
      }
      param = param->right;
    }
  }

  if (param == NULL)
  {
    fprintf(stderr, "minishell: kill: usage: kill [-s] pid");
    return -1;
  }
  int val = atoi(param->value);
  pid_t pid;
  pid = fork();
  if (pid == 0)
    res = kill(val, option);
  else if (pid < 0)
    fprintf(stderr, "fork error\n");
  else
  {
    int status;
    pid = wait(&status);
  }
  return res;
}

int cd_exec(struct ast *param)
{
  if (!param)
  {
    char *home = getenv("HOME");
    if (!home)
    {
      fprintf(stderr, "error cd \n");
      return 1;
    }
    else
    {
      chdir(home);
      return 0;
    }
  }
  if (param->right)
  {
    fprintf(stderr, "minishell: cd: too many arguments\n");
    return 1;
  }
  else
  {
    if (strcmp(param->value, "~") == 0)
    {
      char *home = getenv("HOME");
      if (!home)
      {
        fprintf(stderr, "error cd \n");
        return 1;
      }
      else
      {
        chdir(home);
        return 0;
      }
    }
    else
    {
      int res = chdir(param->value);
      return res;
    }
  }
    
}

FILE *load_file(struct ast *file)
{
  FILE *fd = fopen(file->value, "w+");
  return fd;
}

char **argv_tab(char *function, struct ast *param)
{
  char **res = calloc(30, sizeof(char*));
  int idx = 0;
  res[idx] = function;
  idx++;
  struct ast *tmp = param;
  while (tmp)
  {
    res[idx] = tmp->value;
    idx++;
    tmp = tmp->right;
  }
  return res;
}

int execution_forked(char *buff, char **arg)
{
  int ret_exec = -1;

  //char **arg = argv_tab(buff, param);
  ret_exec = execvp(buff, arg);
  //free(arg);    

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
