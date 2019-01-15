#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

struct map
{
    int y_size, z_size, x_size;
    char ***arr;
};

void free_map(struct map *m);

char **split(char *line, char *delims);

struct map *read_map(char *filename);
