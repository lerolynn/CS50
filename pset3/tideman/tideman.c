#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // If name is a match for the name of a valid candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            // Update the ranks array
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Add voter preferences based on rank to preferences table
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

int compare(const void *a, const void *b)
{
    // https://stackoverflow.com/questions/6105513/need-help-using-qsort-with-an-array-of-structs
    pair *pairA = (pair *)a;
    pair *pairB = (pair *)b;

    return (preferences[pairB->winner][pairB->loser] - preferences[pairA->winner][pairA->loser]);
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Use stdlib's quicksort
    qsort(pairs, pair_count, sizeof(pair), compare);
    return;
}

// Check if there is a cycle
bool cycle_exists(int loser, int count)
{
    // If recursion continues past candidate count, a cycle definitely has occurred
    if (count == candidate_count)
    {
        return true;
    }

    // Voters prefer [loser] over [i]
    for (int i = 0; i < candidate_count; i++)
    {
        // Loop through every i to see if any element in row is true
        if (locked[loser][i])
        {
            // Recurse up if element has been set to true before, setting current winner to loser
            return cycle_exists(i, count + 1);
        }
    }

    // If the loop ends, none of the recursive calls have returned true, so no cycle exists
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Loop through pair list
    for (int i = 0; i < pair_count; i++)
    {
        // Add pair to table
        locked[pairs[i].winner][pairs[i].loser] = true;

        // Remove element from table if cycle exists
        if (cycle_exists(pairs[i].loser, 0))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }

    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // If candidate won, nobody prefers someone else over this candidate, column will be all false
    for (int i = 0; i < candidate_count; i++)
    {
        // Assume that candidate won
        bool candidate_won = true;

        // Go down column to find if anyone beat the candidate
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                candidate_won = false;
            }
        }

        // Print the candidate's name if nobody beat the candidate
        if (candidate_won)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

