/**
 * @file ComapreSequences.c
 * @author  Eliyahu Strugo <eli.strugo@mail.huji.ac.il>
 * @version 1.0
 * @date 14 Nov 2018
 *
 * @brief System is given a file that contains sequences and analyzes the it.
 *
 * @section DESCRIPTION
 * Input  : text file which contains at least two sequences
 * Process: Parsing the sequences in the given file and by that calculates the scores for alignment of each pairs of
 * sequences.
 * Output : prints the scores for alignment of each pairs of sequences.
 */

// ------------------------------ includes ------------------------------

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>

// -------------------------- const definitions -------------------------

#define NEW_SEQUENCE_FLAG '>'

#define NEW_LINE '\n'

#define FAILED 1

#define MAX_LEN 100

#define MAX_SEQUENCES 100

#define MIN_SEQUENCES 2

#define USER_MSG "Score for alignment of seq%d to seq%d is %d\n"

/**
 * This structure represents a single sequences that contains it's content, length and the number of the sequences
 * within the given File
 */
// ------------------------------ structures -----------------------------

typedef struct 
{
    int seqNum;
    char *seq;
    size_t seqLen;
}
        Sequence;

// ------------------------------ globals -----------------------------

Sequence sequences[MAX_SEQUENCES];

// ------------------------------ functions -----------------------------


/**
 * This function finds the max between three integers
 *
 * @param x - some integer
 * @param y - some integer
 * @param z - some integer
 * @return the max value of the three given integers
 */
int findMax(int x, int y, int z)
{
    int max = x > y ? x : y;
    max = max > z ? max : z;
    return max;
}

void buildMatrix(Sequence seq1, Sequence seq2, int m, int s, int gap, size_t N, size_t M, int **matrix)
{
    unsigned int i, j;
    int res1, res2, res3;
    for (i = 1; i < N; i++)
    {
        for (j = 1; j < M; j++)
        {
            int isMatch = seq1.seq[i - 1] == seq2.seq[j - 1];
            res1 = isMatch ? matrix[i - 1][j - 1] + m : matrix[i - 1][j - 1] + s;
            res2 = matrix[i][j - 1] + gap;
            res3 = matrix[i - 1][j] + gap;
            matrix[i][j] = findMax(res1, res2, res3);
        }
    }
}

/**
 * This function initializes the first row and the first column of the given alignment matrix based of the
 * given parameters
 *
 * @param gap - value of gap
 * @param n - Number of rows in matrix
 * @param m -Number of column in the matrix
 * @param matrix - 2D array( memory already allocated)
 */
void initializeMatrix(int gap, size_t n, size_t m, int **matrix)
{
    unsigned int i, j;
    for (j = 1; j < m; j++)
    {
        matrix[0][j] = matrix[0][j - 1] + gap;
    }
    for (i = 1; i < n; i++)
    {
        matrix[i][0] = matrix[i - 1][0] + gap;
    }
}

/**
 * This functions creates 2D (n*m) int array and allocates memory sufficient for each row
 * @param n - Number of rows
 * @param m - Number of columns
 * @return returns pointer to the 2D array
 */
