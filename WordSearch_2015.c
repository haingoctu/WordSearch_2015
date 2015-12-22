/*
*   Hai Tu
*   Word Search 
*	December 2015.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WordSearch.h"

int readDictionary(Dictionary *dict);
void destroyDictionary(Dictionary *dict);
int readPuzzle(WordSearchPuzzle *puzzle);
void destroyPuzzle(WordSearchPuzzle *puzzle);
int checkString(Dictionary *dict, char *str);

/*My supplemental Functions*/
void checkRow (WordSearchPuzzle *puzzle, Dictionary *dict);
void checkRow_forward(char word_builder, WordSearchPuzzle *puzzle,
                        Dictionary *dict,int marked_row, int marked_col);
void checkRow_backward(char word_builder, WordSearchPuzzle *puzzle,
                        Dictionary *dict, int marked_row, int marked_col);
void checkCol(WordSearchPuzzle *puzzle, Dictionary *dict);
void checkCol_forward(char word_builder, WordSearchPuzzle *puzzle,
                        Dictionary *dict, int marked_row, int marked_col);
void checkCol_backward(char word_builder, WordSearchPuzzle *puzzle,
                        Dictionary *dict, int marked_row, int marked_col);
void checkDiagRight(WordSearchPuzzle *puzzle, Dictionary *dict);
void checkDiagRight_forward(char word_builder, WordSearchPuzzle *puzzle,
                            Dictionary *dict, int marked_row, int marked_col);
void checkDiagRight_backward(char word_builder, WordSearchPuzzle *puzzle,
                            Dictionary *dict, int marked_row, int marked_col);
void checkDiagLeft(WordSearchPuzzle *puzzle, Dictionary *dict);
void checkDiagLeft_forward(char word_builder, WordSearchPuzzle *puzzle,
                            Dictionary *dict, int marked_row, int marked_col);
void checkDiagLeft_backward(char word_builder, WordSearchPuzzle *puzzle,
                            Dictionary *dict,int marked_row, int marked_col);
void results(Dictionary *dict);

int main(void)
{
    Dictionary *dict = NULL;
    WordSearchPuzzle *puzzle = NULL;

    puzzle = malloc(sizeof(WordSearchPuzzle));
    dict = calloc(1, sizeof(Dictionary));

    readDictionary(dict);//open dictionary file
    readPuzzle(puzzle);//generate puzzle

    /*search for words in the puzzle that match the ones in our dictionary*/
    checkRow(puzzle, dict);//checks each row
    checkCol(puzzle, dict);//check each col
    checkDiagRight(puzzle, dict);//check diagonal right to left
    checkDiagLeft(puzzle, dict);//check diagonal left to right
    results(dict);//print words found and how many times it was found

    /*free the dynamically allocated memory (structures)*/
    destroyDictionary(dict);
    destroyPuzzle(puzzle);

    return 0;
}

/*
*   return 1 if we succesfully opened dictionary, malloced what we needed
*   and copied the words to memory. otherwise return 0 due to failure of malloc
*   or file was not opened
*/
int readDictionary(Dictionary *dict)
{
    int i, len = 0, fail = 0;
    char buffer[MAX_WORD_LENGTH+1];

    dict->counts = NULL;
    dict->words = NULL;

    /*Open dictionary*/
    FILE *ifp = fopen("dictionary.txt", "r");
    if (ifp == NULL)
        return 0;

    /*
    *   read in the quantity of words, then allocate memory for that many
    *   words as well a tracker(dict->counts) for each word
    */
    fscanf(ifp, "%d", &dict->size);
    dict->words = malloc(sizeof(char*) * dict->size);
    if (dict->words == NULL | dict->size == 0 )
        fail++;

    dict->counts = malloc(sizeof(int) * dict->size);
    if (dict->counts == NULL)
        fail++;

    /*zero out each counts element*/
    for (i = 0; i < dict->size; i++)
        dict->counts[i] = 0;

    /*generate our dictionary*/
    i = 0;
    while (fscanf(ifp, "%s", buffer) != EOF)
    {
            len = strlen(buffer);//determine length of word
            dict->words[i] = malloc(sizeof(char) * (len + 1) );//allocate mem
            strcpy(dict->words[i], buffer);//copy word from file to memory
            if (dict->words[i] == NULL)
                fail++;
            i++;//next word
    }

    fclose(ifp);//close the file

    /*if any mallocs failed, reset everything and return 0*/
    if (fail != 0)
    {
        destroyDictionary(dict);
        printf("Dictionary Read Failed. Beware I may Crash.\n");
        return 0; 
        
    }

    return 1;
}

