/**
 * @file AnalyzeProtein.c
 * @author  Eliyahu Strugo <eli.strugo@mail.huji.ac.il>
 * @version 1.0
 * @date 2 Oct 2018
 *
 * @brief System is given files that contains information about proteins and analyzes it.
 *
 * @section DESCRIPTION
 * The system keeps track of the cooking times.
 * Input  : pdb file/s which contains the relevant information ( ATOM lines)
 * Process: Parsing the coordinates of the atoms in the given files and by that calculates the protein's
 * Center of mass, Radius of gyration and The maximum distance within the protein.
 * Output : prints the result of the analysis.
 */

// ------------------------------ includes ------------------------------

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

// -------------------------- const definitions -------------------------

#define LEN_OF_LINE 80

#define NUM_OF_COORD 3

#define MAX_LINES 20000

#define NUM_OF_COORDS 3

#define ATOM_FLAG "ATOM  "

#define WORD_LEN 6

#define MIN_LINE_LEN 60

#define FLAG_LEN 7

#define LEN_OF_COORD 8

#define FAILURE 1

// ------------------------------ functions -----------------------------

/**
 * This function is given a single ATOM line and an array and converts the coordinates of the given atom from the
 * given text to floats and places them in the given array.
 *
 * @param atom - an array it will contains the three coordinates of the given "atom"
 * @param textLine - string (line) , which contains the relevant information to analyze the atom
 */
void createCoordinates(float *atom, char *textLine)
{
    int j, k;
    float curFloatCoord;
    char *coord = NULL;
    char curCoord[LEN_OF_COORD + 1];
    errno = 0;
    for (j = 0, k = 0; j < NUM_OF_COORDS; j++, k += LEN_OF_COORD)
    {
        strncpy(curCoord, textLine + (30 + k), 8);
        curFloatCoord = strtof(curCoord, &coord);
        if (curFloatCoord == 0 && (errno != 0 || coord == curCoord))// the conversion failed
        {
            fprintf(stderr, "Error in coordinate conversion %s!\n", curCoord);
            exit(EXIT_FAILURE);
        }
        atom[j] = strtof(curCoord, (char **) coord);
    }
}

/**
 *This function is given pointer to an array(2D) and changes it's content, for every  i between 0 to MAX_LINES
 * atoms[i] = [x , y , z] which x,y,z are the coordinates of the atoms i.
 *
 * @param myFile - The file which contains the text to be analyzed
 * @param atoms - 2D array which contains the coordinates of each atom
 * @return The number of atoms that were read from the given file
 */
int createAtoms(FILE *myFile, float atoms[MAX_LINES][NUM_OF_COORD])
{
    char textLine[LEN_OF_LINE];
    char curWord[FLAG_LEN];
    int numOfAtoms = 0;
    while (!feof(myFile))
    {
        fgets(textLine, LEN_OF_LINE, myFile);
        strncpy(curWord, textLine, WORD_LEN);
        size_t lineLen = strlen(textLine);
        if (strcmp(curWord, ATOM_FLAG) == 0)
        {
            if (lineLen <= MIN_LINE_LEN)
            {
                fprintf(stderr, "ATOM line is too short %zu characters\n", lineLen);
                exit(EXIT_FAILURE);
            }
            createCoordinates(atoms[numOfAtoms], textLine);
            numOfAtoms++;
            if (numOfAtoms >= MAX_LINES)
            {
                break;
            }
        }
    }
    return numOfAtoms;
}

/**
 *This function calculates the Center of mass of the protein base on the given arguments
 *
 * @param numOfAtoms - The number of atoms that appears in the file
 * @param atoms - 2D array which contains the coordinates of each atom
 * @param cg - Three coordinates of the Center of mass
 */
void createCg(int numOfAtoms, float atoms[MAX_LINES][NUM_OF_COORD], float cg[NUM_OF_COORDS])
{
    int i, j;
    float sumOfCoordinates = 0;
    for (j = 0; j < NUM_OF_COORD; j++)
    {
        for (i = 0; i < numOfAtoms; i++)
        {
            sumOfCoordinates += atoms[i][j];
        }
        cg[j] = (sumOfCoordinates / numOfAtoms);
        sumOfCoordinates = 0;
    }
}

