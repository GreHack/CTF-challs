#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <time.h>
#include <string.h>
#include <sys/user.h>
#include <math.h>


struct new_values {
    int8_t change_rax;  // Should rax be changed
    uint64_t rax;  // Used only when change_rax == 1
    //TODO: add more

    struct new_values *next;  // Pointing to the next values to used when reaching the same breakpoint
};

struct breakpoint {
    uint64_t address;  // Real address of the breakpoint
    uint8_t real_instr;
    struct new_values *values;
    struct breakpoint *next;
};

struct breakpoint *breakpoints = NULL;
struct breakpoint *last_breakpoint = NULL;


uint8_t validation(char *input)
{
    int longueur = strlen(input);
    if (longueur != 29)
        return 0;
    char encrypted_flag[] = {0x45, 0x87, 0x50, 0x2c, 0xbb, 0xb7, 0x8f, 0x8c, 0x5a, 0xf3, 0x70, 0xe9, 0xf9, 0xd9, 0x27, 0xe0, 0x1d, 0xdd, 0xd6, 0xd8, 0x1b, 0xd2, 0x4f, 0x2b, 0x68, 0xa7, 0xe5, 0x91,0x7e};

    for(uint8_t i = 0; i < longueur; i++){
        double a = 0;
        a = pow(input[i], 4);
        a = sqrt(a);
        a = sqrt(a);
        input[i] = (char) a;
    }

    for (uint8_t j = 0 ; j < 5 ; j++){
        for (uint8_t i = 0; i < longueur; i++){
            uint8_t k = atoi("1337");
            if(input[i] < k)
                memfrob(input, longueur);
            else 
                input[i] = input[i] ^ k;              
            if(toupper(input[i]) == tolower(input[i])){
                int longueurl = (uint8_t)rand();
                for (uint8_t k = rand(); k < longueurl; k++){
                    input[k%longueur] ^= (uint8_t)rand();
                }
            }
        }
    }
    if (!memcmp(input, encrypted_flag, 29)){
        return 1;        
    }
    return 0;
}


void add_breakpoint(pid_t tracee_pid, struct breakpoint *bp)
{
    uint64_t tracee_instr = ptrace(PTRACE_PEEKDATA, tracee_pid, bp->address, 0);
    bp->real_instr = (uint8_t) (tracee_instr & 0x00000000000000FF);

    ptrace(PTRACE_POKETEXT, tracee_pid, bp->address, (tracee_instr & 0xFFFFFFFFFFFFFF00) | 0xCC);

}


void delete_breakpoint(pid_t tracee_pid, struct breakpoint *bp)
{
    uint64_t tracee_instr = ptrace(PTRACE_PEEKDATA, tracee_pid, bp->address, 0);

    ptrace(PTRACE_POKETEXT, tracee_pid, bp->address, (tracee_instr & 0xFFFFFFFFFFFFFF00) | bp->real_instr);
}


/*
 * Finds at which breakpoint the process tracee_pid is stopped.
 * Place this breakpoint in bp.
 */
void which_breakpoint(uint64_t address, struct breakpoint **bp)
{
    *bp = breakpoints;
    while (*bp) {
        if ((*bp)->address == address)
            break;
        *bp = (*bp)->next;
    }
}


void handle_breakpoint(pid_t tracee_pid)
{
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, tracee_pid, 0, &regs);

    /* Finding the current breakpoint */
    struct breakpoint *bp;
    which_breakpoint(regs.rip - 1, &bp);

    /* Changing values that need to be changed */
    if (bp->values->change_rax) {
        regs.rax = bp->values->rax;
    }
    /* Rollback one instruction because we just INT3 */
    regs.rip -= 1;
    /* Setting registers */
    ptrace(PTRACE_SETREGS, tracee_pid, 0, &regs);

    /* Setting back old instruction */
    delete_breakpoint(tracee_pid, bp);

    /* Moving one step */
    ptrace(PTRACE_SINGLESTEP, tracee_pid, 0, 0);
    int status;
    waitpid(tracee_pid, &status, 0);

    /* Removing the used new values */
    struct new_values *save = bp->values->next;
    free(bp->values);
    bp->values = save;

    /* If there are other values */
    if (bp->values) {
        /* Setting back the breakpoint */
        add_breakpoint(tracee_pid, bp);

    /* If there are no more values */
    } else {
        /* Removing breakpoint from the list */
        struct breakpoint *actual = breakpoints;
        struct breakpoint *old = NULL;
        while (actual) {
            if (actual == bp) {
                if (old)
                    old->next = actual->next;
                else {
                    breakpoints = actual->next;
                    if (!actual->next)
                        last_breakpoint = NULL;
                }
                free(actual);
                break;
            }
            old = actual;
            actual = actual->next;
        }
    }
}


/*
 * This is where you create the breakpoints and set the values
 * you want to change.
 */