/*
*   free all the mallocs within struct dict as well as the struct itself,
*   NULL the pointers and zero out int.
*/
void destroyDictionary(Dictionary *dict)
{
    int i;
    for (i = 0; i < dict->size; i++)
    {
        free(dict->words[i]);
        dict->words[i] = NULL;
    }
    free(dict->words);
    dict->words = NULL;
    free(dict->counts);
    dict->counts = NULL;
    dict->size = 0;
    free(dict);
    dict = NULL;
}

/*
*   return 1 if we succesfully read in the puzzle, malloced what we needed
*   otherwise return 0 due to failure of malloc
*/
int readPuzzle(WordSearchPuzzle *puzzle)
{
    int i, fail = 0;
    puzzle->grid = NULL;

    /*read in dimensions of puzzle, and generate the "rows"*/
    scanf("%d", &puzzle->height);
    scanf("%d", &puzzle->width);
    puzzle->grid = malloc(sizeof(char *) * puzzle->height);
    if (puzzle->grid == NULL)
        fail++;

    /*malloc space for each string ("cols") and copy users string*/
    for (i = 0; i < puzzle->height; i++)
    {
        puzzle->grid[i] = malloc(sizeof(char) * (puzzle->width + 1));
        scanf("%s", puzzle->grid[i]);
        if (puzzle->grid[i] == NULL)
            fail++;
    }

    /*if any mallocs failed, reset everything and return 0*/
    if (fail != 0)
    {
        destroyPuzzle(puzzle);
        return 0;
    }

    return 1;
}

/*
*   free all the mallocs within struct puzzle as well as the struct itself,
*   NULL the pointers, and zero out ints.
*/
void destroyPuzzle(WordSearchPuzzle *puzzle)
{
    int i;
    for (i = 0; i < puzzle->height; i++)
    {
        free(puzzle->grid[i]);
        puzzle->grid[i] = NULL;
    }
    free(puzzle->grid);
    puzzle->grid = NULL;
    puzzle->height = 0;
    puzzle->width = 0;
    free(puzzle);
}

/*
*   return 1 if word was found in our dictionary, othewise return 0.
*   this function is based on Dr. Szumlanski's binary search function.
*/
int checkString(Dictionary *dict, char *str)
{
    int max = dict->size - 1, min = 0, mid;
    while (max >= min)
    {
        mid = min + (max-min) / 2;
        if ( strcmp(str, dict->words[mid]) < 0)
        {
            max = mid - 1;
        }
        else if ( strcmp(str, dict->words[mid]) > 0)
        {
            min = mid + 1 ;
        }
        else
        {
            dict->counts[mid]++;
            return 1;
        }
    }

    return 0;
}

/*
*   check each char going vertically (row) of the puzzle for valid words
*/
void checkRow (WordSearchPuzzle *puzzle, Dictionary *dict)
{
    char word_builder;
    int i, j, marked_row, marked_col;

    /*run through each each letter in our grid*/
    for (i = 0; i < puzzle->height; i++)
    {
        j = 0;
        while ( j < puzzle->width)
        {
            word_builder = puzzle->grid[i][j];//copy char to buffer
            marked_row = i;//track char's row
            marked_col = j;//track char's col
            /*check this letter forward and backwards to see if it
            generates a word*/
            checkRow_forward(word_builder, puzzle, dict,
                                marked_row, marked_col);
            checkRow_backward(word_builder, puzzle, dict,
                                marked_row, marked_col);
            j++;
        }
    }
}

/*
*   take a char from checkRow, build a string with neigboring chars
*   relative to the direction we are checking and checking each build if
*   it's a valid dictionary word. Note: this is the only place where we check
*   for single letter strings.
*/
void checkRow_forward(char word_builder, WordSearchPuzzle *puzzle,
                        Dictionary *dict, int marked_row, int marked_col)
{
    int i = 0;
    char *string_builder = malloc(sizeof(char) * (puzzle->width + 1) );

    /*build string one char at a time and checking if it's a word*/
    while (marked_col < puzzle->width )
    {
        string_builder[i] = puzzle->grid[marked_row][marked_col];
        string_builder[i+1] = '\0';
        checkString(dict, string_builder);
        i++;
        marked_col++;
    }

    /*clean up*/
    free(string_builder);
    string_builder = NULL;
}

/*
*   take a char from checkRow, build a string with neigboring chars
*   relative to the direction we are checking and checking each build if
*   it's a valid dictionary word.
*/
void checkRow_backward(char word_builder, WordSearchPuzzle *puzzle,
                        Dictionary *dict, int marked_row, int marked_col)
{
    int i = 0;
    char *string_builder = malloc(sizeof(char) * (puzzle->width + 1) );

    /*
    *   pre generating string to account for the single letter checks
    *   we already did. we start the string as two chars.
    */
    string_builder[i] = puzzle->grid[marked_row][marked_col];
    marked_col--;
    i++;

    /*build string one char at a time and checking if it's a word*/
    while (marked_col >= 0 )
    {
        string_builder[i] = puzzle->grid[marked_row][marked_col];
        string_builder[i+1] = '\0';
        checkString(dict, string_builder);
        i++;
        marked_col--;
    }

    /*clean up*/
    free(string_builder);
    string_builder = NULL;
}

