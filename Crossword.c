#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10

typedef struct _point
{
    int row;
    int col;

}point_t;

typedef struct _slot
{
    point_t s;
    point_t e;
    const char *word;

}slot_t;

static slot_t g_slot_tab[N] = { 0, };
static int g_slot_tab_index = 0;
static char g_common_input[N][N + 1];
static char g_input_words[N][N + 1] = { 0, };
static int g_input_word_used[N] = { 0, };

void build_crossword_slot_table(char input[N][N + 1]);
int slot_cross_check(slot_t b1, slot_t b2, const char* word1,                               const char* word2);
int get_input();
int is_safe_to_put_word(slot_t b, const char *word, int                                         word_used_index, int slot_row);
void place_word(slot_t b, const char *word, int       
word_used_index, int slot_row);
int revert_word(int word_used_index, int slot_row);
int solve_crossword_util();
int solve_crossword();
void print_output();

int main()
{
    solve_crossword();
    return 0;
}

int solve_crossword_util(int n, int slot_row)
{
    if (n == slot_row)
    {
        return 0;
    }

    for (int i = 0; i < n; ++i)
    {
        if (1 == is_safe_to_put_word(g_slot_tab[slot_row], 
                                     &g_input_words[i][0], 
                                     i, 
                                     slot_row))
        {
            place_word(g_slot_tab[slot_row], 
                       &g_input_words[i][0], 
                       i, 
                       slot_row);

            if (0 == solve_crossword_util(n, slot_row + 1))
            {
                return 0;
            }

            revert_word(i, slot_row);
        }
    }

    return -1;
}

int solve_crossword()
{
    int n = get_input();

    build_crossword_slot_table(g_common_input);

    solve_crossword_util(n, 0);

    print_output();

    return 0;
}

int is_safe_to_put_word(slot_t b, const char *word, 
                          int word_used_index, int slot_row)
{
    if (b.s.row == b.e.row)
    {
        if (b.e.col - b.s.col + 1 != strlen(word) ||    
            g_input_word_used[word_used_index] == 1)
        {
            return 0;
        }
    }
    else
    {
        if (b.e.row - b.s.row + 1 != strlen(word) || 
            g_input_word_used[word_used_index] == 1)
        {
            return 0;
        }
    }

    for (int row = 0; row < slot_row; ++row)
    {
        int ret = slot_cross_check(g_slot_tab[row], 
                                   g_slot_tab[slot_row], 
                                   g_slot_tab[row].word, 
                                   word);
        if (-1 == ret)
        {
            return 0;
        }
    }

    return 1;
}

void place_word(slot_t b, const char *word, int 
                word_used_index, int slot_row)
{
    if (b.s.row == b.e.row)
    {
        for (int i = b.s.col; i <= b.e.col; ++i)
        {
            // output is filled input buffer to save space
            g_common_input[b.s.row][i] = word[i - b.s.col];
        }
    }
    else
    {
        for (int i = b.s.row; i <= b.e.row; ++i)
        {
            // output is filled input buffer to save space
            g_common_input[i][b.s.col] = word[i - b.s.row];
        }
    }

    g_input_word_used[word_used_index] = 1;
    g_slot_tab[slot_row].word = 
                         &g_input_words[word_used_index][0];

    return;
}

int revert_word(int word_used_index, int slot_row)
{
    g_input_word_used[word_used_index] = 0;
    g_slot_tab[slot_row].word = 0;

    return 0;
}

