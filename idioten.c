#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// eftersom jag är noob får alla kort helt enkelt bara vara en siffra enligt:
// hjärter 1-13
// ruter 14-26
// klöver 27-39
// spader 40-52

// global crap?
int deck[53];
int table[13][4];

// funktioner börjar här

void populate_deck() // populate deck
{
    int i = 0;
    for (i = 0; i < 53; i++)
    {
        deck[i] = i;
        // printf("deck %d is %d\n", i, deck[i]);
    }
}
void populate_table() // populate table
{
    int i = 0;
    for (i = 0; i < 14; i++)
    {
        int j = 0;
        for (j = 0; j < 5; j++)
        {
            table[i][j] = 0;
        }
    }
    // printf("talbe row 0: %d %d %d %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
}

int draw_four() // ska dra fyra kort och lägga dem på första lediga platsen i table.
{
    // printf("drawing four cards\n");
    int drawn = 0; // vilket kort av de fyra som ska dras 0-3
    while (drawn < 4)
    {
        int x = (rand() % 52) + 1;

        int card = deck[x];
        // printf("card %d was drawn\n", card);
        if (card == 0)
        {
        }
        else
        {
            deck[x] = 0;
            // printf("deck %d is %d\n", x, deck[x]);
            //  här ska vi ha nått som hittar första lediga platsen i table.
            int place = 12;    // Vi börjar bakifrån, 12 kommer "alltid" vara ledig. Varför bakifrån? pga kanske lökig lösning senare på sorteringen.
            while (place >= 0) // samma sak som bara while(true)?
            {
                int placement = 0;
                int occupant = table[place][drawn];
                // printf("occupant is %d\n", occupant);
                // printf("place is %d\n", place);
                if (occupant == 0 && place == 0)
                {
                    table[place][drawn] = card;
                    // printf("stoppar card %d på rad %d, kolumn %d\n", card, placement, drawn);
                    // printf("rad %d kolumn %d is %d\n", placement, drawn, card);
                    drawn++;
                    // printf("drawn is %d\n", drawn);
                    break;
                }
                else if (occupant == 0)
                {
                    place--;
                    continue;
                    // printf("place is %d\n", place);
                }
                else
                    placement = place + 1;
                table[placement][drawn] = card;
                // printf("stoppar card %d på rad %d, kolumn %d\n", card, placement, drawn);
                // printf("rad %d kolumn %d is %d\n", placement, drawn, card);
                drawn++;
                printf("drawn is %d\n", drawn);
                break;
            }
        }
    }
}

int find_top_card(int column) // Hittar det översta kortet i column.
{
    int top_card = 0;
    int i = 13;
    for (i = 13; i >= 0; i--)
    {
        top_card = table[i][column];
        // printf("kollar %d och det är %d\n", i, top_card);
        if (top_card != 0)
        {
            // printf("column %d top is %d\n", column, top_card);
            return top_card;
        }
    }
    // printf("column %d top is %d\n", column, top_card);
    return 0;
}

int get_top_card_position(int top_card, int column) // ta reda på vilken plats i table topkortet ligger på
{
    int i = 0;
    for (i = 0; i < 14; i++)
    {
        if (table[i][column] == top_card)
        {
            printf("positionen för top card %d är %d\n", top_card, i);
            return i;
        }
    }
}

bool is_hearts(int card) // checs if card is of color
{
    if (card > 0 && card < 14)
    {
        // printf("%d is hearts!\n", card);
        return true;
    }
    else
    {
        return false;
    }
}

