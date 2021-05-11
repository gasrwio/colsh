#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char *config_set();
int checkfile(const char *filename); 

int main(int argc, char **argv) {
    char *path = config_set();
    if(checkfile(path) == 0) {
        // TODO: Parse configs, enabling or disabling features 
        // config_load();
    }
    // TODO: The main shell loop, which interpet the commands
    // dsh_loop(); 
}

// Load the config path, future could be used as the 'source' command
char *config_set() {
    char *homedir = getenv("HOME");
    char *filename = "/.yash";
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


