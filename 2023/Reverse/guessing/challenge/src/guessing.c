#include "guessing.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

char messages[][100] = {
    "GH{R3v3rs3_1s_s1mpl3}\0",
    "The Gessing game begins, alea iacta est !\0",
    "Choose a number beetween 1 and 2147483647 (0 to leave) : \0",
    "Your guess is too low.\0",
    "Your guess is too high.\0",
    "Your win ! Congratulation\0",
    "Looser ! (The game is closed)\0",
    "Invalid inputs \0",
};

unsigned int stoi(char *str)
{
    int i = 0;
    char *iterator = str;
    while (*iterator != 0)
    {
        i *= 10;
        i += *iterator - 48;
        iterator++;
    }
    return i;
}
unsigned char isNumber(char *str)
{
    char *iterator = str;
    while (*iterator != 0)
    {
        if (*iterator < 48 || *iterator > 57)
            return 0;
        iterator++;
    }
    return 0xFF;
}

unsigned char read_flag(char *path)
{
    FILE *file;
    file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error opening the file");
        return 1;
    }
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        fwrite(buffer, 1, bytes_read, stdout);
    }
    printf("\n");
    fflush(stdout);
    fclose(file);
    return 0;
}
void init_game(GessingGame **game)
{
    GessingGame *__game = malloc(sizeof(GessingGame));
    __game->min = 1;
    __game->max = 2147483647;
    __game->value = (rand() % __game->max) + 1;
    *game = __game;
}

void end_game(GessingGame **game)
{
    free(*game);
}
void game_loop()
{
    GessingGame *game;
    init_game(&game);
    printf("%s\n\n", get_game_messages(MESSAGE_START));
    fflush(stdout);
    char buffer[100];
    GuessingStatus res;
    do
    {
        printf("%s\n", get_game_messages(MESSAGE_LOOP));
        fflush(stdout);
        scanf("%99s", buffer);
        res = play_round(game, buffer);
        if (res == STAT_EQUALS)
        {
            read_flag("/app/flag2.txt");
        }
        else if (res == STAT_END)
        {
            end_game(&game);
            printf("%s\n\n", get_game_messages(MESSAGE_END));
            fflush(stdout);
            return;
        }
        else if (res == STAT_LOWER)
        {
            printf("%s\n\n", get_game_messages(MESSAGE_LOWER));
            fflush(stdout);
        }
        else if (res == STAT_HIGHER)
        {
            printf("%s\n\n", get_game_messages(MESSAGE_HIGHER));
            fflush(stdout);
        }
        else if (res == STAT_INPUTRR)
        {
            printf("%s\n\n", get_game_messages(MESSAGE_INPUTS_ERROR));
            fflush(stdout);
        }
    } while (res != STAT_END && res != STAT_EQUALS);
}
GuessingStatus play_round(GessingGame *game, char *buffer)
{
    if (!isNumber(buffer))
    {
        return STAT_INPUTRR;
    }

    unsigned int value = stoi(buffer);
    int toGuess = atoi("-1");
    if (((int)value == toGuess % 4294967296)){
        read_flag("/app/flag.txt");
        return STAT_SECRET;
    }

    if (value == 0)
    {
        return STAT_END;
    }
    else if (value == game->value)
    {
        return STAT_EQUALS;
    }
    else if (value > game->value)
    {
        return STAT_HIGHER;
    }
    return STAT_LOWER;
}

char *get_game_messages(GessingMessage msg)
{
    char *res = messages[0];
    switch (msg)
    {
    case MESSAGE_START:
        res = messages[1];
        break;
    case MESSAGE_LOOP:
        res = messages[2];
        break;
    case MESSAGE_LOWER:
        res = messages[3];
        break;
    case MESSAGE_HIGHER:
        res = messages[4];
        break;
    case MESSAGE_WIN:
        res = messages[5];
        break;
    case MESSAGE_END:
        res = messages[6];
        break;
    case MESSAGE_INPUTS_ERROR:
        res = messages[7];
        break;
    }
    return res;
}
