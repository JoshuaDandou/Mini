#include <stdbool.h>
#include <string.h>

char **lexer(char *str)
{
  char **res = calloc(10, sizeof(char*));
  int idx_res = 0;
  int size_res = 1;
  int mem = 1;

  int i = 0;
  while (str[i] != '\0')
  {
    while ((str[i] == ' ' || str[i] == '\t') && (str[i] != '\0'))
      i++;
    if (str[i] == '\0')
      break;
    
    
    char *tmp = calloc(1, sizeof(char));
    int j = 0;
    int size = 1;
    while (str[i] != ' ' && str[i] != '\n' && str[i] != '\0')
    {
      tmp[j] = str[i];
      size++;
      tmp = realloc(tmp, size);
      i++; j++;
    }

    if (j != 0)
    {
      tmp[j] = '\0';

      res[idx_res] = tmp;
      idx_res++;

      size_res++;
      if (size_res > 10)
      {
        mem++;
        res = realloc(res, size_res*mem);
      }
    }
    else
      free(tmp);

    if (str[i] == '\0')
      break;
    i++;
  }

  res[idx_res] = NULL;

  return res;
}

void echo(char **param, int x)
{
  int i = x;
  bool n_mode = false;
  if (strncmp(param[i], "-n", 2) == 0)
  {
    n_mode = true;
    i += 1;
  }

  int fst_wrd = 0;
  while (param[i] != NULL)
  {
    if (fst_wrd > 0)
      write(1, " ", 1);
    char *tmp = param[i];
    while (*tmp != '\0')
    {
      write(1, tmp, 1);
      tmp += 1;
    }
    i++;
    fst_wrd++;
  }
  if (!n_mode)
  {
    write(1, "\n", 1); 
  }
}

struct ast *parser(char *str)
{
  struct ast *res = NULL;
  
  char **word = lexer(str);
  int i = 0;
  while (word[i] != NULL)
  {
    if (strncmp(word[i], "echo", 4) == 0)
    {
      echo(word, i+1);
    }
    i++;
  }

  return res;
  //free word?
}
