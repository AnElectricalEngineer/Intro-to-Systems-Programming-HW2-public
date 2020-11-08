#ifndef HW2_AIRPORT_H
#define HW2_AIRPORT_H

#include "ex2.h"
#include "flight.h"
#include "runway.h"

//create an airport(PLINKEDQAIRPORT headQueue)
typedef struct _LINKEDQAIRPORT LINKEDQAIRPORT, * PLINKEDQAIRPORT;

// interface functions

//*************************************************************************************
// function name : addRunway
//
// Description : add runway from the data, to the airport Queue
//
// Parameters:  int runwayNum - the runway number
//				FlightType runwayType - the flightType
//
// Return value : SUCCESS if he added the runway successfully, else FAILURE
//*************************************************************************************
Result addRunway(int, FlightType);

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
Result addFlightToAirport(/*PLINKEDQAIRPORT, */int, FlightType, char[4], BOOL);

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
void printAirport();

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
Result removeRunway(int);

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
Result departAirport();

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
Result changeDest(char[4], char[4]);

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
Result delay(char[4]);

//*************************************************************************************
// function name : destroyAirport
//
// Description : destroy all the runway in the airport, and all there flight
//
// Parameters:  -nane-
//
// Return value : -nane-
//*************************************************************************************
void destroyAirport();

#endif //HW2_AIRPORT_H
