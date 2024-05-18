#ifndef GESSING_GAME_H
#define GESSING_GAME_H
    #include <stdio.h>

    #define GUESSING_EQUALS 0 
    #define GUESSING_LOWER -1
    #define GUESSING_HIGHER 1

    typedef enum {
        STAT_INIT,
        STAT_EQUALS,
        STAT_SECRET,
        STAT_LOWER,
        STAT_HIGHER,
        STAT_INPUTRR,
        STAT_END,
    } GuessingStatus;

    typedef enum {
        MESSAGE_START,
        MESSAGE_LOOP,
        MESSAGE_LOWER,
        MESSAGE_INPUTS_ERROR,
        MESSAGE_HIGHER,
        MESSAGE_WIN,
        MESSAGE_END,
    } GessingMessage;

    typedef struct{
        unsigned int value; 
        unsigned int min;
        unsigned int max;
    } GessingGame;

    void init_game(GessingGame ** game);
    GuessingStatus play_round(GessingGame *game, char * buffer);
    void end_game(GessingGame ** game);
    char* get_game_messages(GessingMessage msg);
    void game_loop();
#endif