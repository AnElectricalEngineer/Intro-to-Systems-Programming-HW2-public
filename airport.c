#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "flight.h"
#include "runway.h"
#include "airport.h"

//*************************************************************************************
// struct name : LINKEDQAIRPORT
//
// Description : This struct is an object in a one side way queue, and it containe 
//				 Date: a runway
//				 Next: pointer to the next runway in the queue;
//
// Parameters:  PRUNWAY runway - pointer to the runway
//				struct _LINKEDQAIRPORT* next - pointer to the next linedQueue in the queue
//
//*************************************************************************************
typedef struct _LINKEDQAIRPORT
{
	PRUNWAY runway;
	struct _LINKEDQAIRPORT* next;

} LINKEDQAIRPORT, * PLINKEDQAIRPORT;

//**********************************************************************************
//Global pointer to the first runway in the Airport
//**********************************************************************************
PLINKEDQAIRPORT pHeadAirport = NULL;

//*************************************************************************************
// function name : findRunwayNode
//
// Description : Helper function - returns SUCCESS if it found a runway with the given number in the airport
//
// Parameters:  PLINKEDQAIRPORT pHeadAirport - the main Header to the airport
//				int runwayNum - the runway num
//
// Return value : SUCCESS if he find, else FAILURE
//*************************************************************************************
Result findRunwayNode(PLINKEDQAIRPORT pHeadAirport, int runwayNum)
{
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		if (searchNode->runway->num == runwayNum)
			return SUCCESS;
	}
	return FAILURE;
}
//*************************************************************************************
// function name : addRunway
//
// Description : add runway from the data, to the airport Queue
//
// Parameters:  int runwayNum - the runway number
//				FlightType runwayType - the flightType
//
// Return value : SUCCESS if he add the runway successfly, else FAILURE
//*************************************************************************************
Result addRunway(int runwayNum, FlightType runwayType)
{
	//check if runway is not already in the airport
	if (findRunwayNode(pHeadAirport, runwayNum) == SUCCESS)
		return FAILURE;
	//malloc memory to the runway
	PLINKEDQAIRPORT newRunwayNode = (PLINKEDQAIRPORT)malloc(sizeof(LINKEDQAIRPORT));
	if (newRunwayNode == NULL)
		return FAILURE;
	// add the runwayNode to the airport queue
	PRUNWAY newRunway = createRunway(runwayNum, runwayType);
	if (newRunway == NULL)
	{
		free(newRunwayNode);
		return FAILURE;
	}
	newRunwayNode->runway = newRunway;
	newRunwayNode->next = pHeadAirport;
	pHeadAirport = newRunwayNode;
	return SUCCESS;
}
//*************************************************************************************
// function name : checkDestFlight
//
// Description : help fanction, check if the dest is only 3 big laters (check if dest is ligal)
//
// Parameters:  char dest[4] - the dest of the flight
//
// Return value : SUCCESS if the dest is ligal, else FAILURE
//*************************************************************************************
Result checkDestFlight(char dest[4])
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
// function name : addFlightToAirport
//
// Description : get data to create a flight, and create it
//				 then it try to add it to the airport's runway as fallow
//					(1) priority 1: if the flight is emergency:
//									try to add it to the runway with the less most emergencyFlight in it
//					(2) priority 2: if not, or if the is multiple runway with the same number of emergencyFlight
//									look for the flight with the less most flight in it
//					(3) priority 3: if there is multiple of them as well
//									add it to the runway with the lowest number of the runway number
//
//					(4) egde case:	if there is no runway that can contain the flight (not with the same type), return FALIER
//
// Parameters:  int flightNum - the fligth number
//				FlightType flightType - the type of the runway it needed to be added
//				char dest[4] - the dest of the flight
//				BOOL emergency - hold if the flight is emergency or not
//
// Return value : SUCCESS if the added was successfull, else FAILURE
//*************************************************************************************
Result addFlightToAirport(int flightNum, FlightType flightType, char dest[4], BOOL emergency)
{
	if (flightNum > MAX_ID || flightNum < 1 || !checkDestFlight(dest))
		return FAILURE;
	int bestRunwayNum = -1, numRunways = 0;
	int curNumOfFlights, minNumOfFlights = -1; //starting value - negative value at the end means that no runway was found
	//checks if flight in airport
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next) //checks if flight exists in airport
	{
		if (findFlightInRunway((searchNode->runway), flightNum) == SUCCESS)
			return FAILURE;
	}
	//Find best runway
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		if (searchNode->runway->type != flightType)	//if runway is not correct type
			continue;
		//runways are correct type
		curNumOfFlights = getFlightNum(searchNode->runway);
		if (minNumOfFlights == -1) //if we havent found a valid runway yet
			minNumOfFlights = curNumOfFlights;
		else
		{
			if (curNumOfFlights < minNumOfFlights)
			{
				minNumOfFlights = curNumOfFlights;
			}
		}
	}
	//here we have minimum number of flights - not necessarily unqique
	//find runway with minimum flights
	if (minNumOfFlights == -1)
		return FAILURE; // if we get here, there are no suitable runways in the airport

	//count number of runways with minimum number of flights
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		if (searchNode->runway->type != flightType)	//if runway is not correct type
			continue;

		//runways are correct type
		curNumOfFlights = getFlightNum(searchNode->runway);
		if (curNumOfFlights == minNumOfFlights)
			++numRunways;
	}
	//here we have number of runways with minimal flights
	if (numRunways == 0)
		return FAILURE; //probably unncessary but safer
	if (numRunways == 1) //there is a runway with a unique number of min flights
	{
		//find the correct runway in the linked list Aipport
		for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
		{
			if (searchNode->runway->type != flightType)	//if runway is not correct type
				continue;
			if (getFlightNum(searchNode->runway) == minNumOfFlights) //if correct runway, create the flight, and add it to the runway
			{
				PFLIGHT newFlight = createFlight(flightNum, flightType, dest, emergency);
				if (newFlight == NULL)
					return FAILURE;
				return addFlight(searchNode->runway, newFlight); //returns Succes or Failure
			}
		}
	}

	//if we get here, there are multiple runways with minimum num of flights
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		if (searchNode->runway->type != flightType)	//if runway is not correct type
			continue;
		if (getFlightNum(searchNode->runway) == minNumOfFlights)
		{
			if (bestRunwayNum == -1)	//if we havent updated best runway yet
			{
				bestRunwayNum = searchNode->runway->num;
			}
			else
			{
				if ((searchNode->runway->num) < bestRunwayNum)
				{
					bestRunwayNum = searchNode->runway->num;
				}
			}
		}
	}
	if (bestRunwayNum == -1)
		return FAILURE;
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		if (searchNode->runway->type != flightType)	//if runway is not correct type
			continue;
		if (searchNode->runway->num == bestRunwayNum)
		{
			char tmp[4];
			tmp[0] = dest[0];
			tmp[1] = dest[1];
			tmp[2] = dest[2];
			tmp[3] = dest[3];
			PFLIGHT newFlight = createFlight(flightNum, flightType, tmp, emergency);
			if (newFlight == NULL)
				return FAILURE;
			//addFlight(searchNode->runway, newFlight); //I think this line is unnecessary because of next line
			return addFlight(searchNode->runway, newFlight); //returns Succes or Failure
		}
	}
	return FAILURE;
}

