
// analysis.cpp

#include "analysis.h"

void SiteAnalysisDriver(struct FIELD *F, struct GRID *G){
	
	// Compute components of energy-momentum tensor
	ComputeEMT(F,G);
	
	// Compute energy density
	F->energydensity = CalcEnergyDensity(F,G);
	// add it to the integrated energy
	F->energy += F->energydensity;
	F->energy *= G->params.h*G->params.h;
	
} // END Analysis
