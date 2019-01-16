#ifndef COMMS_H
#define COMMS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "player.h"

#define DIFF_BLOCKON    "N"
#define DIFF_BLOCKOFF   "F"
#define DIFF_PLRLOC     "L"

// diff format:
//   code (one of the constants)
//   two bytes explaining difflength

int send_locdiff(int sd, struct actor *pl);

// int send_blockon(int sd, int x, int y, int z);

// int send_blockoff(int sd, int x, int y, int z);

#endif
