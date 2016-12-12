/* Cassidy Brown - cmb195
 * OS Final Project - 12/12/16
 *
 * A "random" number generator from Ken Christensen at University of South Florida
 * http://www.csee.usf.edu/~kchriste/tools/genexp.c 
 * 
 * Adced populateArrayWithExpDist to put generated numbers in an array rather than in a file via main
 */

//===================================================== file = genexp.c =====
//=  Program to generate exponentially distributed random variables         =
//===========================================================================
//=  Notes: 1) Writes to a user specified output file                       =
//=         2) Generates user specified number of values                    =
//=-------------------------------------------------------------------------=
//= Example user input:                                                     =
//=                                                                         =
//=   ----------------------------------------- genexp.c -----              =
//=   -  Program to generate exponential random variables    -              =
//=   --------------------------------------------------------              =
//=   Output file name ===================================> output.dat      =
//=   Random number seed =================================> 1               =
//=   Rate parameter (lambda) ============================> 10.0            =
//=   Number of values to generate =======================> 5               =
//=   --------------------------------------------------------              =
//=   -  Generating samples to file                          -              =
//=   --------------------------------------------------------              =
//=   --------------------------------------------------------              =
//=   -  Done!                                                              =
//=   --------------------------------------------------------              =
//=-------------------------------------------------------------------------=
//= Example output file ("output.dat" for above):                           =
//=                                                                         =
//=   1.175801                                                              =
//=   0.202846                                                              =
//=   0.028024                                                              =
//=   0.077947                                                              =
//=   0.062967                                                              =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 genexp.c                                                  =
//=-------------------------------------------------------------------------=
//=  Execute: genexp                                                        =
//=-------------------------------------------------------------------------=
//=  Author: Ken Christensen                                                =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (10/04/98) - Genesis                                      =
//=           KJC (05/20/03) - Added Jain's RNG for finer granularity       =
//=           KJC (07/07/03) - Changed to generate specified num_values     =
//=           KJC (05/12/00) - Minor clean-up                               =
//===========================================================================
//----- Include files -------------------------------------------------------
#include <stdio.h>            // Needed for printf()
#include <stdlib.h>           // Needed for exit() and ato*()
#include <math.h>             // Needed for log()

//----- Function prototypes -------------------------------------------------
double expon(double x);       // Returns an exponential random variable
double rand_val(int seed);    // Jain's RNG
void originalOperation();
void populateArrayWithExpDist(int (*arr)[], int seed, double lambda, int array_length);

//===========================================================================
//=  Function to generate exponentially distributed random variables        =
//=    - Input:  Mean value of distribution                                 =
//=    - Output: Returns with exponentially distributed random variable     =
//===========================================================================
double expon(double x)
{
    double z;                     // Uniform random number (0 < z < 1)
    double exp_value;             // Computed exponential value to be returned

    // Pull a uniform random number (0 < z < 1)
    do
    {
        z = rand_val(0);
    }
    while ((z == 0) || (z == 1));

    // Compute exponential random variable using inversion method
    exp_value = -x * log(z);

    return(exp_value);
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double rand_val(int seed)
{
    const long  a =      16807;  // Multiplier
    const long  m = 2147483647;  // Modulus
    const long  q =     127773;  // m div a
    const long  r =       2836;  // m mod a
    static long x;               // Random int value
    long        x_div_q;         // x divided by q
    long        x_mod_q;         // x modulo q
    long        x_new;           // New x value

    // Set the seed if argument is non-zero and then return zero
    if (seed > 0)
    {
        x = seed;
        return(0.0);
    }

    // RNG using integer arithmetic
    x_div_q = x / q;
    x_mod_q = x % q;
    x_new = (a * x_mod_q) - (r * x_div_q);
    if (x_new > 0)
        x = x_new;
    else
        x = x_new + m;

    // Return a random value between 0.0 and 1.0
    return((double) x / m);
}

void originalOperation(){
    char   in_string[256];      // Input string
    FILE   *fp;                 // File pointer to output file
    double lambda;              // Mean rate
    double exp_rv;              // Exponential random variable
    int    num_values;          // Number of values
    int    i;                   // Loop counter
    int test;

    // Output banner
    printf("----------------------------------------- genexp.c ----- \n");
    printf("-  Program to generate exponential random variables    - \n");
    printf("-------------------------------------------------------- \n");

    // Prompt for output filename and then create/open the file
    printf("Output file name ===================================> ");
    scanf("%s", in_string);
    fp = fopen(in_string, "w");
    if (fp == NULL)
    {
        printf("ERROR in creating output file (%s) \n", in_string);
        exit(1);
    }

    // Prompt for random number seed and then use it
    printf("Random number seed (greater than 0) ================> ");
    scanf("%s", in_string);
    rand_val((int) atoi(in_string));

    // Prompt for rate (lambda)
    printf("Rate parameter (lambda) ============================> ");
    scanf("%s", in_string);
    lambda = atof(in_string);

    // Prompt for number of values to generate
    printf("Number of values to generate =======================> ");
    scanf("%s", in_string);
    num_values = atoi(in_string);

    // Output message and generate interarrival times
    printf("-------------------------------------------------------- \n");
    printf("-  Generating samples to file                          - \n");
    printf("-------------------------------------------------------- \n");

    // Generate and output exponential random variables
    for (i=0; i<num_values; i++)
    {
        exp_rv = expon(1.0 / lambda);
        test = (int)(exp_rv * 10 + 1);
        fprintf(fp, "%d \n", test);
    }

    // Output message and close the output file
    printf("-------------------------------------------------------- \n");
    printf("-  Done! \n");
    printf("-------------------------------------------------------- \n");
    fclose(fp);
}

// Populates arr with values
void populateArrayWithExpDist(int (*arr)[], int seed, double lambda, int array_length){
    int i;
    double exp_rv;
    int generated;

    if(seed < 0)
        seed = 0;
    rand_val(seed);

    // The first value is always oddly large, so we're going to discard it
    exp_rv = expon(1.0 / lambda);

    for (i=0; i<array_length; i++)
    {
        exp_rv = expon(1.0 / lambda);
        generated = (int)(exp_rv * 10 + 1);
        (*arr)[i]= generated;
    }

}
