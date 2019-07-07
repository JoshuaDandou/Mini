void	signal_handler(int i)
{
	if (i == SIGINT)
	{
    printf("\n");
    interact_mode();
	}
}