void build_crossword_slot_table(char input[N][N + 1])
{
    // count by row
    for (int row = 0; row < N; ++row)
    {
        int counting = 0;
        int start_col = -1, end_col = -1;

        for (int col = 0; col < N; ++col)
        {
            if (input[row][col] == '+')
            {
                if (counting == 1)
                {
                    if (end_col - start_col > 1)
                    {
                        //register
                        g_slot_tab[g_slot_tab_index].s.row = 
                                                         row;
                        g_slot_tab[g_slot_tab_index].s.col = 
                                               start_col + 1;
                        g_slot_tab[g_slot_tab_index].e.row = 
                                                         row;
                        g_slot_tab[g_slot_tab_index].e.col = 
                                                     end_col;

                        ++g_slot_tab_index;
                    }
                }

                counting = 0;
                end_col = start_col = col;
            }
            else
            {
                counting = 1;
                end_col = col;
            }
        }

        if (counting == 1)
        {
            end_col = N - 1;

            if (end_col - start_col > 1)
            {
                //register
                g_slot_tab[g_slot_tab_index].s.row = row;
                g_slot_tab[g_slot_tab_index].s.col = 
                                               start_col + 1;
                g_slot_tab[g_slot_tab_index].e.row = row;
                g_slot_tab[g_slot_tab_index].e.col = end_col;
                ++g_slot_tab_index;
            }
        }
    }

    // count by column
    for (int col = 0; col < N; ++col)
    {
        int counting = 0;
        int start_row = -1, end_row = -1;

        for (int row = 0; row < N; ++row)
        {
            if (input[row][col] == '+')
            {
                if (counting == 1)
                {
                    if (end_row - start_row > 1)
                    {
                        //register
                        g_slot_tab[g_slot_tab_index].s.row = 
                                               start_row + 1;
                        g_slot_tab[g_slot_tab_index].s.col =                                                              col;
                        g_slot_tab[g_slot_tab_index].e.row = 
                                                     end_row;
                        g_slot_tab[g_slot_tab_index].e.col = 
                                                         col;
                        ++g_slot_tab_index;
                    }
                }

                counting = 0;
                end_row = start_row = row;
            }
            else
            {
                counting = 1;
                end_row = row;
            }
        }

        if (counting == 1)
        {
            end_row = N - 1;
            if (end_row - start_row > 1)
            {
                //register
                g_slot_tab[g_slot_tab_index].s.row = 
                                             start_row + 1;
                g_slot_tab[g_slot_tab_index].s.col = col;
                g_slot_tab[g_slot_tab_index].e.row = end_row;
                g_slot_tab[g_slot_tab_index].e.col = col;
                ++g_slot_tab_index;
            }
        }
    }
}

int slot_cross_check(slot_t b1, slot_t b2, const char* word1,                                                const char* word2)
{
    if ((b1.s.row == b1.e.row && b2.s.col == b2.e.col &&  
         b1.s.col <= b2.s.col && b1.e.col >= b2.e.col && 
         b1.s.row >= b2.s.row && b1.e.row <= b2.e.row) ||
        (b2.s.row == b2.e.row && b1.s.col == b1.e.col && 
         b2.s.col <= b1.s.col && b2.e.col >= b1.e.col && 
         b2.s.row >= b1.s.row && b2.e.row <= b1.e.row))
    {
        char cross_char1, cross_char2;

        if (b1.s.row == b1.e.row)
        {
            cross_char1 = word1[b2.s.col - b1.s.col];
            cross_char2 = word2[b1.s.row - b2.s.row];
        }
        else
        {
            cross_char1 = word1[b1.s.col - b2.s.col];
            cross_char2 = word2[b2.s.row - b1.s.row];
        }

        if (cross_char1 == cross_char2)
        {
            return 0;
        }
        else
        {
            return -1;
        }

    }
    else
    {
        return 1;
    }
}

int get_input()
{
    for (int row = 0; row < N; ++row)
    {
        for (int col = 0; col < N + 1; ++col)
        {
            scanf("%c", &g_common_input[row][col]);
            if (g_common_input[row][col] == '\n')
            {
                g_common_input[row][col] = '\0';
            }
        }
    }

    char c;

    int row = 0;
    int col = 0;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (c != ';')
        {
            g_input_words[row][col] = c;
            ++col;
        }
        else
        {
            col = 0;
            ++row;
        }
    }

    g_input_words[row][col] = '\0';

    ++row;

    return row;
}

void print_output()
{
    for (int row = 0; row < N; ++row)
    {
        for (int col = 0; col < N; ++col)
        {
            printf("%c", g_common_input[row][col]);
        }

        printf("\n");
    }
}
