
// main.h


// main.h


#include <iostream>
#include <cmath>
#include <float.h>
#include <fstream>
#include <string>
#include "inireader.h"

using namespace std;
#include "grid.h"
#include "field.h"


#include <boost/filesystem.hpp> // Used for creating the output filenames
#include <boost/timer/timer.hpp>

void setup(int argc, char* argv[],struct FIELD *field, struct GRID *grid);
void printparams(ostream& whereto, struct FIELD *F, struct GRID *G);
void SolveFieldEquation(struct FIELD *field, struct GRID *grid);

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// EOF