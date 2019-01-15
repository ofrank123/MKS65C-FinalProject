#include "map.h"

/*
 *  split() 
 *  splits @line along @delims
 *  returns a pointer to a string array with the results
 *  (remember to free!)
 *  supports an arbitrary number of splitees
 *
 */
char **split(char *line, char *delims)
{
    int i = 0;
    // first pass to find necessary length
    char oof[strlen(line)]; // make a backup, which is annoying
    strcpy(oof, line);
    char *line2 = oof; // mmmmmmmmmm
    // printf("[split] pre-split0, line2 is %s\n", line2);
    while(strsep(&line2, delims)) ++i; //actualy find the length
    // printf("[split] i is %i\n", i);
    char **args = malloc((i+1) * sizeof(char *));
    args[i] = NULL;
    // printf("[split] pre-split1, line is %s\n", line);
    // now actually fill the array
    i = 0;
    while(args[i++] = strsep(&line, delims));
    // printf("[split] separated <%s>\n\tline is %s\n", args[i-1], line);
    return args;
}

struct map *read_map(char *filename)
{
    FILE *mapfile = fopen(filename, "r");

    char linebuf[128];

    // hopefully read the dimensions
    fgets(linebuf, 128, mapfile);
    *strchr(linebuf, '\n') = '\0'; // delet newline
    char **dims = split(linebuf, " ");

    int y_size = atoi(dims[1]);
    int z_size = atoi(dims[2]);
    int x_size = atoi(dims[0]);
    char ***map = malloc(sizeof(char **) * y_size);

    int y, z;
    for(y = 0; y < y_size; ++y) {
        map[y] = malloc(sizeof(char *) * z_size);
        // printf("beginning y layer %i\n", y);
        for(z = 0; z < z_size + 1; ++z) {
            map[y][z] = malloc(sizeof(char) * x_size);
            fgets(map[y][z], x_size + 1, mapfile);

            // debug output
            // printf("x-row at [%i][%i]:\n\t<%s>\n", y, z, map[y][z]);

            fgets(linebuf, 2, mapfile); // picks up the newline
        }
        fgets(linebuf, 3, mapfile); // picks up the dash and newline
    }

    free(dims);
    fclose(mapfile);

    struct map *m = malloc(sizeof(map));
    m->y_size = y_size;
    m->x_size = x_size;
    m->z_size = z_size;
    m->arr = map;
    return m;
}

// // for testing because I suck
// int main()
// {
//     struct map *testmap = read_map("small_map");
//     int y, z;
//     printf("NUT\n");
//     for(y = 0; y < testmap->y_size; ++y) {
//         printf("------LAYER %i\n", y);
//         for(z = 0; z < testmap->z_size; ++z) {
//             printf("%s\n", testmap->arr[y][z]);
//         }
//         printf("---ENDLAYER %i\n", y);
//     }
// 
//     free_map(testmap);
//     return 0;
// }

void free_map(struct map *m)
{
    int y, z;
    for(y = 0; y < m->y_size; ++y) {
        for(z = 0; z < m->z_size; ++z) {
            free(m->arr[y][z]);
            // printf("freed row [%i][%i]\n", y, z);
        }
        free(m->arr[y]);
        // printf("freed layer [%i]\n", y);
    }
    free(m->arr);
    // printf("freed map array\n");
    free(m);
}
