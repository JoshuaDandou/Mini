#include "parsing.c"

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
    while ((ret_fread = fgetc(fd)) && (ret_fread != '\n'))
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
  int ret = 0;
  char *buff = load_buff(fd);
  char *tmp = memsize(buff);
  int idx = 0;
  while (buff[idx] != '\0')
  {
    int idx_tmp = 0;
     while (buff[idx] != '\0' && buff[idx] != ';')
    {
      tmp[idx_tmp] = buff[idx];
      idx_tmp += 1;
      idx += 1;
    }
    write(1, tmp, idx_tmp);
    struct ast *tree = parser(tmp);
    if (!tree)
    {
      for (int x = 0; x < idx_tmp; x++)
        tmp[x] = '\0';
      idx += 1;
    }
  }
  free(buff);
  free(tmp);
  return ret;
}

int interact_mode()
{
  int ret = 0;
  while (isatty(0))
  {
    write(1, "minishell$ ", 11);
    ret = split_instruction(stdin);
  }
  while (! isatty(0)) 
  {
    ret = split_instruction(stdin);
  }
  return ret;
}
