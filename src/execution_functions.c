#include <sys/wait.h>
#include <signal.h>
#include <time.h>

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

  if (param && (strcmp(param->value, "-s") == 0 || 
      strcmp(param->value, "-TERM") == 0 || 
      strcmp(param->value, "-INT") == 0 || 
      strcmp(param->value, "-HUP") == 0 || 
      strcmp(param->value, "-KILL") == 0))
  {
    if (strcmp(param->value, "-TERM") == 0)
      option = SIGTERM;
    else if (strcmp(param->value, "-INT") == 0)
      option = SIGINT;
    else if (strcmp(param->value, "-HUP") == 0)
      option = SIGHUP;
    else if (strcmp(param->value, "-KILL") == 0)
      option = SIGKILL;
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
        fprintf(stderr, "minishell: kill: %s: invalid signal specification\n"
            , (param->value));
        return -1;
      }
    }
    param = param->right;
  }

  if (param == NULL)
  {
    fprintf(stderr, "minishell: kill: usage: kill [-s] pid\n");
    return -1;
  }
  res = kill(atoi(param->value), option);
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

FILE *load_file(struct ast *file, char *opt)
{
  FILE *fd = fopen(file->value, opt);
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

int execution_forked(char *buff, struct ast *param)
{
  int res= 0;
  pid_t pid;
  int res_exec = 0;
  char **arg = argv_tab(buff, param);
  pid = fork();
  fflush(stderr);
  fflush(stdout);
  if (pid == 0)
  {
    fflush(stderr);
    fflush(stdout);
    res_exec = execvp(buff, arg);
  }
  else if (pid < 0)
    fprintf(stderr, "fork error\n");
  else 
  {
    int status;
    pid = waitpid(pid, &status, 0);
    res = WEXITSTATUS(status);
  }
  fflush(stderr);
  fflush(stdout);
  free(arg);

  if (res_exec < 0)
  {
    fprintf(stderr, "minishell: %s: command not found\n", buff);
    return 127;
  }

  return res;
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
