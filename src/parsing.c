#include <stdbool.h>
#include <string.h>
#include "ast_function.c"

char **lexer(char *str)
{
  char **res = calloc(100, sizeof(char*));
  int idx_res = 0;

  int i = 0;
  while (str[i] != '\0')
  {
    while ((str[i] != '\0') && ((str[i] == ' ') 
          || (str[i] == '\t') || (str[i] == '\n')))
      i++;
    if (str[i] == '\0')
      break;

    char *tmp = calloc(1, sizeof(char));
    int j = 0;
    int size = 1;
    while (str[i] != '\0' && str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
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
    res = add_ast(res, word[i]);
    i++;
  }
  free(word);
  return res;
  //free word?
}
