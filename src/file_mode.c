int read_file(char *file)
{
  int ret = 0;
  FILE* fd = fopen(file, "r");

  if (fd)
  {
    ret = split_instruction(fd);
    if (fclose(fd) != 0)
      fprintf(stderr, "error fclose\n");
  }
  else
  {
    fprintf(stderr, "error fopen\n");
    ret = 1;
  }

  return ret;
}