//*************************************************************************************
// function name : removeRunway
//
// Description : free and remove all the flight and the runway itself, from the airport.
//				 if there is no runway with that number in the airport, return FALIER
//
// Parameters:  int numRunway -  the number of the runway that witch to be remove
//
// Return value : SUCCESS if it remove it, else FAILURE
//*************************************************************************************
Result removeRunway(int numRunway)
{
	//cheach if numRunway in the airport
	if (pHeadAirport == NULL || findRunwayNode(pHeadAirport, numRunway) == FAILURE)
		return FAILURE;

	// if the removeing runway is the first
	if (pHeadAirport->runway->num == numRunway)
	{
		PLINKEDQAIRPORT linkedRuywayToRemove = pHeadAirport;
		pHeadAirport = pHeadAirport->next;
		destroyRunway(linkedRuywayToRemove->runway);
		free(linkedRuywayToRemove);
		return SUCCESS;
	}

	//search and remove the runway from the second link
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		// if the next runway is the one
		if (searchNode->next->runway->num == numRunway)
		{
			PLINKEDQAIRPORT linkedRuywayToRemove = searchNode->next;
			searchNode->next = searchNode->next->next;
			destroyRunway(linkedRuywayToRemove->runway);
			free(linkedRuywayToRemove);
			return SUCCESS;
		}
	}
	return FAILURE;
}

