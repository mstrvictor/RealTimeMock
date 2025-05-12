/*
** File: mock_main.c
** Project: QuantSoc Mock Real Time Mock Trading
*/

/******************************************************************************
 ***                                 M A I N                                ***
 ******************************************************************************
 *                                                                            *
 *                 Project Name : QuantSoc Real Time                          *
 *                                Mock Trading Game                           *
 *                                                                            *
 *                       Author:: Victor Tang                                 *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *		Main body of project, handles overall logic.                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "helper_main.h"


///////////////////////////////////// MAIN /////////////////////////////////////


int main(void) {
    game *g = initialise_game();

    game_log(g, 't');

    game_loop(g);

    game_log(g, 't');

    free_game(g);
    return 0;
}