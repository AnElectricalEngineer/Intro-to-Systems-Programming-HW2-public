#ifndef HW2_FLIGHT_H
#define HW2_FLIGHT_H

#include "ex2.h"

//create a flight
typedef struct _FLIGHT
{
	int num;
	FlightType type;
	char dest[4];
	BOOL emergency;
} FLIGHT, * PFLIGHT;

// interface functions
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
PFLIGHT createFlight(int, FlightType, char[4], BOOL);

//************************************************************************************* 
//function name : destroyFlight
//
// Description : This function destroys an existing flight
//
// Parameters:  flight: a pointer to a flight struct
//
// Return value : None
//*************************************************************************************
void destroyFlight(PFLIGHT);

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
void printFlight(PFLIGHT);

#endif //HW2_FLIGHT_H