//*************************************************************************************
// function name : departAirport
//
// Description : will depart a fligth from the runway, as fallow 
//					(1) priority 1: look for the runway run with the must emergencyFlight
//					(2) priority 2: if there is multiple, look for the one with the must flight in it
//					(3) priority 3: if there is multiple, look for the runway with the lowest number
//					(4) priority 4: depart that runway
//
//					(5) egde case:	if there is no runway that can contain the flight, or there no runway. return FALIURE
//
// Parameters:  -nane-
//
// Return value : SUCCESS if he depart a flight, else FAILURE
//*************************************************************************************
Result departAirport()
{
	//edge case
	if (pHeadAirport == NULL)
		return FAILURE;

	// search for the number of the must emorgencyNum in the airport
	int emergencyMax = 0;
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		if (getEmergencyNum(searchNode->runway) > emergencyMax)
			emergencyMax = getEmergencyNum(searchNode->runway);
	}
	// search for the number of the must fligth with that emorgencyNumMax
	int maxNum = 0;
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		if (getFlightNum(searchNode->runway) > maxNum&& getEmergencyNum(searchNode->runway) == emergencyMax)
			maxNum = getFlightNum(searchNode->runway);
	}
	if (maxNum == 0 && emergencyMax == 0)
		return FAILURE;

	// now find the runway with that emorgencyFlight and raiular number, and with the lowest runwayNum
	int minRunwayNum = -1;
	PLINKEDQAIRPORT runwayToDepart = pHeadAirport;
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		if (minRunwayNum == -1 && getEmergencyNum(searchNode->runway) == emergencyMax && getFlightNum(searchNode->runway) == maxNum)
		{
			minRunwayNum = searchNode->runway->num;
			runwayToDepart = searchNode;
		}
		else if ((searchNode->runway->num < minRunwayNum) && getEmergencyNum(searchNode->runway) == emergencyMax && getFlightNum(searchNode->runway) == maxNum)
		{
			minRunwayNum = searchNode->runway->num;
			runwayToDepart = searchNode;
		}
	}
	if (minRunwayNum == -1)
		return FAILURE;

	//now depart the runway with that number
	return depart(runwayToDepart->runway);
}


//*************************************************************************************
// function name : doPrintAirport
//
// Description : help function, use Recursion to print the runway from the end of the queue to the start
//				 use this function only with the head of the airport as link
//
// Parameters:  PLINKEDQAIRPORT link - the link to the runway
//
// Return value : -nane-
//*************************************************************************************
void doPrintAirport(PLINKEDQAIRPORT link)
{
	// exit point
	if (link == NULL)
		return;

	// go deep first
	doPrintAirport(link->next);

	// then print this link
	printRunway(link->runway);
	return;
}


//*************************************************************************************
// function name : printAirport
//
// Description : print the runway in the airport, and the flight in them.
//				 it use Recursion to do so
//
// Parameters:  -nane- the program already have the head of the airport
//
// Return value : -nane-
//*************************************************************************************
void printAirport()
{
	printf("Airport status:\n");
	// the Recursion function
	doPrintAirport(pHeadAirport);

	printf("\n");
}

//*************************************************************************************
// function name : changeDest
//
// Description : check all the flight in the airport, with the preDest, to the newDest
//
// Parameters:  char preDest[4] - the dest we looking to change
//				char newDest[4] - the changed dest
//
// Return value : FALIUR only if the input is not ok
//*************************************************************************************
Result changeDest(char preDest[4], char newDest[4])
{
	// check edge case
	if (checkDestFlight(preDest) == FAILURE || checkDestFlight(newDest) == FAILURE)
		return FAILURE;

	// if input is ok, change all old dest into new ones
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		for (PLINKEDQUEUE searchRunway = searchNode->runway->headQueue; searchRunway != NULL; searchRunway = searchRunway->next)
		{
			if (searchRunway->flight->dest[0] == preDest[0] && searchRunway->flight->dest[1] == preDest[1] && searchRunway->flight->dest[2] == preDest[2] && searchRunway->flight->dest[3] == preDest[3])
			{
				// change the dest
				searchRunway->flight->dest[0] = newDest[0];
				searchRunway->flight->dest[1] = newDest[1];
				searchRunway->flight->dest[2] = newDest[2];
				searchRunway->flight->dest[3] = newDest[3];
			}
		}
	}
	return SUCCESS;
}

