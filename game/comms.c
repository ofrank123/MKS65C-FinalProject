#include "comms.h"

int send_locdiff(int sd, struct actor *pl)
{
    // send diff code
    write(sd, DIFF_PLRLOC, 1);

    char coords[32];
    char d_size[3];
    // put together the coordinates
    sprintf(coords, "%i %i %i", pl->x, pl->y, pl->z);
    // put together the message length
    snprintf(d_size, 3, "%02i", strlen(coords));

    if(write(sd, d_size, 2) == -1)
        perror("writing msg length");
    if(write(sd, coords, strlen(coords)) == -1)
        perror("writing coords");
    return 0;
}
