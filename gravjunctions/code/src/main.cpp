
// main.cpp

#include "main.h"

int main(int argc, char* argv[]){
	

	// Boot up the structs that will hold the field
	// and grid info
	FIELD field;
	GRID grid;
	
	// Setup the parameters, field, & grid structs
	setup(argc, argv, &field, &grid);
	printparams(cout, &field, &grid);
	
	// Set the initial conditions for the field
	field.SetICS(&field, &grid);
	
	// Solve the field equation
	SolveFieldEquation(&field, &grid);
	
	// Cleanup the memory 
	field.cleanup(&field);
	grid.cleanup(&grid);
	
	
} // END main()

