
// setup.h

#include <iostream>
#include <cmath>
#include <float.h>
#include <fstream>
#include <string>
#include "inireader.h"

using namespace std;
#include "grid.h"
#include "field.h"

void checkdirexists(string dir);
void setupID(struct FIELD *F, struct GRID *G);
void getparams(int argc, char* argv[], struct FIELD *F, struct GRID *G);
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// EOF