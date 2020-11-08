#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"

//Helper function
//************************************************************************************* 
//function name : checkDestGood
//
// Description : This function checks if input for destination is legal
//
// Parameters: dest: a string containing the destination for a flight
//			   
// Return value : SUCCESS if the destination is legal, FAILURE otherwise
//*************************************************************************************
Result checkDestGood(char dest[4])
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
//************************************************************************************* 
//function name : createFlight
//
// Description : This function creates a new flight
//
// Parameters:  num: the flight number
//				type: the type of flight - DOMESTIC or INTERNATIONAL
//				dest: the flight destination - 3 capital letters: "ABC"
//				emergency: TRUE if the flight is an emergency flight, FALSE if not
//
// Return value : None
//*************************************************************************************
PFLIGHT createFlight(int num, FlightType type, char dest[4], BOOL emergency)
{
	if (num > MAX_ID || num < 1)
		return NULL;
	if (checkDestGood(dest) != SUCCESS)
		return NULL;
	PFLIGHT flight = (PFLIGHT)malloc(sizeof(FLIGHT));
	if (flight == NULL)
		return NULL;
	flight->num = num;
	flight->type = type;
	flight->dest[0] = dest[0];
	flight->dest[1] = dest[1];
	flight->dest[2] = dest[2];
	flight->dest[3] = dest[3];
	flight->emergency = emergency;
	return flight;
}
//************************************************************************************* 
//function name : destroyFlight
//
// Description : This function destroys an existing flight
//
// Parameters:  flight: a pointer to a flight struct
//
// Return value : None
//*************************************************************************************
void destroyFlight(PFLIGHT flight)
{
	if (flight == NULL)
		return;
	free(flight);
	return;
}
//************************************************************************************* 
//function name : printFlight
//
// Description : This function prints out the details of a flight: flight number, 
//				 flight type, flight destination, emergency flight or not.
//
// Parameters:  flight: a pointer to a flight struct
//
// Return value : None
//*************************************************************************************
void printFlight(PFLIGHT flight)
{
	if (flight == NULL)
		return;
	// convert type and emergency typedef into chars
	char type, emergency;
	if (flight->type == DOMESTIC)
		type = 'D';
	else
		type = 'I';
	if (flight->emergency == TRUE)
		emergency = 'E';
	else
		emergency = 'R';
	printf("Flight %d %c %s %c", flight->num, type, flight->dest, emergency);
	return;
}