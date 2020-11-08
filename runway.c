#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "flight.h"
#include "runway.h"

//************************************************************************************* 
//function name : destroyLinkedQueue
//
// Description : This function destroys a linked queue node, which is a struct 
//			     in the linked list of flights.
//
// Parameters:  link - a node in the linked list of flights
//
// Return value : None
//*************************************************************************************
void destroyLinkedQueue(PLINKEDQUEUE link)
{
	//edge case, already destroy
	if (link == NULL)
		return;

	destroyFlight(link->flight);
	free(link);
	return;
}
//************************************************************************************* 
//function name : addQueue
//
// Description : This helper function adds a new node to a linked list
//
// Parameters:  runway: a pointer to a runway
//				linkedFlight: a pointer to a flight that we want to add to the list
//
// Return value : None
//*************************************************************************************
void addQueue(PRUNWAY runway, PLINKEDQUEUE linkedFlight)
{
	//edge case
	if (runway == NULL || linkedFlight == NULL)
		return;

	linkedFlight->next = runway->headQueue;
	runway->headQueue = linkedFlight;
	return;
}
//************************************************************************************* 
//function name : emergencyAddQueue
//
// Description : This helper function adds an emergency flight to the list of flights 
//               of a particular runway.
//
// Parameters:  runway: a pointer to a runway that we want to add the flight to
//				linkedFlight: a pointer to a flight that we want to add to the list
//
// Return value : None
//*************************************************************************************
void emergencyAddQueue(PRUNWAY runway, PLINKEDQUEUE linkedFlight)
{
	//edge case
	if (runway == NULL || linkedFlight == NULL)
		return;

	//edge case
	PLINKEDQUEUE preLink = runway->headQueue;
	if (preLink == NULL || preLink->flight->emergency == TRUE)
	{
		addQueue(runway, linkedFlight);
		return;
	}

	// continue until the next link is en emergency prelink, or we reach the end of the queue
	while (preLink->next != NULL && preLink->next->flight->emergency == FALSE)
	{
		preLink = preLink->next;
	}
	// we want to add the linkedFlight after prelink
	linkedFlight->next = preLink->next;
	preLink->next = linkedFlight;
	return;
}
//************************************************************************************* 
//function name : findFlightInRunway
//
// Description : This function searches a runway for a specific flight
//
// Parameters: runway: a pointer to the runway whose flight list we want to search
//             num: the number of the flight that we are searching for
//			   
// Return value : SUCCESS if the flight was found, FAILURE otherwise
//*************************************************************************************
Result findFlightInRunway(PRUNWAY runway, int num)
{
	//edge case
	if (runway == NULL)
		return FAILURE;
	PLINKEDQUEUE nextLink = runway->headQueue;
	while (1)
	{
		// stopping condition
		if (nextLink == NULL)
			return FAILURE;
		// if we find a flight with the num in the runway
		if ((nextLink->flight)->num == num)
			return SUCCESS;
		nextLink = nextLink->next;
	}
}
//************************************************************************************* 
//function name : removeLastLink
//
// Description : This helper function removes the last flight from a list of flights
//
// Parameters:  runway: a pointer to a runway that we want to remove the flight from
//
// Return value : a pointer to the flight node that we want to remove 
//*************************************************************************************
// up to the user to free the memory
PLINKEDQUEUE removeLastLink(PRUNWAY runway)
{
	// edge cases
	if (runway == NULL || runway->headQueue == NULL)
		return NULL;
	//check for one flight in runway
	PLINKEDQUEUE firstLink = runway->headQueue;
	if (firstLink->next == NULL)
	{
		runway->headQueue = NULL;
		return firstLink;
	}
	// search for the last link in the queue, when next Link == NULL
	PLINKEDQUEUE preLink = runway->headQueue;
	PLINKEDQUEUE link = runway->headQueue->next;
	while (link->next != NULL)
	{
		preLink = preLink->next;
		link = link->next;
	}
	// remove link from the queue
	preLink->next = NULL;
	return link;
}
//************************************************************************************* 
//function name : createRunway
//
// Description : This function creates a new, empty runway
//
// Parameters: num: the desired number for the new runway
//			   type: the type of the runway - DOMESTIC or INTERNATIONAL
//
// Return value : a pointer to the runway, or NULL if failure
//*************************************************************************************
PRUNWAY createRunway(int num, FlightType type)
{
	//check input
	if (num > MAX_ID || num < 1)
		return NULL;

	PRUNWAY runway = (PRUNWAY)malloc(sizeof(RUNWAY));
	if (runway == NULL)
		return NULL;

	runway->num = num;
	runway->type = type;
	runway->headQueue = NULL;

	return runway;
}