int **allocateMatrix(size_t n, size_t m)
{
    unsigned int rowIndex;
    int **matrix = (int **) malloc(n * sizeof(int *));
    if (matrix == NULL)
    {
        fprintf(stderr, "Failed to allocate memory to matrix");
        exit(EXIT_FAILURE);
    }
    for (rowIndex = 0; rowIndex < n; rowIndex++)
    {
        matrix[rowIndex] = (int *) malloc(m * sizeof(int));
        if (matrix[rowIndex] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory to row");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

/**
 * This function frees the memory of the given 2D array
 *
 * @param n - number of rows
 * @param matrix - 2D array
 */
void freeMatrix(size_t n, int **matrix)
{
    unsigned int i;
    for (i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void getAlignment(Sequence seq1, Sequence seq2, int match, int misMatch, int gap)
{
    size_t n, m;
    n = seq1.seqLen + 1;
    m = seq2.seqLen + 1;
    int **matrix = allocateMatrix(n, m);
    matrix[0][0] = 0;
    initializeMatrix(gap, n, m, matrix); //First row and first column
    buildMatrix(seq1, seq2, match, misMatch, gap, n, m, matrix);
    printf(USER_MSG, seq1.seqNum, seq2.seqNum, matrix[n - 1][m - 1]);
    freeMatrix(n, matrix);
}

/**
 * This function is given string and checks and removes unwanted chars from it
 * @param textLine - line of the text(string)
 */
void removeNewLine(const char *textLine)
{
    char *pos;
    if ((pos = strchr(textLine, NEW_LINE)) != NULL)
    {
        *pos = '\0';
    }
    if ((pos = strchr(textLine, '\r')) != NULL)
    {
        *pos = '\0';
    }

}

/**
 *This function allocates memory to store the sequences whose index is given
 *
 * @param seqIndex - The index to the current sequence that stored int the global array of Sequences
 * @param seqLineCounter - The number of the line int the scope of the current sequence
 * @param lineSize - the length of the line that being read
 */
void allocateSequences(int seqIndex, int seqLineCounter, size_t lineSize)
{
    size_t newSize;
    if (seqLineCounter == 0)
    {
        sequences[seqIndex].seq = (char *) calloc(lineSize, sizeof(char));
    } 
    else
    {
        newSize = strlen(sequences[seqIndex].seq) + lineSize + 1;
        sequences[seqIndex].seq = (char *) realloc(sequences[seqIndex].seq, newSize);
    }
}

/**
 * This function is given a File that contains, reads the file line by line , allocates memory for each sequence and
 * store it. within the process the function counts the number of sequences and returns it.
 *
 * @param myFile - The file that contains sequences
 * @return The number of sequences in the given file
 */
unsigned int analyzeText(FILE *myFile)
{
    char textLine[MAX_LEN];
    unsigned int seqNum, seqIndex, seqLineCounter;
    seqNum = seqIndex = seqLineCounter = 0;

    while (fgets(textLine, MAX_LEN, myFile))
    {
        if (textLine[0] != NEW_SEQUENCE_FLAG)
        {
            removeNewLine(textLine);
            allocateSequences(seqIndex, seqLineCounter, strlen(textLine) + 1);
            seqLineCounter++;
            strcat(sequences[seqIndex].seq, textLine);
            sequences[seqIndex].seqLen = strlen(sequences[seqIndex].seq);
        } 
        else
        {
            seqIndex = seqNum;
            seqNum++;
            seqLineCounter = 0;
            sequences[seqIndex].seqNum = seqNum;
        }
    }
    if (seqNum <MIN_SEQUENCES )
    {
        printf("Too few sequences");
        exit(EXIT_FAILURE);
    }
    return seqNum;

}

/**
 * This function is given a string (array of chars), and  attempts to convert it to int.
 * @param str - Array of chars of digits
 * @return The converted int in success otherwise exits the program 
 */
int convertStrToInt(char *str)
{
    char *end;
    errno = 0;
    int val;
    val = (int) strtol(str, &end, 10);
    if (val == 0 && (errno != 0 || *end != '\0'))
    {
        fprintf(stderr, "Error: input is not a valid Integer\n");
        exit(EXIT_FAILURE);
    }
    return val;
}

/**
 * This function analyzes every pair of sequences
 * @param numOfSequences - the number of sequences
 * @param match - match value
 * @param misMatch - mis-match value
 * @param gap - gap value
 */
void analyzeSequences(unsigned int numOfSequences, int match, int misMatch, int gap)
{
    unsigned int i, j;
    for (i = 0; i < numOfSequences; i++)
    {
        for (j = i + 1; j < numOfSequences; j++)
        {
            getAlignment(sequences[i], sequences[j], match, misMatch, gap);
        }
    }
}

int main(int argc, char **argv)
{
    unsigned int i, numOfSequences;
    if (argc <= 1 || argc > 5) // Too few or too many arguments
    {
        fprintf(stdout, "Usage: CompareSequences <path_to_sequences_file> <m> <s> <g>\n");
        return FAILED;
    }
    int match, misMatch, gap;
    FILE *myFile = fopen(argv[1], "r");
    if (myFile == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return FAILED;
    }
    numOfSequences = analyzeText(myFile);
    fclose(myFile);
    match = convertStrToInt(argv[2]);
    misMatch = convertStrToInt(argv[3]);
    gap = convertStrToInt(argv[4]);
    analyzeSequences(numOfSequences, match, misMatch, gap);
    for (i = 0; i < numOfSequences; i++) // free sequences
    {
        free(sequences[i].seq);
    }
}
