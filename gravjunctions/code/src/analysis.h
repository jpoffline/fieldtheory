
// analysis.h

#include <iostream>
#include <cmath>
#include <float.h>
#include <fstream>
#include <string>

using namespace std;
#include "grid.h"
#include "field.h"

double CalcEnergyDensity(struct FIELD *F, struct GRID *G);
void ComputeEMT(struct FIELD *F, struct GRID *G);