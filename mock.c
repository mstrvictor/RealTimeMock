/*
** File: mock.c
** Project: QuantSoc Mock Real Time Mock Trading
*/

/******************************************************************************
 ***                               H E A D E R                              ***
 ******************************************************************************
 *                                                                            *
 *                 Project Name :                                             *
 *                                                                            *
 *                       Author::                                             *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * Functions:                                                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - -*/


#include "func.h"


// Global variable tracking player_count and used in assinging player numbers
int global_player_counter = 100;

///////////////////////////////////// MAIN /////////////////////////////////////


int main(void) {
    game *g = initialise_game();

    game_log(g, 't');

    free_game(g);
    return 0;
}


///////////////////////////////// SEQUENCE 1a) /////////////////////////////////


game *initialise_game() {
    game *g = malloc(sizeof(game));
    g->data = initialise_game_data();
    g->lobby = initialise_lobby();
    g->orderbook = malloc(
        DEFAULT_ORDERBOOK_SIZE * sizeof(orderHead *)
    );

    for (int i = 0;i < DEFAULT_ORDERBOOK_SIZE;i++) {
        g->orderbook = NULL;
    }

    g->que = NULL;

    g->current_question = 0;

    return g;
}

gameData *initialise_game_data() {
    gameData *data = malloc(sizeof(gameData));

    // How many rounds of questions in this game?
    read_int(
        GAME_ROUNDS,
        &data->count,
        GAME_ROUND_LOWER,
        GAME_ROUND_UPPER
    );
 
   // Malloc for array of pointers to question_count
    data->questions = malloc(data->count * sizeof(gameQuestion *));
    
    // Initialise each question
    for (int i = 0;i < data->count;i++) {
        data->questions[i] = initialise_question();
    }

    return data;
}

gameQuestion *initialise_question() {
    gameQuestion *q = malloc(sizeof(gameQuestion));
    q->question = malloc(MAX_STRING_LENGTH * sizeof(char));
    q->hints = malloc(MAX_HINTS * sizeof(char *));
    q->hint_count = 0;

    // Read question
    read_string(
        STRING_WRITE_Q, 
        q->question, 
        MAX_STRING_LENGTH
    );

    // Read answer
    read_int(
        STRING_WRITE_ANS,
        &q->answer,
        ANSWER_LOWER,
        ANSWER_UPPER
    );

    // Add hint
    printf("%s", STRING_ADD_HINT);
    char user_input = getchar();

    while (user_input == 'y') {
        flush_input();
        q->hints[q->hint_count] = malloc(MAX_STRING_LENGTH * sizeof(char));

        // Read hint
        read_string(
            STRING_WRITE_HINT, 
            q->hints[q->hint_count], 
            MAX_STRING_LENGTH
        );

        q->hint_count++;

        if (q->hint_count < MAX_HINTS) {
            printf("%s", STRING_MORE_HINT);
            user_input = getchar();
        } else {
            break;
        }
    }

    flush_input();

    return q;
}

gameLobby *initialise_lobby() {
    gameLobby *lobby = malloc(sizeof(gameLobby));
    lobby->players = malloc(DEFAULT_PLAYER_MAX * sizeof(player *));
    lobby->player_count = 0;

    while (lobby->player_count < DEFAULT_PLAYER_MAX) {
        lobby->players[lobby->player_count] = initialise_player();
        lobby->player_count++;

        printf("%s", ADD_PLAYER_QUERY);
        if (check_response()) {
            continue;
        } else {
            break;
        }
    }

    return lobby;
}

player *initialise_player() {
    player *p = malloc(sizeof(player));

    p->player_name = malloc(MAX_NAME_LENGTH * sizeof(char));

    read_string(
        REGO_PLAYER_NAME,
        p->player_name,
        MAX_NAME_LENGTH
    );

    p->player_number = global_player_counter++;

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


///////////////////////////////// SEQUENCE 3a) /////////////////////////////////


// setteling market
// Take true value, cleanup orderbook and settle the position of everyone in the
// orderbook. 
void settle_game(gameData *g, gameLobby *lobby, orderHead *orderbook) {

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

        if (check_response()) {
            continue;
        } else {
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

        if (check_response()) {
            continue;
        } else {
            break;
        }
    }
}

void game_log(game *g, char mode) {
    if (mode == 't') {
        // Print to terminal
        printf("Game data:\n");
        printf("Number of rounds: %d\n", g->data->count);
        printf("Current question: %d\n", g->current_question);

        for (int i = 0;i < g->data->count;i++) {
            printf("Question %d: %s\n", i + 1, g->data->questions[i]->question);
            printf("Answer: %d\n", g->data->questions[i]->answer);
            printf("Hints:\n");
            for (int j = 0;j < g->data->questions[i]->hint_count;j++) {
                printf("\t%d: %s\n", j, g->data->questions[i]->hints[j]);
            }
        }

        printf("Orderbook data:\n");

    } else if (mode == 'f') {
        // Print to file

    } else {
        printf("Invalid mode");

    }
}



///////////////////////////////////// FREE /////////////////////////////////////


// Frees contents of game struct then game
void free_game(game *g) {
    free_game_data(g->data);

    free_lobby(g->lobby);

    free_orderHead(g->orderbook);
    free_orderHead(g->que);

    free(g);
}

// Frees array of questions then gameData
void free_game_data(gameData *data) {
    for (int i = 0;i < data->count;i++) {
        free_question(data->questions[i]);
    }

    free(data->questions);
    free(data);
}

// Frees hints, then answer string, then question
void free_question(gameQuestion *q) {
    for (int i = 0;i < q->hint_count;i++) free(q->hints[i]);
    free(q->hints);

    free(q->question);

    free(q);
}

// Frees every active player in lobby
void free_lobby(gameLobby *lob) {
    for (int i = 0;i < DEFAULT_PLAYER_MAX;i++) {
        // Listed as such since players can leave game, so an entry between
        // active players can be NULL.
        if (!lob->players[i]) free_player(lob->players[i]);
    } 

    free(lob->players);
    free(lob);
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
    if (!h) return;

    order *temp = h->next, *prev = h->next;

    while (temp != NULL) {
        prev = temp;
        temp = temp->next;
        free(prev);
    }

    free(h);
}