void init_breakpoints(pid_t pid)
{
    int longueur_flag = 29;

    struct breakpoint *breakpoint;
    struct new_values *new_values;
    struct new_values *last_values;

    /* Call to atoi: Change at first ca*/
    breakpoint = malloc(sizeof(struct breakpoint));
    breakpoint->address = 0x409f08;  // TODO: change this address
    breakpoint->next = NULL;

    for (uint8_t i = 0; i < 0x05; i++){
        for (uint8_t j = 0; j < longueur_flag; j++){
            new_values = malloc(sizeof(struct new_values));
            new_values->change_rax = 1;
            new_values->rax = (i+j)%longueur_flag;
            new_values->next = NULL;
            if (i == 0 && j == 0){
                breakpoint->values = new_values;
            } else {
                last_values->next = new_values;
            }
            last_values = new_values;
        }
    }

    add_breakpoint(pid, breakpoint);
    if (last_breakpoint)
        last_breakpoint->next = breakpoint;
    else
        breakpoints = breakpoint;
    last_breakpoint = breakpoint;


    /* Calls to rand : makes it deterministic */
    breakpoint = malloc(sizeof(struct breakpoint));
    breakpoint->address = 0x40b331;  // TODO: change this address
    breakpoint->next = NULL;

    for (uint8_t i = 0; i < 0x05; i++){
        for (uint8_t j = 0; j < longueur_flag; j++){
            srand(i*j);
            uint8_t k = rand()%longueur_flag;
            new_values = malloc(sizeof(struct new_values));
            new_values->change_rax = 1;
            new_values->rax = rand() & 0xFF;
            new_values->next = NULL;
            if (i == 0 && j == 0){
                breakpoint->values = new_values;
            } else {
                last_values->next = new_values;
            }
            last_values = new_values;
            for (; k < longueur_flag; k++) {
                new_values = malloc(sizeof(struct new_values));
                new_values->change_rax = 1;
                new_values->rax = 0xFF & rand();
                new_values->next = NULL;
                last_values->next = new_values;
                last_values = new_values;
            }
        }
        new_values = malloc(sizeof(struct new_values));
        new_values->change_rax = 1;
        new_values->rax = 0xFF & rand();
        new_values->next = NULL;
        last_values->next = new_values;
        last_values = new_values;
    }

    add_breakpoint(pid, breakpoint);
    if (last_breakpoint)
        last_breakpoint->next = breakpoint;
    else
        breakpoints = breakpoint;
    last_breakpoint = breakpoint;


    /* Calls to toupper: return 1337*/
    breakpoint = malloc(sizeof(struct breakpoint));
    breakpoint->address = 0x4051ee;  // TODO: change this address
    breakpoint->next = NULL;

    for (uint8_t i = 0; i < 5; i++){
        for (uint8_t j = 0; j < longueur_flag; j++){
            new_values = malloc(sizeof(struct new_values));
            new_values->change_rax = 1;
            new_values->rax = 1337;
            new_values->next = NULL;
            if (i == 0 && j == 0){
                breakpoint->values = new_values;
            } else {
                last_values->next = new_values;
            }
            last_values = new_values;
        }
    }

    add_breakpoint(pid, breakpoint);
    if (last_breakpoint)
        last_breakpoint->next = breakpoint;
    else
        breakpoints = breakpoint;
    last_breakpoint = breakpoint;

    /* Calls to tolower: return 1337*/
    breakpoint = malloc(sizeof(struct breakpoint));
    breakpoint->address = 0x4051be;  // TODO: change this address
    breakpoint->next = NULL;

    for (uint8_t i = 0; i < 5; i++){
        for (uint8_t j = 0; j < longueur_flag; j++){
            new_values = malloc(sizeof(struct new_values));
            new_values->change_rax = 1;
            new_values->rax = 1337;
            new_values->next = NULL;
            if (i == 0 && j == 0){
                breakpoint->values = new_values;
            } else {
                last_values->next = new_values;
            }
            last_values = new_values;
        }
    }

    add_breakpoint(pid, breakpoint);
    if (last_breakpoint)
        last_breakpoint->next = breakpoint;
    else
        breakpoints = breakpoint;
    last_breakpoint = breakpoint;
}


int main(int argc, char **argv, char **envp)
{

    pid_t pid;
    int status;
    struct user_regs_struct regs;

    if (argc != 2){
        printf("Usage : ./dolos <flag>\n");
        exit(EXIT_FAILURE);
    }


    pid = fork();

    /* Child */
    if (pid == 0) {
        if(ptrace(PTRACE_TRACEME, 0, 0, 0) == -1){
            exit(1);
        }
        raise(SIGSTOP);
        if(validation(argv[1])){
            printf("GG WP!\n");
        } else {
            printf("You're not trustworthy!\n");
        }

    /* Parent */
    } else {
        waitpid(pid, &status, 0);
        init_breakpoints(pid);

        while (1) {
            if (ptrace(PTRACE_CONT, pid, 0, 0) == -1) {
                exit(EXIT_FAILURE);
            }
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) break;
            else if (WIFSTOPPED(status)) {
                if (WSTOPSIG(status) == 5)
                    handle_breakpoint(pid);
            }
        }
    }
    printf("Bye! ");

    return EXIT_SUCCESS;
}