/**
 *This function is given the number of atoms in the protein, an array that contains the atoms coordinates and the
 * three coordinates of the Center of mass, and base on the given arguments calculates the The Radius of gyration.
 *
 * @param numOfAtoms - The number of atoms that appears in the file
 * @param atoms - 2D array which contains the coordinates of each atom
 * @param cg - Three coordinates of the Center of mass
 * @return The Radius of gyration
 */
float getRg(int numOfAtoms, float atoms[MAX_LINES][NUM_OF_COORDS], float *cg)
{
    int i, j;
    float curSum = 0;
    for (i = 0; i < numOfAtoms; i++)
    {
        for (j = 0; j < NUM_OF_COORDS; j++)
        {
            curSum += powf(cg[j] - atoms[i][j], 2);
        }
    }
    curSum = sqrtf(curSum / numOfAtoms); // numOfAtom != 0
    return curSum;
}

/**
 *This function is given an array(2D) that contain the coordinates of each atom and number of atoms in the protein
 * and calculates the maximum distance of all the atoms in the protein and returns it.
 *
 * @param atoms - 2D array which contains the coordinates of each atom
 * @param numOfAtoms - The number of atoms that appears in the file
 * @return The maximum distance within the protein : the max distance of all the atoms in the protein
 */
float getDmax(float atoms[MAX_LINES][NUM_OF_COORDS], int numOfAtoms)
{
    int i, j, k;
    float curSum = 0, max = 0, curMax = 0;
    for (i = 0; i < numOfAtoms; i++)
    {
        for (k = 1; k < numOfAtoms; k++)
        {
            for (j = 0; j < NUM_OF_COORDS; j++)
            {
                curSum += powf(atoms[i][j] - atoms[k][j], 2);
            }
            curMax = sqrtf(curSum);
            if (curMax > max)
            {
                max = curMax;
            }
            curSum = 0;
        }
    }
    return max;
}

/**
 *This function prints the output of the analysis.
 *
 * @param fileName - The name of the file which is being analyzed
 * @param numOfAtoms - The number of atoms that appears in the file
 * @param cg - Three coordinates of the Center of mass
 * @param rg - The Radius of gyration
 * @param dMax - The maximum distance within the protein : the max distance of all the atoms in the protein
 */
void printOutput(const char *fileName, int numOfAtoms, const float *cg, float rg, float dMax)
{
    printf("PDB file %s, %d atoms were read\n", fileName, numOfAtoms);
    printf("Cg = %.3f %.3f %.3f\n", cg[0], cg[1], cg[2]);
    printf("Rg = %.3f\n", rg);
    printf("Dmax = %.3f\n", dMax);
}

/**
 *This function is given a file (which contains text), the function reads the file and performs mathematical
 * manipulations on the text in order to analyze it.
 *
 * @param myFile - The file which contains the text to be analyzed
 * @param fileName - The name of the file which is being analyzed
 * @return if successful returns 0 and int != 0 otherwise
 */
int analyzeInput(const FILE *myFile, char *fileName)
{
    float atoms[MAX_LINES][NUM_OF_COORD];
    int numOfAtoms = createAtoms((FILE *) myFile, atoms);
    if (numOfAtoms == 0)
    {
        fprintf(stderr, "Error - 0 atoms were found in the file %s\n", fileName);
        return FAILURE;
    }
    float cg[NUM_OF_COORD];
    createCg(numOfAtoms, atoms, cg);
    float rg = getRg(numOfAtoms, atoms, cg);
    float dMax = getDmax(atoms, numOfAtoms);
    printOutput(fileName, numOfAtoms, cg, rg, dMax);
    fclose((FILE *) myFile);
    return 0;
}


/**
 *Runs the AnalyzeProtein program
 *
 * @return if successful returns 0 and 1 otherwise
 */
int main(int argc, char **argv)
{
    if (argc <= 1) // Too few arguments
    {
        fprintf(stdout, "Usage: AnalyzeProtein <pdb1> <pdb2> ...\n");
        return FAILURE;
    }
    for (int i = 1; i < argc; ++i)
    {
        FILE *myFile = fopen(argv[i], "r");
        if (myFile == NULL)
        {
            fprintf(stderr, "Error opening file: %s\n", argv[i]);
            return FAILURE;
        }
        int result = analyzeInput(myFile, argv[i]);
        if (result == FAILURE) // failed to analyze the file
        {
            return FAILURE;
        }
    }
    return 0;
}
