#include "lab.h"
#include <string.h>
#include <stdio.h>

char *get_prompt(const char *env) {
    UNUSED(env);
    // TODO: Implement this function
    return NULL;
}

int change_dir(char **dir) {
    UNUSED(dir);
    // TODO: Implement this function
    return -1;
}

char **cmd_parse(char const *line) {
    UNUSED(line);
    // TODO: Implement this function
    return NULL;
}

void cmd_free(char **line) {
    UNUSED(line);
    // TODO: Implement this function
}

char *trim_white(char *line) {
    UNUSED(line);
    // TODO: Implement this function
    return NULL;
}

bool do_builtin(struct shell *sh, char **argv) {
    UNUSED(sh);
    UNUSED(argv);
    // TODO: Implement this function
    return false;
}

void sh_init(struct shell *sh) {
    UNUSED(sh);
    // TODO: Implement this function
}

void sh_destroy(struct shell *sh) {
    UNUSED(sh);
    // TODO: Implement this function
}

void parse_args(int argc, char **argv) {
    UNUSED(argc);
    UNUSED(argv);
    // TODO: Implement this function
}