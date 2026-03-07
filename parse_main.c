#include <stdio.h>
#include <string.h>

int	valid_extension(char *filename)
{
	return (strcmp(filename + strlen(filename) - 4, ".cub"));
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr,"Error:\n   Invalid argument count\n");
		return (1);
	}

	if (valid_extension(argv[1]) != 0)
	{
		fprintf(stderr,"Usage:\n   ./cub3D <map.cub>\n");
		return (1);
	}

	printf("map file: %s\n", argv[1]);
	return 0;
}
