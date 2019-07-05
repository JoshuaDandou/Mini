#include "parsing.c"
//#include "sig_functions.c"
//#include "return_struct.h"

void  signal_handler(int i);

char *memsize(char *src)
{
  char *ret = calloc(1, sizeof(char));
  int i = 0;
  int count = 1;
  while (src[i] != '\0')
  {
    ret = realloc(ret, count+1);
    count++;
    i++;
  }
  return ret;
}

char *load_buff(FILE *fd)
{
  int ret_fread;
  char *ret_buff = calloc(1, sizeof(char));
  int idx = 0;
  int count = 1;
  if (fd != stdin)
  {
    while ((ret_fread = fgetc(fd)) && (ret_fread != EOF))
    {
      *(ret_buff + idx) = ret_fread;
      idx++;
      ret_buff = realloc(ret_buff, count+1);
      count++;
    }
  }
  else
  { 
    while ((ret_fread = fgetc(stdin)) && (ret_fread != '\n'))
    {
      *(ret_buff + idx) = ret_fread;
      idx++;
      ret_buff = realloc(ret_buff, count+1);
      count++;
    }
  }
  *(ret_buff + idx) = '\n';
  idx++;
  ret_buff = realloc(ret_buff, count+1);
  *(ret_buff + idx) = '\0';
  return ret_buff;
}

int split_instruction(FILE *fd)
{
  int val_ret = 0;
  struct ret_instr *ret = NULL;
  char *buff = load_buff(fd);
  char *tmp = memsize(buff);
  int idx = 0;

  while ((buff[idx] != '\0') &&
      ((buff[idx] == ' ') || (buff[idx] == '\t') || (buff[idx] == '\n')))
      idx++;
  while (buff[idx] != '\0')
  {
    int idx_tmp = 0;
    while (buff[idx] != '\0' && buff[idx] != ';' && buff[idx] != '\n')
    {
      tmp[idx_tmp] = buff[idx];
      idx_tmp += 1;
      idx += 1;
    }
    tmp[idx_tmp] = '\0';
    struct ast *tree = parser(tmp);
    ret = exec_ast(tree, stdout);
    free_ast(tree);

    if (ret->code != 0)
    {
      if (strcmp(ret->msg, "exit") == 0)
      {
        //free all
        if (ret->code < 0)
          exit(0);
        exit(ret->code);
      }
    }

    for (int x = 0; x < idx_tmp; x++)
      tmp[x] = '\0';
    idx += 1;
    while ((buff[idx] != '\0') &&
      ((buff[idx] == ' ') || (buff[idx] == '\t') || (buff[idx] == '\n')))
      idx++;
    val_ret = ret->code;
    free(ret);
  }
  free(buff);
  free(tmp);

  return val_ret;
}

int interact_mode()
{
  int ret = 0;
  while (isatty(0))
  {
    write(1, "minishell$ ", 11);
    signal(SIGINT, signal_handler);
    ret = split_instruction(stdin);
  }
  while (! isatty(0)) 
  {
    ret = split_instruction(stdin);
    break;
  }
  return ret;
}
