#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"       // Function headers

// Global variable tracking player_count and used in assinging player numbers
int global_player_counter = 100;

///////////////////////////////////// MAIN /////////////////////////////////////


int main(void) {
    gameData    *game   = initialise_game();
    gameLobby   *lobby  = initialise_lobby();

    game_loop(game);

    free_lobby(lobby);
    free_game(game);

    return 0;
}


//////////////////////////////////// HELPER ////////////////////////////////////


void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void read_string(char printString[], char *destination, int strLen) {
    while (1) {
        printf("%s", printString);
        if (fgets(destination, MAX_STRING_LENGTH, stdin) == NULL) {
            printf("%s", STRING_READ_ERROR);
            flush_input();
            continue;
        }

        size_t len = strlen(destination);
        if (len > 0 && (destination[len - 1] == '\n')) {
            destination[len - 1] = '\0';
        }

        printf("%s \"%s\": ", STRING_ERROR_CHECK, destination);

        if (getchar() == 'n') {
            flush_input();
            break;
        }
    }
}

void read_int(char printString[], int *destination, int lower, int upper) {
    while (1) {
        printf("%s", printString);
        if (scanf("%d", destination) != 1) {
            printf("%s", INT_READ_ERROR);
            flush_input();
            continue;
        }

        flush_input();

        if (*destination < lower || *destination > upper) {
            printf("%s [%d - %d]", INT_RANGE_ERROR, lower, upper);
            continue;
        }

        printf("%s \"%d\": ", STRING_ERROR_CHECK, *destination);

        if (getchar() == 'n') {
            flush_input();
            break;
        }
    }
}


///////////////////////////////////// FREE /////////////////////////////////////


// Frees every questions in game, then frees game.
void free_game(gameData *game) {
    for (int i = 0;i < game->question_count;i++) {
        free_question(game->questions[i]);
    }
    free(game->questions);
    free(game);
}

// Frees hints, then question
void free_question(question *q) {
    for (int i = 0;i < q->hint_count;i++) free(q->hints[i]);
    free(q->question);
    free(q->hints);
    free(q);
}

// Frees every player in lobby
void free_lobby(gameLobby *lobby) {
    for (int i = 0;i < DEFAULT_PLAYER_MAX;i++) {
        if (lobby->players[i] != NULL) free_player(lobby->players[i]);
    } 
    free(lobby);
}

// Frees contents then player.
void free_player(player *p) {
    free(p->player_name);
    free_orderHead(p->outstanding_bids);
    free_orderHead(p->outstanding_asks);
    free(p);
}

// Order is a linked list, so frres as such. 
void free_orderHead(orderHead *h) {
    order *temp = h->next, *prev = h->next;

    while (temp != NULL) {
        prev = temp;
        temp = temp->next;
        free(prev);
    }
}


///////////////////////////////// SEQUENCE 1a) /////////////////////////////////


gameData *initialise_game() {
    gameData *game = malloc(sizeof(gameData));
    game->question_count = 0;

    printf("%s", GAME_ROUNDS);

    // Reading what intended number of game rounds should be. 
    read_int(
        GAME_ROUNDS, 
        &game->question_count, 
        GAME_ROUND_LOWER, 
        GAME_ROUND_UPPER
    );

    game->questions = malloc(game->question_count * sizeof(question *));

    for (
        game->current_question = 0; 
        game->current_question < game->question_count;
        game->current_question++
    ) game->questions[game->current_question] = initialise_question();

    return game;
}

question *initialise_question() {
    question *q = malloc(sizeof(question));
    q->question = malloc(MAX_STRING_LENGTH * sizeof(char));
    q->hints = malloc(MAX_HINTS * sizeof(char *));
    q->hint_count = 0;

    for (int i = 0; i < MAX_HINTS; i++) q->hints[i] = NULL;

    // Read question
    read_string(STRING_WRITE_Q, q->question, MAX_STRING_LENGTH);

    // Read answer
    read_int(STRING_WRITE_ANS, &q->answer, ANSWER_LOWER, ANSWER_UPPER);

    // Add hint
    printf("%s", STRING_ADD_HINT);
    char user_input = getchar();
    flush_input();

    while (user_input == 'y') {
        flush_input();
        q->hints[q->hint_count] = malloc(MAX_STRING_LENGTH * sizeof(char));
        read_string(
            STRING_WRITE_HINT, 
            q->hints[q->hint_count++], 
            MAX_STRING_LENGTH
        );

        if (q->hint_count < MAX_HINTS) {
            printf("%s", STRING_MORE_HINT);
            user_input = getchar();
        } else {
            break;
        }
    }

    return q;
}

gameLobby *initialise_lobby() {
    gameLobby *lobby = malloc(sizeof(gameLobby));
    lobby->players = malloc(DEFAULT_PLAYER_MAX * sizeof(player));
    for (int i = 0;i < DEFAULT_PLAYER_MAX;i++) lobby->players[i] = NULL;
    lobby->player_count = 0;

    while (lobby->player_count < DEFAULT_PLAYER_MAX) {
        initialise_player(lobby->players[lobby->player_count++]);

        printf("%s", ADD_PLAYER_QUERY);
        if (getchar() == 'n') {
            flush_input();
            break;
        }
    }

    return lobby;
}

void *initialise_player(player *p) {
    p->player_name = malloc(MAX_NAME_LENGTH * sizeof(char));
    read_string(REGO_PLAYER_NAME, p->player_name, MAX_NAME_LENGTH);
    p->player_number = global_player_counter;
    global_player_counter++;
    p->balance = 0;
    p->position = 0;
    p->outstanding_bids = malloc(sizeof(orderHead));
    p->outstanding_bids->next = NULL;
    p->outstanding_bids->volume = 0;
    p->outstanding_bids->price = 0;
    p->outstanding_asks = malloc(sizeof(orderHead));
    p->outstanding_asks->next = NULL;
    p->outstanding_asks->volume = 0;
    p->outstanding_asks->price = 0;
    return p;

}


///////////////////////////////// SEQUENCE 2a) /////////////////////////////////


// Start game, players can make orders. Format will be:
// player (bid / ask) (price) (volume)
void game_loop(gameData *g) {

}

// player orders get appended to a que, orders are executed when this is called.
void dequeue_orders(
    orderHead *que,
    orderHead *orderbook
) {

}

// prints order book to a csv file. something that can be processed easily for
// the visual component
void output_orderbook(orderHead *orderbook) {

}

 
///////////////////////////////// SEQUENCE 3a) /////////////////////////////////


// setteling market
// Take true value, cleanup orderbook and settle the position of everyone in the
// orderbook. 
void settle_game(gameData *g, gameLobby *lobby, orderHead *orderbook) {

}