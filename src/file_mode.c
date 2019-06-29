int read_file(char *file)
{
  int ret = 0;
  FILE* fd = fopen(file, "r");

  if (fd)
  {
    ret = split_instruction(fd);
  }
  return ret;
}
