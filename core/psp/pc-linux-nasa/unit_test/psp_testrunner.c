
void PSP_AddTestCase(void);

/*
 * Includes
 */

#include "uttest.h"
#include <getopt.h>
#include "utlist.h"


extern UtListHead_t UtTestDataBase;

/*
** getopts parameter passing options string
*/
static const char *optString = "T:h:";

/*
** getopts_long long form argument table
*/
static const struct option longOpts[] = {
   { "test", required_argument, 0, 'T' },
   { "count",no_argument,       0, 'C' },
   { "help", no_argument,       0, 'h' },
   { 0,      no_argument,       0,  0 }
};

/*
 * Function Definitions
 */

void PSP_UT_DisplayUsage(char *Name );

int main(int argc, char *argv[])
{
	unsigned int testCaseNumber = 0;
    int opt = 0;
    int longIndex = 0;
    int rc = 0;
    boolean runTests = TRUE;

    /* Call AddTestSuite or AddTestCase functions here */
    PSP_AddTestCases();

	/*
	 ** Process the arguments with getopt_long(), then
	 ** start the cFE
	 */
	opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	while( opt != -1 )
	{
	    switch( opt )
	    {
	        case 'T':
	        {
	        	testCaseNumber = strtol(optarg, 0, 0 );
	            break;
	        }

	        case 'C':
	        {
	            rc = UtTestDataBase.NumberOfEntries;
	            runTests = FALSE;
	            printf("%d\n", rc);
	            break;
	        }

	        case 'h':
	        {
	            PSP_UT_DisplayUsage(argv[0]);
	            runTests = FALSE;
	            break;
	        }

	        default:
	        {
	            break;
	        }
	    }

        opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	} /* end while */

	if(runTests == TRUE)
	{
		if(testCaseNumber == 0)
		{
			rc = UtTest_Run();
		}
		else
		{
			rc = UtTest_RunTest(testCaseNumber);
		}
	}

	return rc;
}


void PSP_UT_DisplayUsage(char *Name )
{

   printf("usage : %s [-T <value>] [-C] [-h] \n", Name);
   printf("\n");
   printf("        All parameters are optional and can be used in any order.\n");
   printf("        Total number of test cases:  %d\n", UtTestDataBase.NumberOfEntries);
   printf("\n");
   printf("        Parameters include:\n");
   printf("        -T [ --test ]  Test case to execute.:\n");
   printf("             When omitted, all test cases will execute.  Range is 1 to %d.\n", UtTestDataBase.NumberOfEntries);
   printf("        -C [ --count ] Return the total test case count.\n");
   printf("        -h [ --help ]    This message.\n");
   printf("\n");
   printf("       Example invocation:\n");
   printf(" \n");
   printf("       Short form:\n");
   printf("       %s -T 1 \n",Name);
   printf("       Long form:\n");
   printf("       %s --test 1\n",Name);
   printf(" \n");

}