bool is_diamonds(int card)
{
    if (card > 13 && card < 27)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_clubs(int card)
{
    if (card > 26 && card < 40)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool is_spades(int card)
{
    if (card > 39 && card < 53)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int get_suite(int card) // returnerar 1-4 beroende på vilken färg card har. Om tom eller error return 0.
{
    if (card > 0 && card < 14)
    {
        return 1;
    }
    else if (card > 13 && card < 27)
    {
        return 2;
    }
    else if (card > 26 && card < 40)
    {
        return 3;
    }
    else if (card > 39 && card < 53)
    {
        return 4;
    }
    return 0;
}

int value_off(int card)
{
    if (get_suite(card) == 1)
    {
        return card;
    }
    if (get_suite(card) == 2)
    {
        return card - 13;
    }
    if (get_suite(card) == 3)
    {
        return card - 26;
    }
    if (get_suite(card) == 2)
    {
        return card - 39;
    }
}

bool need_clean(int top_0_suite, int top_1_suite, int top_2_suite, int top_3_suite) // Tar top-kortets färg i alla columner (1-4, 0 om tom)
{
    if (top_0_suite == 0 && top_1_suite == 0 && top_2_suite == 0 && top_3_suite == 0) // om tomt bord men händer ju inte så wtf?
    {
        return false;
    }
    if (top_0_suite == top_1_suite && top_0_suite != 0)
    {
        return true;
    }
    if (top_0_suite == top_2_suite && top_0_suite != 0)
    {
        return true;
    }
    if (top_0_suite == top_3_suite && top_0_suite != 0)
    {
        return true;
    }
    if (top_1_suite == top_2_suite && top_1_suite != 0)
    {
        return true;
    }
    if (top_1_suite == top_3_suite && top_1_suite != 0)
    {
        return true;
    }
    if (top_2_suite == top_3_suite && top_2_suite != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool match_suite(int card_1, int card_2)
{
    if (get_suite(card_1) == get_suite(card_2) && get_suite(card_1) != 0 && get_suite(card_2) != 0)
    {
        // printf("card 1 is of color %d and card 2 is of color %d so they match!\n", get_suite(card_1), get_suite(card_2));
        //  printf("table rad 0:    %d  %d  %d  %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
        return true;
    }
    else
    {
        return false;
    }
}

bool only_one_stacks()
{
    int top_0 = find_top_card(0);
    int top_1 = find_top_card(1);
    int top_2 = find_top_card(2);
    int top_3 = find_top_card(3);
    if (get_top_card_position(top_0, 0) == 0 && get_top_card_position(top_1, 1) == 0 && get_top_card_position(top_2, 2) == 0 && get_top_card_position(top_3, 3) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool evaluate_result()
{
    if (table[1][0] == 0 && table[1][1] == 0 && table[1][2] == 0 && table[1][3] == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool clean_table() // ska rensa allt utan att flytta runt, hearts is 1, diamonds is 2, clubs is 3, spades is 4.
{
    int top_0 = find_top_card(0);
    int top_0_suite = get_suite(top_0);
    int top_1 = find_top_card(1);
    int top_1_suite = get_suite(top_1);
    int top_2 = find_top_card(2);
    int top_2_suite = get_suite(top_2);
    int top_3 = find_top_card(3);
    int top_3_suite = get_suite(top_3);

    while (need_clean(top_0_suite, top_1_suite, top_2_suite, top_3_suite) == true && only_one_stacks() == false) // kollar om cleaning needed
    {
        printf("Cleaning IS needed\n");
        if (match_suite(top_0, top_1)) // FIXA IF ELLER ELSE IF..
        {
            if (top_0 < top_1)
            {
                table[get_top_card_position(top_0, 0)][0] = 0;
                // printf("rensar\n");
                top_0 = 0;
            }
            else if (top_1 < top_0)
            {
                table[get_top_card_position(top_1, 1)][1] = 0;
                // printf("rensar\n");
                top_1 = 0;
            }
        }

        else if (match_suite(top_0, top_2))
        {
            if (top_0 < top_2)
            {
                table[get_top_card_position(top_0, 0)][0] = 0;
                top_0 = 0;
            }
            else if (top_2 < top_0)
            {
                table[get_top_card_position(top_2, 2)][2] = 0;
                top_2 = 0;
            }
        }

        else if (match_suite(top_0, top_3))
        {
            if (top_0 < top_3)
            {
                table[get_top_card_position(top_0, 0)][0] = 0;
                top_0 = 0;
            }
            else if (top_3 < top_0)
            {
                table[get_top_card_position(top_3, 3)][3] = 0;
                top_3 = 0;
            }
        }

        else if (match_suite(top_1, top_2))
        {
            if (top_1 < top_2)
            {
                table[get_top_card_position(top_1, 1)][1] = 0;
                top_1 = 0;
            }
            else if (top_2 < top_1)
            {
                table[get_top_card_position(top_2, 2)][2] = 0;
                top_2 = 0;
            }
        }

        else if (match_suite(top_1, top_3))
        {
            if (top_1 < top_3)
            {
                table[get_top_card_position(top_1, 1)][1] = 0;
                top_1 = 0;
            }
            else if (top_3 < top_1)
            {
                table[get_top_card_position(top_3, 3)][3] = 0;
                top_3 = 0;
            }
        }
        else if (match_suite(top_2, top_3))
        {
            if (top_2 < top_3)
            {
                table[get_top_card_position(top_2, 2)][2] = 0;
                top_2 = 0;
            }
            else if (top_3 < top_2)
            {
                table[get_top_card_position(top_3, 3)][3] = 0;
                top_3 = 0;
            }
        }
        top_0 = find_top_card(0);
        top_0_suite = get_suite(top_0);
        top_1 = find_top_card(1);
        top_1_suite = get_suite(top_1);
        top_2 = find_top_card(2);
        top_2_suite = get_suite(top_2);
        top_3 = find_top_card(3);
        top_3_suite = get_suite(top_3);

        printf("table rad 0:    %d  %d  %d  %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
        printf("table rad 1:    %d  %d  %d  %d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
        printf("table rad 2:    %d  %d  %d  %d\n", table[2][0], table[2][1], table[2][2], table[2][3]);
        printf("table rad 3:    %d  %d  %d  %d\n", table[3][0], table[3][1], table[3][2], table[3][3]);
    }

    // printf("cleaning is NOT needed!\n");
    return false;
}
bool check_empty_column()
{
    int top_0 = find_top_card(0);
    int top_1 = find_top_card(1);
    int top_2 = find_top_card(2);
    int top_3 = find_top_card(3);
    if (find_top_card(0) == 0 || find_top_card(1) == 0 || find_top_card(2) == 0 || find_top_card(3) == 0)
    {
        printf("Empty column exists!\n");
        return true;
    }
    else
        return false;
}

int what_empty_column() // kanske ska va bool
{
    int top_0 = find_top_card(0);
    int top_1 = find_top_card(1);
    int top_2 = find_top_card(2);
    int top_3 = find_top_card(3);

    if (top_0 == 0) // OM 0 så är raden tom?!
    {
        printf("column 0 is empty\n");
        return 0;
    }
    if (top_1 == 0) // OM 0 så är raden tom?!
    {
        printf("column 1 is empty\n");
        return 1;
    }
    if (top_2 == 0) // OM 0 så är raden tom?!
    {
        printf("column 2 is empty\n");
        return 2;
    }
    if (top_3 == 0) // OM 0 så är raden tom?!
    {
        printf("column 3 is empty\n");
        return 3;
    }
    return 4;

    // blablabla
}

void strategy_no_braniers() // HÄR KÖR VI NOG FAST ATM!
{
    // bool moved = true;
    for (int i = 0; i < 10; i++) // FIXA VETTIG LÖSNING FÖR HELVETE!
    {
        // printf("loop curse i strat_no_brainers?\n");
        int top = 0;
        int second = 0;
        for (int i = 0; i > 4; i++)
        {
            top = find_top_card(i);                      // hittar top i hög i
            int top_pos = get_top_card_position(top, i); // hittar positionen för det
            second = table[top_pos - 1][i];              // hittar kortet som ligger under
            if (match_suite(top, second))                // om de har samma färg...
            {
                table[top_pos][i] = 0; //...tar vi bort det översta (flyttar det till tom column och då försvinner det)
                // moved == true;
            }
            // else
            // {
            //     moved == false;
            // }
        }
    }
    return;
}

int count_cards()
{
    int count = 0;
    for (int c = 0; c < 4; c++)
    {
        for (int i = 0; i < 14; i++)
            if (table[i][c] != 0)
                count++;
    }
    return count;
}

void strategy_1() // Flyttar bara ner det högsta kortet till ledig plats.
{
    printf("table rad 0:    %d   %d   %d   %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
    printf("table rad 1:    %d   %d   %d   %d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
    printf("table rad 2:    %d   %d   %d   %d\n", table[2][0], table[2][1], table[2][2], table[2][3]);
    printf("table rad 3:    %d   %d   %d   %d\n", table[3][0], table[3][1], table[3][2], table[3][3]);
    printf("table rad 4:    %d   %d   %d   %d\n", table[4][0], table[4][1], table[4][2], table[4][3]);
    printf("table rad 5:    %d   %d   %d   %d\n", table[5][0], table[5][1], table[5][2], table[5][3]);
    printf("table rad 6:    %d   %d   %d   %d\n", table[6][0], table[6][1], table[6][2], table[6][3]);
    printf("table rad 7:    %d   %d   %d   %d\n", table[7][0], table[7][1], table[7][2], table[7][3]);
    int top_0 = find_top_card(0); // JA det här ska vara en funktion som hämtar array, det kan användas på fler ställen.
    int top_1 = find_top_card(1);
    int top_2 = find_top_card(2);
    int top_3 = find_top_card(3);
    int top_0_pos = get_top_card_position(top_0, 0);
    int top_1_pos = get_top_card_position(top_1, 1);
    int top_2_pos = get_top_card_position(top_2, 2);
    int top_3_pos = get_top_card_position(top_3, 3);
    int top_0_value = value_off(top_0);
    int top_1_value = value_off(top_1);
    int top_2_value = value_off(top_2);
    int top_3_value = value_off(top_3);

    if (top_0_pos != 0)
    {
        printf("top 0 pos är INTE 0!\n");
        if (top_0_value >= top_1_value || top_1_pos == 0)
        {
            printf("top 0 value är större än top 1 value ELLER top 1 pos är 0!\n");
            if (top_0_value >= top_2_value || top_2_pos == 0)
            {
                printf("top 0 value är större än top 2 value ELLER top 2 pos är 0!\n");
                if (top_0_value >= top_3_value || top_3_pos == 0)
                {
                    printf("top 0 value är större än top 3 value ELLER top 3 pos är 0!\n");
                    table[0][what_empty_column()] = top_0; // HOLY!
                    printf("kopierat top 0 till lediga platsen\n");
                    table[top_0_pos][0] = 0;
                    printf("tagit bort top 0 från där den låg!\n");
                    top_0 = find_top_card(0);
                    top_0_pos = get_top_card_position(top_0, 0);
                    top_0_value = value_off(top_0);
                }
            }
        }
    }
    if (check_empty_column() && top_1_pos != 0)
    {
        printf("top 0 pos är INTE 0!\n");
        if (top_1_value >= top_0_value || top_0_pos == 0)
        {
            printf("top 0 value är större än top 1 value ELLER top 1 pos är 0!\n");
            if (top_1_value >= top_2_value || top_2_pos == 0)
            {
                printf("top 0 value är större än top 2 value ELLER top 2 pos är 0!\n");
                if (top_1_value >= top_3_value || top_3_pos == 0)
                {
                    printf("top 0 value är större än top 3 value ELLER top 3 pos är 0!\n");
                    table[0][what_empty_column()] = top_1; // HOLY!
                    printf("kopierat top 0 till lediga platsen\n");
                    table[top_1_pos][1] = 0;
                    printf("tagit bort top 0 från där den låg!\n");
                    top_1 = find_top_card(0);
                    top_1_pos = get_top_card_position(top_0, 0);
                    top_1_value = value_off(top_0);
                }
            }
        }
    }
    if (check_empty_column() && top_2_pos != 0)
    {
        if (top_2_value >= top_0_value || top_0_pos == 0)
        {
            if (top_2_value >= top_1_value || top_1_pos == 0)
            {
                if (top_2_value >= top_3_value || top_3_pos == 0)
                {
                    table[0][what_empty_column()] = top_2; // HOLY!
                    table[top_2_pos][2] = 0;
                    top_2 = find_top_card(0);
                    top_2_pos = get_top_card_position(top_0, 0);
                    top_2_value = value_off(top_0);
                }
            }
        }
    }
    if (check_empty_column() && top_3_pos != 0)
    {
        if (top_3_value > top_0_value || top_0_pos == 0)
        {
            if (top_3_value > top_1_value || top_1_pos == 0)
            {
                if (top_3_value > top_2_value || top_2_pos == 0)
                {
                    table[0][what_empty_column()] = top_3; // HOLY!
                    table[top_3_pos][3] = 0;
                    top_3 = find_top_card(0);
                    top_3_pos = get_top_card_position(top_0, 0);
                    top_3_value = value_off(top_0);
                }
            }
        }
    }
    printf("table rad 0:    %d   %d   %d   %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
    printf("table rad 1:    %d   %d   %d   %d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
    printf("table rad 2:    %d   %d   %d   %d\n", table[2][0], table[2][1], table[2][2], table[2][3]);
    printf("table rad 3:    %d   %d   %d   %d\n", table[3][0], table[3][1], table[3][2], table[3][3]);
    printf("table rad 4:    %d   %d   %d   %d\n", table[4][0], table[4][1], table[4][2], table[4][3]);
    printf("table rad 5:    %d   %d   %d   %d\n", table[5][0], table[5][1], table[5][2], table[5][3]);
    printf("table rad 6:    %d   %d   %d   %d\n", table[6][0], table[6][1], table[6][2], table[6][3]);
    printf("table rad 7:    %d   %d   %d   %d\n", table[7][0], table[7][1], table[7][2], table[7][3]);
}

void strategy_2() // Flyttar ner det kort som ligger i den hög med flest olika instances av olika färger(eller bara flest olika färger(eller bara flest kort?))
{
}

void strategy_3() // strategi som lägger ner det kort som leder till minst antal kort på bordet efter cleanups etc
{
    printf("APPLYING STRAT 3 WATCH OUT!!!\n");
    strategy_no_braniers();
    int empty = what_empty_column();
    if (empty == 4)
    {
        return;
    }
    int count_if_moved_0 = 0;
    int count_if_moved_1 = 0;
    int count_if_moved_2 = 0;
    int count_if_moved_3 = 0;
    int table_backup_0[13][4];
    int table_backup_1[13][4];
    int table_backup_2[13][4];
    int table_backup_3[13][4];

    for (int i = 0; i < 4; i++)
    {

        int top = (find_top_card(i));
        if (top == 0) // Då är den tom och top kort kan således inte flyttas ner.
        {
            continue;
        }
        table[get_top_card_position(top, i)][i] = 0;
        if (get_top_card_position(top, i) == 0) // Om den inte är noll men på position noll kan den inte heller flyttas ner, eller det ger iaf inget. Loopade här förut...
        {
            continue;
        }
        table[0][empty] = top;                                                                                                              // Stoppar top i nuvarande iteration i en tom column.
        if (need_clean(get_suite(find_top_card(1)), get_suite(find_top_card(2)), get_suite(find_top_card(3)), get_suite(find_top_card(4)))) // behöver vi städa nu? Om ja, städa och sen ner i kaninhålet.....
        {
            clean_table();
            strategy_3();
        }
        else // Om vi inte behövde städa mer vid något tillfälle, kolla vilket i som gav minst kort på bordet, men den här är ju nu i nån deep recursion galore?
        {
            if (i == 0)
            {
                count_if_moved_0 = count_cards();
                printf("om top_0 flyttades blev count %d\n", count_if_moved_0);
                memcpy(table_backup_0, table, sizeof(table_backup_0)); // backup table state
            }
            else if (i == 1)
            {
                count_if_moved_1 = count_cards();
                printf("om top_1 flyttades blev count %d\n", count_if_moved_1);
                memcpy(table_backup_1, table, sizeof(table_backup_1)); // backup table state
            }
            else if (i == 2)
            {
                count_if_moved_2 = count_cards();
                printf("om top_2 flyttades blev count %d\n", count_if_moved_2);
                memcpy(table_backup_2, table, sizeof(table_backup_2)); // backup table state
            }
            else if (i == 3)
            {
                count_if_moved_3 = count_cards();
                printf("om top_3 flyttades blev count %d\n", count_if_moved_3);
                memcpy(table_backup_3, table, sizeof(table_backup_3)); // backup table state
            }
            return;
        }
    }

    if (count_if_moved_0 <= count_if_moved_1 && count_if_moved_0 <= count_if_moved_2 && count_if_moved_0 <= count_if_moved_3)
    {
        memcpy(table, table_backup_0, sizeof(table)); // restore table state
    }
    else if (count_if_moved_1 <= count_if_moved_0 && count_if_moved_1 <= count_if_moved_2 && count_if_moved_1 <= count_if_moved_3)
    {
        memcpy(table, table_backup_1, sizeof(table)); // restore table state
    }
    else if (count_if_moved_2 <= count_if_moved_0 && count_if_moved_2 <= count_if_moved_1 && count_if_moved_2 <= count_if_moved_3)
    {
        memcpy(table, table_backup_2, sizeof(table)); // restore table state
    }
    else if (count_if_moved_3 <= count_if_moved_0 && count_if_moved_3 <= count_if_moved_1 && count_if_moved_3 <= count_if_moved_2)
    {
        memcpy(table, table_backup_3, sizeof(table)); // restore table state
    }
}

void apply_strategy(int strategy)
{
    if (strategy == 1)
    {
        strategy_1();
    }
    else if (strategy == 3)
    {
        strategy_3();
    }
}

int play(int n_games, int strategy) // plays the game n_games times med strategy
{
    int played = 0;
    int successes = 0;
    for (int n = 0; n < n_games; n++)
    {

        populate_deck();
        populate_table();
        for (int i = 0; i < 13; i++) // 13 varv använder upp hela leken och spelet är slut sen.
        {
            // printf("drar 4\n");
            draw_four();
            // printf("table rad 0:  %d  %d  %d  %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
            clean_table();
            printf("kommer vi hit?\n");
            while (check_empty_column() == true && only_one_stacks() == false)
            {
                printf("table rad 0:    %d   %d   %d   %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
                printf("table rad 1:    %d   %d   %d   %d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
                printf("table rad 2:    %d   %d   %d   %d\n", table[2][0], table[2][1], table[2][2], table[2][3]);
                printf("table rad 3:    %d   %d   %d   %d\n", table[3][0], table[3][1], table[3][2], table[3][3]);
                printf("table rad 4:    %d   %d   %d   %d\n", table[4][0], table[4][1], table[4][2], table[4][3]);
                printf("table rad 5:    %d   %d   %d   %d\n", table[5][0], table[5][1], table[5][2], table[5][3]);
                printf("table rad 6:    %d   %d   %d   %d\n", table[6][0], table[6][1], table[6][2], table[6][3]);
                printf("table rad 7:    %d   %d   %d   %d\n", table[7][0], table[7][1], table[7][2], table[7][3]);
                printf("curse?\n");
                strategy_no_braniers();
                printf("curse 2?\n");
                apply_strategy(strategy); // Den här gör väl bara strategy_1 nu?
                clean_table();
                printf("curse of the while\n");
                printf("table rad 0:    %d   %d   %d   %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
                printf("table rad 1:    %d   %d   %d   %d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
                printf("table rad 2:    %d   %d   %d   %d\n", table[2][0], table[2][1], table[2][2], table[2][3]);
                printf("table rad 3:    %d   %d   %d   %d\n", table[3][0], table[3][1], table[3][2], table[3][3]);
                printf("table rad 4:    %d   %d   %d   %d\n", table[4][0], table[4][1], table[4][2], table[4][3]);
                printf("table rad 5:    %d   %d   %d   %d\n", table[5][0], table[5][1], table[5][2], table[5][3]);
                printf("table rad 6:    %d   %d   %d   %d\n", table[6][0], table[6][1], table[6][2], table[6][3]);
                printf("table rad 7:    %d   %d   %d   %d\n", table[7][0], table[7][1], table[7][2], table[7][3]);
            }
        }
        played++;
        if (evaluate_result() == true)
        {
            printf("SUCCESS!\n");
            successes++;
            printf("table rad 0:  %d  %d  %d  %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
            printf("table rad 1:  %d  %d  %d  %d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
        }
        else
        {
            printf("FAIL! \n");
        }
    }

    double success_rate = successes / n_games;
    printf("Played %d games...\n", played);
    printf("Number of successes %d\n", successes);
    printf("Success rate %f\n", success_rate * 1000000);
}

// funktioner slutar här

void main()
{
    srand(time(NULL)); // seeds with current time
    int n_games_exponent = 0;
    int strategy;
    printf("What strategy would you like to try?\n");
    printf("1. Highest card gets put on empty column\n");
    printf("2. n/a yet\n");
    printf("3. Card that results in fewest cards on table gets put on empty column\n");
    scanf("%d", &strategy);
    printf("How many games? 10 raised to input will be played.\n");
    scanf("%d", &n_games_exponent);
    int n_games = pow(10, n_games_exponent);
    printf("Playing %d games with strategy %d\n", n_games, strategy);
    clock_t start = clock();
    play(n_games, strategy);
    clock_t end = clock();
    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("It took %f seconds\n", time_spent);

    // play(1, 1);

    // play(1,1);
    //  populate_deck();
    //  populate_table();
    //  draw_four();

    // // manupulera:
    // table[0][0] = 2;
    // table[0][1] = 5;

    // printf("table rad 0:    %d  %d  %d  %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);
    // printf("table rad 1:    %d  %d  %d  %d\n", table[1][0], table[1][1], table[1][2], table[1][3]);
    // printf("table rad 2:    %d  %d  %d  %d\n", table[2][0], table[2][1], table[2][2], table[2][3]);
    // printf("table rad 3:    %d  %d  %d  %d\n", table[3][0], table[3][1], table[3][2], table[3][3]);
    // printf("table rad 12:   %d  %d  %d  %d\n", table[12][0], table[12][1], table[12][2], table[12][3]);

    // clean_table();
    // check_empty_column();
    // what_empty_column();

    // printf("table rad 0:    %d  %d  %d  %d\n", table[0][0], table[0][1], table[0][2], table[0][3]);

    // int yeah = find_top_card(0);
    // printf("top is %d\n", yeah);
}