//*************************************************************************************
// function name : reEnterFlight
//
// Description : help fanction,  remove a flight with that number from the runway, and reenter it
//				 as help function. try and call it only if you sure the flight in the runway
//
// Parameters:  PRUNWAY runway - the runway the function work on,
//				int numFlight - the number of flight needed to be remove
//
// Return value : -nane-
//*************************************************************************************
void reEnterFlight(PRUNWAY runway, int numFlight)
{
	if (runway == NULL)
		return;
	//save a copy of the flight

	PFLIGHT tempFlight = NULL; //only a pointer, not needed to be free
	for (PLINKEDQUEUE searchRunway = runway->headQueue; searchRunway != NULL; searchRunway = searchRunway->next)
	{
		if (searchRunway->flight->num == numFlight)
		{
			tempFlight = searchRunway->flight;
		}
	}
	//read the flight
	if (tempFlight != NULL)
	{
		char tmp[4];
		tmp[0] = tempFlight->dest[0];
		tmp[1] = tempFlight->dest[1];
		tmp[2] = tempFlight->dest[2];
		tmp[3] = tempFlight->dest[3];
		PFLIGHT copyFlight = createFlight(tempFlight->num, tempFlight->type, tmp, tempFlight->emergency);
		removeFlight(runway, copyFlight->num);
		addFlight(runway, copyFlight);
	}
	return;
}

//*************************************************************************************
// function name : delay
//
// Description : delay all the flight in the runway, with that dest.
//				 keep emergency flight in ahead of all the Regular flight
//
// Parameters:  char dest[4] - the dest of the flight, we want to delay
//
// Return value : FALIUR only if the input is not ok
//*************************************************************************************
Result delay(char dest[4])
{
	// edge check
	if (checkDestFlight(dest) == FAILURE)
		return FAILURE;

	// re enter every flight with that destination
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		PLINKEDQUEUE searchRunway = searchNode->runway->headQueue;
		while (searchRunway != NULL)
		{
			if (searchRunway->flight->dest[0] == dest[0] && searchRunway->flight->dest[1] == dest[1] && searchRunway->flight->dest[2] == dest[2] && searchRunway->flight->dest[3] == dest[3])
			{
				PLINKEDQUEUE tempLink = searchRunway;
				searchRunway = searchRunway->next;
				reEnterFlight(searchNode->runway, tempLink->flight->num);
			}
			else
				searchRunway = searchRunway->next;
		}
	}

	// and re enter every flight with that desteny AGAIN! (inorder to flip the order)
	for (PLINKEDQAIRPORT searchNode = pHeadAirport; searchNode != NULL; searchNode = searchNode->next)
	{
		PLINKEDQUEUE searchRunway = searchNode->runway->headQueue;
		while (searchRunway != NULL)
		{
			if (searchRunway->flight->dest[0] == dest[0] && searchRunway->flight->dest[1] == dest[1] && searchRunway->flight->dest[2] == dest[2] && searchRunway->flight->dest[3] == dest[3])
			{
				PLINKEDQUEUE tempLink = searchRunway;
				searchRunway = searchRunway->next;
				reEnterFlight(searchNode->runway, tempLink->flight->num);
			}
			else
				searchRunway = searchRunway->next;
		}
	}

	return SUCCESS;
}


//*************************************************************************************
// function name : destroyAirport
//
// Description : destroy all the runway in the airport, and all there flight
//
// Parameters:  -nane-
//
// Return value : -nane-
//*************************************************************************************
void destroyAirport()
{
	while (pHeadAirport != NULL)
	{
		removeRunway(pHeadAirport->runway->num);
	}
}