/*
*   check each char going horizontally (col) of the puzzle for valid words
*/
void checkCol(WordSearchPuzzle *puzzle, Dictionary *dict)
{
    char word_builder;
    int i, j, marked_row, marked_col;

    /*run through each each letter in our grid*/
    for (i = 0; i < puzzle->width; i++)
    {
        j = 0;
        while ( j < puzzle->height )
        {
            word_builder = puzzle->grid[j][i];//copy char to buffer
            marked_row = j;//track char's row
            marked_col = i;//track char's col
            /*check this letter up(forward) and down(backward) to see if it
            generates a word*/
            checkCol_forward(word_builder, puzzle, dict,
                                marked_row, marked_col);
            checkCol_backward(word_builder, puzzle, dict,
                                marked_row, marked_col);
            j++;
        }
    }
}

/*
*   take a char from checkCol, build a string with neigboring chars
*   relative to the direction we are checking and checking each build if
*   it's a valid dictionary word.
*/
void checkCol_forward(char word_builder, WordSearchPuzzle *puzzle,
                        Dictionary *dict, int marked_row, int marked_col)
{
    int i = 0;
    //allocate memory large enough for greatest string in our puzzle
    char *string_builder = malloc(sizeof(char) * (puzzle->height + 1) );

    /*  pre generating string to account for the single letter checks
    *   we already did. we start the string as two chars now.
    */
    string_builder[i] = puzzle->grid[marked_row][marked_col];
    marked_row++;
    i++;

    /*build string one char at a time and checking if it's a word*/
    while (marked_row < puzzle->height )
    {
        string_builder[i] = puzzle->grid[marked_row][marked_col];
        string_builder[i+1] = '\0';
        checkString(dict, string_builder);
        i++;
        marked_row++;
    }

    /*clean up*/
    free(string_builder);
    string_builder = NULL;

}

/*
*   take a char from checkCol, build a string with neigboring chars
*   relative to the direction we are checking and checking each build if
*   it's a valid dictionary word.
*/
void checkCol_backward(char word_builder, WordSearchPuzzle *puzzle,
                        Dictionary *dict, int marked_row, int marked_col)
{
    int i = 0;
    //allocate memory large enough for greatest string in our puzzle
    char *string_builder = malloc(sizeof(char) * (puzzle->height + 1) );

    /*  pre generating string to account for the single letter checks
    *   we already did. we start the string as two chars now.
    */
    string_builder[i] = puzzle->grid[marked_row][marked_col];
    marked_row--;
    i++;

    /*build string one char at a time and checking if it's a word*/
    while (marked_row >= 0 )
    {
        string_builder[i] = puzzle->grid[marked_row][marked_col];
        string_builder[i+1] = '\0';
        checkString(dict, string_builder);
        i++;
        marked_row--;
    }

    /*clean up*/
    free(string_builder);
    string_builder = NULL;

}

/*
*   check each char going diagonally (going right) of the puzzle for valid words
*/
void checkDiagRight(WordSearchPuzzle *puzzle, Dictionary *dict)
{
    char word_builder;
    int i, j, marked_row, marked_col;

    /*run through each each letter in our grid*/
    for (i = 0; i < puzzle->height; i++)
    {
        j = 0;
        while ( j < puzzle->width )
        {
            word_builder = puzzle->grid[i][j];//copy char to buffer
            marked_row = i;//track char's row
            marked_col = j;//track char's col
            /*check this letter forward and backwards to see if it
            generates a word*/
            checkDiagRight_forward(word_builder, puzzle, dict, marked_row,
                                    marked_col);
            checkDiagRight_backward(word_builder, puzzle, dict, marked_row,
                                    marked_col);
            j++;
        }
    }
}

/*
*   take a char from checkDiagRight, build a string with neigboring chars
*   relative to the direction we are checking and checking each build if
*   it's a valid dictionary word.
*/
void checkDiagRight_forward(char word_builder, WordSearchPuzzle *puzzle,
                            Dictionary *dict, int marked_row, int marked_col)
{
    int i = 0;
    //allocate memory large enough for greatest string in our puzzle
    char *string_builder = malloc(sizeof(char) * (puzzle->height + 1) );

    /*
    *   pre generating string to account for the single letter checks
    *   we already did. we start the string as two chars.
    */
    string_builder[i] = puzzle->grid[marked_row][marked_col];
    marked_row++;
    marked_col++;
    i++;

    /*build string one char at a time and checking if it's a word*/
    while ( marked_row < puzzle->height && marked_col < puzzle->width )
    {
        string_builder[i] = puzzle->grid[marked_row][marked_col];
        string_builder[i+1] = '\0';
        checkString(dict, string_builder);
        //shift diagonally
        marked_row++;
        marked_col++;
        i++;
    }

    /*clean up*/
    free(string_builder);
    string_builder = NULL;

}

