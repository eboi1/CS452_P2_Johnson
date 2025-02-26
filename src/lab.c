#include "lab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#include <termios.h>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>

/**
 * lab.c - Implements those functions defined in lab.h to contain the basic functions
 *         necessary to create a simple shell program.  The functions are called in main.c.
 *
 * @author Eric Johnson
 */

/**
 * Gets the prompt from the environment variable.
 *  @param env the environment variable
 *  @see lab.h
 */
char *get_prompt(const char *env)
{
    const char *envVariable = getenv(env);  //use getenv 
    const char *defaultPrompt = "shell>";

    if (!envVariable) 
    {
        envVariable = defaultPrompt;   //if no environment variable is provided use default
    }

    char *prompt = malloc(strlen(envVariable) + 1);
    if (!prompt)    //check for memory allocation errors
    {
        fprintf(stderr, "Error: memory allocation\n");
        exit(EXIT_FAILURE);
    }
    strcpy(prompt, envVariable);    //set prommpt to the environment variable
    return prompt;
}


/**
 * This function changes the shell directory.
 *  @param dir the destination directory
 *  @see lab.h
 */
int change_dir(char **dir)
{
    char *destDirectory = NULL;

    if (dir[1] == NULL) //if no directory is provided, cd to home directory
    {
        destDirectory = getenv("HOME");
    } else{
        destDirectory = dir[1];
    }

    if (chdir(destDirectory) != 0)
    {
        fprintf(stderr, "Error: cd error\n");
        return -1;
    }
    return 0;
}

/**
 * This function parses the line from the shell into tokens.
 * @param line the line to parse
 *  @see lab.h
 */
char **cmd_parse(const char *line)
{
    if (!line) 
    {
        return NULL;
    }
    int numToken = 0;
    int inToken = 0;
    for (const char *ptr = line; *ptr; ptr++)
    {
        if (!isspace((unsigned char)*ptr))
        {
            if (!inToken)
            {
                numToken++;
                inToken = 1;
            }
        }
        else
        {
            inToken = 0;
        }
    }

    char **tokenArray = malloc((numToken + 1) * sizeof(char *));
    if (!tokenArray)
        return NULL;

    char *copy = strdup(line);
    if (!copy)
    {
        free(tokenArray);
        return NULL;
    }

    int index = 0;
    char *token = strtok(copy, " ");

    while (token)
    {
        tokenArray[index] = strdup(token);

        if (!tokenArray[index])
        {
            for (int i = 0; i < index; i++) 
            {
                free(tokenArray[i]);
            }
            free(tokenArray);
            free(copy);
            return NULL;
        }
        index++;
        token = strtok(NULL, " ");
    }
    tokenArray[index] = NULL;
    free(copy);
    return tokenArray;
}

/**
 * This function frees the memory allocated for the tokens in the line.
 *  @param line the line to free
 *  @see lab.h
 */
void cmd_free(char **line)
{
    if (!line)
    {
        return; //line is empty
    }
    for (int i = 0; line[i] != NULL; i++)
    {
        free(line[i]);
    }
    free(line);
}

/**
 * This function trims starting or tailing white space on the line for later
 * argument parsing.
 *  @param line  line to trim
 *  @see lab.h
 */
char *trim_white(char *line)
{
    if (!line)
    {
        return NULL;
    }

    int initial = 0;
    while (line[initial] && isspace((unsigned char)line[initial]))
    {
        initial++;
    }

    if (initial > 0)
    {
        int i = 0;
        while (line[initial + i])
        {
            line[i] = line[initial + i];
            i++;
        }
        line[i] = '\0';
    }

    int length = strlen(line);
    while (length > 0 && isspace((unsigned char)line[length - 1]))
    {
        length--;
    }
    line[length] = '\0';
    return line;
}

/**
 * This function checks to see if the command is a built-in command
 * such as exit, cd, or history. If it is, it executes the respective function
 * call.
 *  @param sh the shell
 *  @param argv the args
 *  @see lab.h
 */
bool do_builtin(struct shell *sh, char **argv)
{
    if (!argv || !argv[0])
        return false;
    if (strcmp(argv[0], "exit") == 0)
    {
        sh_destroy(sh);
        exit(0);
    }
    else if (strcmp(argv[0], "cd") == 0)
    {
        change_dir(argv);
        return true;
    }
    else if (strcmp(argv[0], "history") == 0)
    {
        HIST_ENTRY **hist = history_list();
        if (hist)
        {
            for (int i = 0; hist[i] != NULL; i++)
            {
                printf("%s\n", hist[i]->line);
            }
        }
        return true;
    }
    return false;
}

/**
 * This function initializes the shell.
 * @param sh the shell
 *  @see lab.h
 */
void sh_init(struct shell *sh)
{
    sh->shell_terminal = STDIN_FILENO;
    sh->shell_is_interactive = isatty(sh->shell_terminal);
    if (sh->shell_is_interactive)
    {
        while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp()))
            kill(-sh->shell_pgid, SIGTTIN);
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        tcgetattr(sh->shell_terminal, &sh->shell_tmodes);
    }
    sh->prompt = get_prompt("MY_PROMPT");
}

/**
 * This function destroys the shell and frees allocated memory.
 *  @param sh the shell
 *  @see lab.h
 */
void sh_destroy(struct shell *sh)
{
    if (sh->prompt)
    {
        free(sh->prompt);
        sh->prompt = NULL;
    }
    tcsetattr(sh->shell_terminal, TCSANOW, &sh->shell_tmodes);
}

/**
 * This function parses the arguments from the provided array.
 * @param argc the number of args
 * @param argv the arg array
 *  @see lab.h
 */
void parse_args(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "v")) != -1)
    {
        switch (opt)
        {
        case 'v':
            printf("Shell version %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
            exit(EXIT_SUCCESS);
        default:
            fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}