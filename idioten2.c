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

typedef enum
{
    Hearts,
    Diamonds,
    Clubs,
    Spades
} Suite;

void print_card(const card_t *cardp)
{
    switch (cardp->suite)
    {
    case 0:
        printf("%d of Hearts\n", cardp->value);
        break;
    case 1:
        printf("%d of Diamonds\n", cardp->value);
        break;
    case 2:
        printf("%d of Clubs\n", cardp->value);
        break;
    case 3:
        printf("%d of Spades\n", cardp->value);
        break;
    }
}
void print_pile(const pile_t *pilep)
{
    printf("available_cards = %zu\n", pilep->available_cards);
    for (size_t i = 0; i < pilep->available_cards; i++)
    {
        print_card(pilep->cards + i);
    }
}
void print_table(const table_t *tablep)
{
    printf("********\n");
    printf("%zu piles with cards\n", tablep->occupied_piles);
    printf("********\n\n");
    for (size_t i = 0; i < 4; i++)
        if (tablep->piles[i].available_cards == 0)
        {
            printf("\npile: %zu is empty\n", i);
        }
        else
        {
            printf("\npile: %zu\n", i);
            print_pile(tablep->piles + i);
        }
    printf("\n********\n");
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
        for (int j = i + 1; j < 4; j++)
        {
            if (game_table->piles[i].available_cards > 0 &&
                game_table->piles[j].available_cards > 0 &&
                game_table->piles[i].cards[game_table->piles[i].available_cards - 1].suite ==
                    game_table->piles[j].cards[game_table->piles[j].available_cards - 1].suite)
            {
                printf("cleaning is needed\n");
                return true;
            }
        }
    }
    return false;
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
            }
        }
    } while (need_cleaning(game_table));
    return game_table;
}

int empty_piles(table_t *game_table)
{
    int empty_piles_count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (game_table->piles[1].available_cards == 0)
        {
            empty_piles_count++;
        }
    }
    printf("there is %d empty piles\n", empty_piles_count);
    return empty_piles_count;
}

table_t *strategy_no_brainers(table_t *game_table)
// om vi har ledig plats och en högs två översta kort har samma färg och
// den översta är lägre än det näst översta så flyttar vi ner det översta, dvs det slängs direkt.
{
    bool no_brainer_exists;
    do
    {
        no_brainer_exists = false;
        if (empty_piles > 0)
        {
            for (int i = 0; i < 4; i++)
            {
                if (game_table->piles[i].available_cards > 1)
                {
                    if (game_table->piles[i].cards[game_table->piles[i].available_cards - 1].suite ==
                            game_table->piles[i].cards[game_table->piles[i].available_cards - 2].suite &&
                        game_table->piles[i].cards[game_table->piles[i].available_cards - 1].value <
                            game_table->piles[i].cards[game_table->piles[i].available_cards - 2].value)
                    {
                        game_table->piles[i].available_cards--;
                        no_brainer_exists = true;
                    }
                }
            }
        }
    } while (no_brainer_exists);
    return game_table;
}

table_t *strategy_1(table_t *game_table)
// tar högsta kortet och lägger på ledig plats
{
    int highest_is_in_pile = -1;
    int highest_value = -1;
    for (int i = 0; i < 4; i++)
    {
        if (game_table->piles[i].available_cards > 0)
        {
            if (game_table->piles[i].cards[game_table->piles[i].available_cards - 1].value > highest_value)
            {
                highest_is_in_pile = i;
                highest_value = game_table->piles[i].cards[game_table->piles[i].available_cards - 1].value;
            }
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (game_table->piles[i].available_cards == 0)
        {
            game_table->piles[i].cards[0] = game_table->piles[highest_is_in_pile].cards
                                                [game_table->piles[highest_is_in_pile].available_cards - 1];
            game_table->piles[i].available_cards = 1;
            game_table->piles[highest_is_in_pile].available_cards--;
            return game_table;
        }
    }
    printf("we should never end up here so something is wrong\n");
    return game_table;
}

bool to_do(table_t *game_table)
// kollar bara om det finns fler än 1 kort i någon hög.
{
    for (int i = 0; i < 4; i++)
    {
        if (game_table->piles[i].available_cards > 1)
            return true;
    }
    return false;
}

bool evaluate_result(table_t *game_table)
{
    int lonely_ace = 0;
    for (int i = 0; i < 4; i++)
    {
        if (game_table->piles[i].available_cards == 1 && game_table->piles[i].cards[0].value == 13)
            lonely_ace++;
    }
    return (lonely_ace == 4);
}

void main()
{
    srand(time(NULL)); // seeds with current time

    pile_t game_deck = create_random_deck();
    table_t game_table = create_empty_table();
    for (int i = 0; i < 13; i++)
    {
        place_four(&game_deck, &game_table);
        while (need_cleaning(&game_table))
        {
            clean_table(&game_table);
            while (empty_piles(&game_table) > 0 && to_do(&game_table))
            {
                strategy_no_brainers(&game_table);
                strategy_1(&game_table);
            }
        }
    }
    if (evaluate_result(&game_table))
    {
        printf("SUCCESS!!!\n");
    }
    else
    {
        printf("failface\n");
    }
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
