
// solve.h

#include <iostream>
#include <cmath>
#include <float.h>
#include <fstream>
#include <string>

using namespace std;
#include "grid.h"
#include "field.h"

void SetEvolRegion(struct GRID *G);
void SiteSolveDriver(struct FIELD *F, struct GRID *G);
void SiteAnalysisDriver(struct FIELD *F, struct GRID *G);
double CalcEnergyDensity(struct FIELD *F, struct GRID *G);
void SetTimeHistory(struct GRID *G, struct FIELD *F);
void DumpToScreen(struct FIELD *F, struct GRID *G);

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// EOF