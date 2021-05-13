#include <string.h>
#include <sys/stat.h>
#include "colsh.h"

char *config_set(void);
int checkfile(const char *filename); 

int main(int argc, char **argv) {
    char *path = config_set();
    if(checkfile(path) == 0) {
        // Parse configs, enabling or disabling features 
        // config_load();
    }
    colsh_loop(); 

    return EXIT_SUCCESS;
}

// Load the config path, future could be used as the 'source' command
char *config_set(void) {
    char *homedir = getenv("HOME");
    char *filename = "/.colsh";
    char *path = strcat(homedir, filename);
    
    return path;
}

// Check if a file exists using the stat() function.
int checkfile(const char *filename) {
    struct stat buffer;
    int exist = stat(filename, &buffer);
    if(exist == 0) {
        return 0;
    } else {
        return 1;
    }
}
