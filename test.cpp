#include "header.hpp"
#include <sstream>
#include <dirent.h>
int main()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("/")) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
        if(ent->d_name[0] != '.')
            printf ("%s\n", ent->d_name);
    }
    closedir (dir);
    } else {
    /* could not open directory */
    perror ("");
    return EXIT_FAILURE;
    }
}