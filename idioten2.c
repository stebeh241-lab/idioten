#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define MAX_CARDS 52

typedef struct card_t
{
    int32_t suite; // size = 4 byte
    int32_t value; // size = 4 byte
} card_t;
typedef struct pile_t
{
    card_t cards[MAX_CARDS]; // 0 = bottom, 51 = top
    size_t available_cards;
} pile_t;
typedef struct table_t
{
    pile_t piles[4]; // 0 = bottom, 51 = top
    size_t occupied_piles;
} table_t;

void print_card(const card_t *cardp)
{
    printf("suite = %d, value = %d\n", cardp->suite, cardp->value);
}
void print_pile(const pile_t *pilep)
{
    for (size_t i = 0; i < pilep->available_cards; i++)
    {
        print_card(pilep->cards + i);
    }
    printf("\navailable_cards = %zu\n", pilep->available_cards);
}
void print_table(const table_t *tablep)
{
    printf("********\n");
    printf("\npiles with cards = %zu\n", tablep->occupied_piles);
    for (size_t i = 0; i < tablep->occupied_piles; i++)
    {
        printf("pile:\n");
        print_pile(tablep->piles + i);
    }
    printf("********\n");
}

pile_t create_empty_pile()
{
    pile_t out_deck;
    out_deck.available_cards = 0u;
    return out_deck;
}
pile_t create_ordered_deck()
{
    pile_t the_deck;
    size_t n = 0;
    for (int32_t suite = 0; suite < 4; suite++)
    {
        for (int32_t value = 0; value < 13; value++)
        {
            the_deck.cards[n].suite = suite;
            the_deck.cards[n].value = value;
            n++;
        }
    }
    the_deck.available_cards = MAX_CARDS;
    return the_deck;
}
pile_t create_random_deck()
{
    pile_t ordered_deck = create_ordered_deck();
    pile_t out_deck = create_empty_pile();
    while (ordered_deck.available_cards > 0)
    {
        const size_t take_index = rand() % ordered_deck.available_cards;
        out_deck.cards[out_deck.available_cards] = ordered_deck.cards[take_index];
        out_deck.available_cards++;
        ordered_deck.cards[take_index] = ordered_deck.cards[ordered_deck.available_cards - 1];
        ordered_deck.available_cards--;
    }
    return out_deck;
}
table_t create_empty_table()
{
    table_t table;
    for (size_t i = 0; i < 4; i++)
    {
        table.piles[i] = create_empty_pile();
    }
    table.occupied_piles = 0u;
    return table;
}

card_t draw(pile_t *pilep)
{
    pilep->available_cards--;
    return pilep->cards[pilep->available_cards];
}

void place_four(pile_t *game_deck, table_t *game_table)
{
    for (size_t i = 0; i < 4; i++)
    {
        game_table->piles[i].cards[game_table->piles[i].available_cards] = draw(game_deck);
        game_table->piles[i].available_cards++;
    }
    game_table->occupied_piles = 4;
}

bool need_cleaning(table_t *game_table)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game_table->piles[i].available_cards != 0 && game_table->piles[j].available_cards != 0 &&
                game_table->piles[i].cards[game_table->piles[i].available_cards - 1].suite ==
                    game_table->piles[i].cards[game_table->piles[i].available_cards - 1].suite &&
                i != j)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

table_t *clean_table(table_t *game_table)
{
    do
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == j || game_table->piles[i].available_cards == 0 || game_table->piles[j].available_cards == 0)
                {
                    continue;
                }
                if (game_table->piles[i].cards[game_table->piles[i].available_cards - 1].suite ==
                        game_table->piles[j].cards[game_table->piles[j].available_cards - 1].suite &&
                    i != j)
                {
                    if (game_table->piles[i].cards[game_table->piles[i].available_cards - 1].value <
                            game_table->piles[j].cards[game_table->piles[j].available_cards - 1].value &&
                        i != j)
                    {
                        game_table->piles[i].available_cards--;
                    }
                    else if (game_table->piles[i].cards[game_table->piles[i].available_cards - 1].value >
                                 game_table->piles[j].cards[game_table->piles[j].available_cards - 1].value &&
                             i != j)
                    {
                        game_table->piles[j].available_cards--;
                    }
                }
                // int top_i_idx = game_table->piles[i].available_cards - 1;
                // int top_j_idx = game_table->piles[j].available_cards - 1;
                // if (game_table->) // HÄR FÅR VI FORTSÄTTA!!
            }
        }
    } while (need_cleaning(game_table));
    return game_table;
}

void main()
{
    srand(0); // seeds with current time

    pile_t game_deck = create_random_deck();
    table_t game_table = create_empty_table();
    place_four(&game_deck, &game_table);
    place_four(&game_deck, &game_table);
    print_table(&game_table);
}

// deck_t stefans_deck;
// deck_t* stefans_deckp = &stefans_deck;

// card_t* cardp =  stefans_deck   .cards;
// card_t* cardp2 = stefans_deckp ->cards;

// 0x0000008    |   0000 0000   // Detta är suite    <----------- card_pekare, deck_pekare
// 0x0000009    |   0000 0000   // Detta är suite
// 0x000000a    |   0000 0000   // Detta är suite
// 0x000000b    |   0000 0000   // Detta är suite
// 0x000000c    |   0000 0000   // Detta är value
// 0x000000d    |   0000 0000   // Detta är value
// 0x000000e    |   0000 0000   // Detta är value
// 0x000000f    |   0000 0000   // Detta är value

// 0x0000010    |   0000 0000   // Detta är suite
// 0x0000011    |   0000 0000   // Detta är suite
// 0x0000012    |   0000 0000   // Detta är suite
// 0x0000013    |   0000 0000   // Detta är suite
// 0x0000014    |   0000 0000   // Detta är value
// 0x0000015    |   0000 0000   // Detta är value
// 0x0000016    |   0000 0000   // Detta är value
// 0x0000017    |   0000 0000   // Detta är value

// 0x0000010    |   0000 0000   // Detta är suite
// 0x0000011    |   0000 0000   // Detta är suite
// 0x0000012    |   0000 0000   // Detta är suite
// 0x0000013    |   0000 0000   // Detta är suite
// 0x0000014    |   0000 0000   // Detta är value
// 0x0000015    |   0000 0000   // Detta är value
// 0x0000016    |   0000 0000   // Detta är value
// 0x0000017    |   0000 0000   // Detta är value

// ...
// 0x0000010    |   0000 0000   // Detta är available_cards
// 0x0000011    |   0000 0000   // Detta är available_cards
// 0x0000012    |   0000 0000   // Detta är available_cards
// 0x0000013    |   0000 0000   // Detta är available_cards
// 0x0000010    |   0000 0000   // Detta är available_cards
// 0x0000011    |   0000 0000   // Detta är available_cards
// 0x0000012    |   0000 0000   // Detta är available_cards
// 0x0000013    |   0000 0000   // Detta är available_cards
