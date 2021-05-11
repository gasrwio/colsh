#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

char *config_set(void);
char *read_line(void);
char **line_split_args(char *line);
void yash_loop(void);
int checkfile(const char *filename); 
int yash_launch(char **args);
int yash_execute(char **args);
int yash_num_builtins();
int yash_cd(char **args);
int yash_help(char **args);
int yash_exit(char **args);

int (*builtin_func[]) (char**) = {
    &yash_cd,
    &yash_help,
    &yash_exit
};

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int main(int argc, char **argv) {
    char *path = config_set();
    if(checkfile(path) == 0) {
        // TODO: Parse configs, enabling or disabling features 
        // config_load();
    }
    yash_loop(); 

    return EXIT_SUCCESS;
}

// Main loop
void yash_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        line = read_line();
        args = line_split_args(line);
        status = yash_execute(args);
    } while(status);
}

int yash_launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if(pid == 0) {
        if(execvp(args[0], args) == -1) {
            perror("yash");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0) {
        perror("yash");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int yash_execute(char **args) {
    if(args[0] == NULL) {
        return 1;
    }

    for(int i = 0; i < yash_num_builtins(); i++) {
        if(strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return yash_launch(args);
}

// Read line, handling EOF for files or Ctrl-D
char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0; 

    if (getline(&line, &bufsize, stdin) == -1){
        if (feof(stdin)) {  
            exit(EXIT_SUCCESS);
    } else {
        perror("readline");
        exit(EXIT_FAILURE);
    }
  }
  return line;
}

// Split line of args into tokens.
#define TOK_BUFSIZE 64
#define TOK_DELIM "\t\r\n\a"
char **line_split_args(char *line) {
    int bufsize = TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if(!tokens) {
        fprintf(stderr, "yash: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);
    while(token != NULL) {
        tokens[position] = token;
        position++;
        
        if(position >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(!tokens) {
                fprintf(stderr, "yash: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

// Load the config path, future could be used as the 'source' command
char *config_set(void) {
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

int yash_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// cd is not working, work on this latter;
int yash_cd(char **args) {
    if(args[1] == NULL) {
        fprintf(stderr, "yash: expected argument to \"cd\"\n");
    } else if(chdir(args[1]) != 0) {
        perror("yash");
    }

    return 1;
}

int yash_help(char **args) {
    puts("Type program names and arguments, and hit enter."); 
    puts("The following are built in:");

    for(int i = 0; i < yash_num_builtins(); i++) {
        printf("\t%s\n", builtin_str[i]);
    }

    puts("Use the man command for information on other programs.");

    return 1;
}

int yash_exit(char **args) {
    return 0;
}
