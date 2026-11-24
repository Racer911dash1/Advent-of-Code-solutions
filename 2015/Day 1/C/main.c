#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

bool is_bonus = false;
int floor = 0;
int position = 0;

int change_floor(char chr)
{
	switch (chr)
	{
		case '(':
			return 1;
		case ')':
			return -1;
		default:
			return 0;
	}
}

char *str_append(char *front, char *back)
{
	if (!front)
	{
		front = calloc(1, sizeof(char));
		if (!front)
		{
			perror("malloc");
			return NULL;
		}
	}

	size_t len = strlen(front) + strlen(back);

	char *joined = malloc(len + 1);
	if (!joined)
	{
		perror("malloc");
		return NULL;
	}
	joined = strcpy(joined, front);
	joined = strcat(joined, back);
	joined[len] = '\0';
	free(front);
	return joined;
}

char *get_file_content(const char *filename)
{
	char *str = NULL;
	char buf[4096];
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("File");
		return NULL;
	}
	while (true)
	{
		ssize_t size = read(fd, buf, sizeof(buf) - 1);
		if (size < 0)
		{
			perror("File");
			close(fd);
			return NULL;
		}
		else if (size == 0)
		{
			break;
		}
		else
		{
			buf[size] = '\0';
			str = str_append(str, buf);
			if (!str)
			{
				perror("malloc");
				close(fd);
				return NULL;
			}
		}
	}
	close(fd);
	return str;
}

void print_answer()
{
	if (is_bonus)
	{
		printf("%i\n", position);
	}
	else
	{
		printf("%i\n", floor);
	}
}

void move_floors(char *str)
{
	while (str[position])
	{
		floor += change_floor(str[position]);
		position += 1;
		if (is_bonus && floor == -1)
		{
			break;
		}
	}
}

int main(int argc, char **argv)
{
	int opt;
	while ((opt = getopt(argc, argv, "b")) != -1)
	{
		switch (opt)
		{
			case 'b':
				is_bonus = true;
				break;
			case '?':
				return EXIT_FAILURE;
			default:
				break;
		}
	}

	if (optind == argc)
	{
		char *str = get_file_content("input");
		if (!str)
		{
			return EXIT_FAILURE;
		}

		print_answer();
		free(str);
	}
	else
	{
		for (int i = optind; i < argc; i++)
		{
			floor = 0;
			position = 0;
			move_floors(argv[i]);
			print_answer();
		}
	}
	return EXIT_SUCCESS;
}
