#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flight.h"
#include "runway.h"
#include "airport.h"

#define MAX_LINE_SIZE 256

//FUNCTIONS: check if the dest is only 3 capital letters

//************************************************************************************* 
//function name : checkDest
//
// Description : This function checks if input for destination is legal
//
// Parameters: dest: a string containing the destination for a flight
//			   
// Return value : SUCCESS if the destination is legal, FAILURE otherwise
//*************************************************************************************
Result checkDest(char* dest)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == 3)
		{
			if (dest[3] == 0x00)
				return SUCCESS;
			else
				return FAILURE;
		}

		if (dest[i] < 'A' || dest[i] > 'Z')
		{
			return FAILURE;
		}
	}
	return FAILURE;
}

int main()
{
	char szLine[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	while (fgets(szLine, MAX_LINE_SIZE, stdin))
	{
		char* pszCommand = strtok(szLine, delimiters); // read the first word in the line
		if (!pszCommand) //check if the data not defined
		{
			destroyAirport();
			return 0;
		}
		else if (strcmp(pszCommand, "Insert") == 0) //if the words are the same
		{
			char* pszData1 = strtok(NULL, delimiters); // get second word
			char* pszData2 = strtok(NULL, delimiters); // get third word
			if (!pszData1 || !pszData2) //check if the data not defined
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", pszCommand);
				continue;
			}
			int numRunway = atoi(pszData1);
			char* typeRunway = pszData2;
			// check if the data is ok
			if (numRunway < 1 || numRunway > MAX_ID || (strcmp(typeRunway, "I") != 0 && strcmp(typeRunway, "D") != 0))
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
			// run the function
			if (addRunway(numRunway, ((strcmp(typeRunway, "D") == 0) ? DOMESTIC : INTERNATIONAL)) == FAILURE) // if function FAILED
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
		}
		else if (strcmp(pszCommand, "Remove") == 0)
		{
			char* pszData1 = strtok(NULL, delimiters);
			if (!pszData1)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", pszCommand);
				continue;
			}
			int runwayNum = atoi(pszData1);

			if (runwayNum < 1 || runwayNum > MAX_ID) {
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
			if (removeRunway(runwayNum) == FAILURE)
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
		}
		else if (strcmp(pszCommand, "Add") == 0)
		{
			char* pszData1 = strtok(NULL, delimiters);
			char* pszData2 = strtok(NULL, delimiters);
			char* pszData3 = strtok(NULL, delimiters);
			char* pszData4 = strtok(NULL, delimiters);
			if (!pszData1 || !pszData2 || !pszData3 || !pszData4)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", pszCommand);
				continue;
			}
			int flightNum = atoi(pszData1);
			BOOL checkType = (strcmp(pszData2, "I") != 0 && strcmp(pszData2, "D") != 0);
			BOOL checkEmergency = (strcmp(pszData4, "E") != 0 && strcmp(pszData4, "R") != 0);
			Result checkDestrny = checkDest(pszData3);
			if (flightNum < 1 || flightNum > MAX_ID || checkType || checkEmergency || !checkDestrny)
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
			FlightType type = ((strcmp(pszData2, "D") == 0) ? DOMESTIC : INTERNATIONAL);
			BOOL emergency = ((strcmp(pszData4, "E") == 0) ? TRUE : FALSE);
			char tempDest[4] = { pszData3[0], pszData3[1], pszData3[2], 0x00 };
			if (addFlightToAirport(flightNum, type, tempDest, emergency) == FAILURE)
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
		}
		else if (strcmp(pszCommand, "Depart") == 0)
		{
			if (departAirport() == FAILURE)
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
		}
		else if (strcmp(pszCommand, "Change") == 0)
		{
			char* pszOldDest = strtok(NULL, delimiters);
			char* pszNewDest = strtok(NULL, delimiters);
			if (!pszOldDest || !pszNewDest)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", pszCommand);
				continue;
			}
			
			if (!checkDest(pszOldDest) || !checkDest(pszNewDest))
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
			char tempDestOld[4] = { pszOldDest[0], pszOldDest[1], pszOldDest[2], 0x00 };
			char tempDestNew[4] = { pszNewDest[0], pszNewDest[1], pszNewDest[2], 0x00 };
			if (changeDest(tempDestOld, tempDestNew) == FAILURE)
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
		}
		else if (strcmp(pszCommand, "Delay") == 0)
		{
			char* pszDest = strtok(NULL, delimiters);
			if (!pszDest)
			{
				fprintf(stderr, "%s failed: not enough parameters.\n", pszCommand);
				continue;
			}

			if (checkDest(pszDest) == FAILURE)
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}

			if (delay(pszDest) == FAILURE)
			{
				fprintf(stderr, "%s execution failed.\n", pszCommand);
				continue;
			}
		}
		else if (strcmp(pszCommand, "Print") == 0)
		{
			printAirport();
		}
		else if (strcmp(pszCommand, "Exit") == 0)
		{
			destroyAirport();
			return 0;
		}
		else
		{
			fprintf(stderr, "Command not found.\n");
		}
	}
	destroyAirport(); //frees all runways and flights
	return 0;
}