/*
*   take a char from checkDiagRight, build a string with neigboring chars
*   relative to the direction we are checking and checking each build if
*   it's a valid dictionary word.
*/
void checkDiagRight_backward(char word_builder, WordSearchPuzzle *puzzle,
                            Dictionary *dict, int marked_row, int marked_col)
{
    int i = 0;
    char *string_builder = malloc(sizeof(char) * (puzzle->height + 1) );

    /*
    *   pre generating string to account for the single letter checks
    *   we already did. we start the string as two chars.
    */
    string_builder[i] = puzzle->grid[marked_row][marked_col];
    marked_row--;
    marked_col--;
    i++;

    /*build string one char at a time and checking if it's a word*/
    while ( marked_row >= 0 && marked_col >= 0 )
    {
        string_builder[i] = puzzle->grid[marked_row][marked_col];
        string_builder[i+1] = '\0';
        checkString(dict, string_builder);
        //shift diagonally
        marked_row--;
        marked_col--;
        i++;
    }

    /*clean up*/
    free(string_builder);
    string_builder = NULL;
}
/*
*   check each char going diagonally (going left) of the puzzle for valid words
*/
void checkDiagLeft(WordSearchPuzzle *puzzle, Dictionary *dict)
{
    char word_builder;
    int i, j, marked_row, marked_col;

    /*run through each each letter in our grid*/
    for (i = 0; i < puzzle->height; i++)
    {
        j = 0;
        while ( j < puzzle->width )
        {
            word_builder = puzzle->grid[i][j];//copy char to buffer
            marked_row = i;//track char's row
            marked_col = j;//track char's col
            /*check this letter forward and backwards to see if it
            generates a word*/
            checkDiagLeft_forward(word_builder, puzzle, dict, marked_row,
                                    marked_col);
            checkDiagLeft_backward(word_builder, puzzle, dict, marked_row,
                                    marked_col);
            j++;
        }
    }
}

/*
*   take a char from checkDiagLeft, build a string with neigboring chars
*   relative to the direction we are checking and checking each build if
*   it's a valid dictionary word.
*/
void checkDiagLeft_forward(char word_builder, WordSearchPuzzle *puzzle,
                            Dictionary *dict, int marked_row, int marked_col)
{
    int i = 0;
    char *string_builder = malloc(sizeof(char) * (puzzle->height + 1) );

    /*
    *   pre generating string to account for the single letter checks
    *   we already did. we start the string as two chars.
    */
    string_builder[i] = puzzle->grid[marked_row][marked_col];
    marked_row++;
    marked_col--;
    i++;

    /*build string one char at a time and checking if it's a word*/
    while ( marked_row < puzzle->height && marked_col >= 0 )
    {
        string_builder[i] = puzzle->grid[marked_row][marked_col];
        string_builder[i+1] = '\0';
        checkString(dict, string_builder);
        //shift diagonally
        marked_row++;
        marked_col--;
        i++;
    }

    /*clean up*/
    free(string_builder);
    string_builder = NULL;
}

/*
*   take a char from checkDiagLeft, build a string with neigboring chars
*   relative to the direction we are checking and checking each build if
*   it's a valid dictionary word.
*/
void checkDiagLeft_backward(char word_builder, WordSearchPuzzle *puzzle,
                            Dictionary *dict, int marked_row, int marked_col)
{
    int i = 0;
    char *string_builder = malloc(sizeof(char) * (puzzle->height + 1) );

    /*
    *   pre generating string to account for the single letter checks
    *   we already did. we start the string as two chars.
    */
    string_builder[i] = puzzle->grid[marked_row][marked_col];
    marked_row--;
    marked_col++;
    i++;

    while ( marked_row >= 0 && marked_col < puzzle->width )
    {
        string_builder[i] = puzzle->grid[marked_row][marked_col];
        string_builder[i+1] = '\0';
        checkString(dict, string_builder);
        /*shift diagonally*/
        marked_row--;
        marked_col++;
        i++;
    }

    /*clean up*/
    free(string_builder);
    string_builder = NULL;
}

/*  print out the words we found and how many times it was found*/
void results(Dictionary *dict)
{
    int i;
    for(i = 0; i < dict->size; i++)
        if ( *(dict->counts+i) > 0)
            printf("%s (%d)\n", *(dict->words+i), *(dict->counts+i));
}
