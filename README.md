# CS452 Eric Johnson Project 2: Shell
This program implements a simple shell that can be run using './myprogram' after compiling. Version information can be found by executing './myprogram -v'.  The main functionality for this program can be found in lab.c where the core methods of the shell are implemented from lab.h.  Main.c contains the main method for the shell to run and calls the necessary functions from lab.c.  The method documentation in lab.h, main.c, and the majority of test-lab.c was provided. 


Steps to configure, build, run, and test the project.

## Building

```bash
make
```

## Testing

```bash
make check
```

## Run
```bash
./myprogram
```

## Clean

```bash
make clean
```

## Install Dependencies

In order to use git send-mail you need to run the following command:

```bash
make install-deps
```
