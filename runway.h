#ifndef HW2_RUNWAY_H
#define HW2_RUNWAY_H

#include "ex2.h"
#include "flight.h"

typedef struct _LINKEDQUEUE
{
	PFLIGHT flight;
	struct _LINKEDQUEUE* next;

} LINKEDQUEUE, * PLINKEDQUEUE;

typedef struct _RUNWAY
{
	int num;
	FlightType type;
	PLINKEDQUEUE headQueue; // pointer to the linked Queue

} RUNWAY, * PRUNWAY;

// interface functions
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
PRUNWAY createRunway(int, FlightType);
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
Result addFlight(PRUNWAY, PFLIGHT);
//************************************************************************************* 
//function name : depart
//
// Description : This function removes the first (last added) flight from a runway
//
// Parameters: runway: a pointer to the runway that we want a plane to depart from
//
// Return value : SUCCESS if a plane successfully departed, FAILURE if not
//*************************************************************************************
Result depart(PRUNWAY);
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
Result removeFlight(PRUNWAY, int);
//************************************************************************************* 
//function name : destroyRunway
//
// Description : This function destroys an entire runway, and all the flights on it
//
// Parameters: runway: a pointer to the runway that we want to remove
//			   
// Return value : none
//*************************************************************************************
void destroyRunway(PRUNWAY);
//************************************************************************************* 
//function name : getFlightNum
//
// Description : This function gets the number of flights waiting on a runway
//
// Parameters: runway: a pointer to the runway whose flights we want to count
//			   
// Return value : the number of flights on the runway, -1 if error
//*************************************************************************************
int getFlightNum(PRUNWAY);
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
BOOL isFlightExists(PRUNWAY, int);
//************************************************************************************* 
//function name : getEmergencyNum
//
// Description : This function counts the number of emergency flights on a runway
//
// Parameters: runway: a pointer to the runway whose emergency flights we want to count
//			   
// Return value : the number of emergency flights waiting, -1 if error.
//*************************************************************************************
int getEmergencyNum(PRUNWAY);
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
void printRunway(PRUNWAY);

//helper functions
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
Result findFlightInRunway(PRUNWAY, int);
#endif //HW2_RUNWAY_H