//************************************************************************************* 
//function name : addFlight
//
// Description : This function adds a flight to a runway
//
// Parameters: runway: a pointer to the runway that we want to add the flight to
//			   flight: a pointer to the flight that we want to add 
//
// Return value : a pointer to the runway, or NULL if failure
//*************************************************************************************
Result addFlight(PRUNWAY runway, PFLIGHT flight)
{
	//edge case
	if (runway == NULL || flight == NULL)
		return FAILURE;

	// search if the flight not already in the runway
	if (findFlightInRunway(runway, flight->num) == SUCCESS)
	{
		free(flight);
		return FAILURE;
	}
	if (runway->type != flight->type)
	{
		free(flight);
		return FAILURE;
	}
	PLINKEDQUEUE linkedQ = (PLINKEDQUEUE)malloc(sizeof(LINKEDQUEUE));
	if (linkedQ == NULL)
	{
		free(flight);
		return FAILURE;
	}
	// create a copy of the flight
	char tmp[4];
	tmp[0] = flight->dest[0];
	tmp[1] = flight->dest[1];
	tmp[2] = flight->dest[2];
	tmp[3] = flight->dest[3];
	PFLIGHT tempFlight = createFlight(flight->num, flight->type, tmp, flight->emergency);
	free(flight);
	if (tempFlight == NULL)
	{
		free(linkedQ);
		return FAILURE;
	}
	// add the linked flight to the start of the queue
	linkedQ->flight = tempFlight;
	linkedQ->next = NULL;

	if (linkedQ->flight->emergency == TRUE)
		emergencyAddQueue(runway, linkedQ);
	else
		addQueue(runway, linkedQ);

	return SUCCESS;
}
//************************************************************************************* 
//function name : depart
//
// Description : This function removes the first (last added) flight from a runway
//
// Parameters: runway: a pointer to the runway that we want a plane to depart from
//
// Return value : SUCCESS if a plane successfully departed, FAILURE if not
//*************************************************************************************
Result depart(PRUNWAY runway)
{
	//edge case
	if (runway == NULL)
		return FAILURE;

	PLINKEDQUEUE link = removeLastLink(runway);
	if (link == NULL)
	{
		//free(link);
		return FAILURE;
	}
	else
	{
		if(link->flight != NULL)
		{
			destroyFlight(link->flight);
		}
		free(link);
		return SUCCESS;
	}
}
//************************************************************************************* 
//function name : removeFlight
//
// Description : This function removes a flight from a list of flights
//
// Parameters: runway: a pointer to the runway that we want to remove the flight from
//			   num: the desired number for the new runway
//			   
// Return value : SUCCESS if a plane was removed, FAILURE if not
//*************************************************************************************
Result removeFlight(PRUNWAY runway, int num)
{
	// check if inputs are ok, and if the flight is in the queue
	if (runway == NULL || findFlightInRunway(runway, num) == FAILURE)
		return FAILURE;

	PLINKEDQUEUE preLink = runway->headQueue;
	PLINKEDQUEUE link = runway->headQueue->next;

	// if the flight is in the fist link
	if (preLink->flight->num == num)
	{
		runway->headQueue = link;
		destroyLinkedQueue(preLink);
		return SUCCESS;
	}
	// from here the flight is sure to be in the second link or deeper
	while (1)
	{
		if (link->flight->num == num)
		{
			preLink->next = link->next;
			destroyLinkedQueue(link);
			return SUCCESS;
		}
		preLink = preLink->next;
		link = link->next;
	}
}
//************************************************************************************* 
//function name : destroyRunway
//
// Description : This function destroys an entire runway, and all the flights on it
//
// Parameters: runway: a pointer to the runway that we want to remove
//			   
// Return value : none
//*************************************************************************************
void destroyRunway(PRUNWAY runway)
{
	//edge case
	if (runway == NULL)
		return;
	// remove the head flight in the queue and continue until all the flights are removed
	while (runway->headQueue != NULL)
	{
		removeFlight(runway, runway->headQueue->flight->num);
	}
	free(runway);
	return;
}
//************************************************************************************* 
//function name : getFlightNum
//
// Description : This function gets the number of flights waiting on a runway
//
// Parameters: runway: a pointer to the runway whose flights we want to count
//			   
// Return value : the number of flights on the runway, -1 if error
//*************************************************************************************
int getFlightNum(PRUNWAY runway)
{
	//edge case
	if (runway == NULL)
		return -1;
	PLINKEDQUEUE linkFlight = runway->headQueue;
	int numWaitingFlights = 0;
	// run until the end of the list
	while (linkFlight)
	{
		++numWaitingFlights;
		linkFlight = linkFlight->next;
	}
	return numWaitingFlights;
}
//************************************************************************************* 
//function name : getEmergencyNum
//
// Description : This function counts the number of emergency flights on a runway
//
// Parameters: runway: a pointer to the runway whose emergency flights we want to count
//			   
// Return value : the number of emergency flights waiting, -1 if error.
//*************************************************************************************
int getEmergencyNum(PRUNWAY runway)
{
	//egde case
	if (runway == NULL)
		return -1;

	PLINKEDQUEUE linkFlight = runway->headQueue;
	int numWaitingFlights = 0;
	// run until the end of the queue
	while (linkFlight)
	{
		if (linkFlight->flight->emergency == TRUE)
			++numWaitingFlights;
		linkFlight = linkFlight->next;
	}

	return numWaitingFlights;
}
//************************************************************************************* 
//function name : isFlightExists
//
// Description : This function checks if a flight with a certain flight number exists
//				 on a runway.
//
// Parameters: runway: a pointer to the runway that want to check
//			   num: the flight number of the flight that we want to check for
//			   
// Return value : TRUE if the flight is waiting, FALSE if not
//*************************************************************************************
BOOL isFlightExists(PRUNWAY runway, int num)
{
	// check for good input, edge case
	if (num > MAX_ID || num < 1 || runway == NULL)
		return FALSE;

	// return SUCCESS if the flight is in the runway.
	if (findFlightInRunway(runway, num) == SUCCESS)
		return TRUE;
	else
		return FALSE;
}
//************************************************************************************* 
//function name : doPrint
//
// Description : This function prints all the flights on a runway recursively
//
// Parameters: link: a pointer to a flight node in the list of flights on a runway
//			   
// Return value : none
//*************************************************************************************
void doPrint(PLINKEDQUEUE link)
{
	// exit point
	if (link == NULL)
		return;

	// go deep first
	doPrint(link->next);

	// then print this link
	printFlight(link->flight);
	printf("\n");
	return;
}
//************************************************************************************* 
//function name : printRunway
//
// Description : This function is a cover function for printing the details of all 
//               flights waiting on a runway.
//
// Parameters: runway: a pointer to the runway whose flight list we want to print
//			   
// Return value : none
//*************************************************************************************
void printRunway(PRUNWAY runway)
{
	//edge case
	if (runway == NULL)
		return;

	// check what type the runway is
	char* tmpRunwayType;
	if (runway->type == DOMESTIC)
		tmpRunwayType = "domestic";
	else
		tmpRunwayType = "international";

	// printing
	printf("Runway %u %s\n", runway->num, tmpRunwayType);
	printf("%d flights are waiting:\n", getFlightNum(runway));

	//print from the end to the start
	doPrint(runway->headQueue);
}