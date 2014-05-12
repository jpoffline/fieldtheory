
// solve.cpp

#include "solve.h"


void SolveFieldEquation(struct FIELD *F, struct GRID *G){
	
	// Set the evolution region: important if using fixed boundaries
	SetEvolRegion(G);
	G->T.SetupTimeHistory(G);
	
	
	
	for(int timestep = 0; timestep < G->ntimesteps; timestep++){
		
		G->SetTime(G, timestep);
		F->energy = 0.0;
		
		// Setup the field dump file, but only
		// at the correct multiple of freq.file.
		F->SetupFieldDumpFile(F,G);
		
		for(int i = G->params.evol.imin; i < G->params.evol.imax; i++){		
			for(int j = G->params.evol.jmin; j < G->params.evol.jmax; j++){
				G->SetLocation(G,i,j);
				
				// Call driver routine to solve the field equation.
				SiteSolveDriver(F,G);
				
				// Call analysis routines at this location and time-step.
				SiteAnalysisDriver(F,G);
				
				// Dump the field (only happens at the correct time-step)
				F->FieldDump(F,G);
				
			} // END j-loop
		} // END i-loop
		
		// Clean up the file dumping
		F->ResetFieldDumpFile(F);
		
		// Set the time-history
		SetTimeHistory(G, F);
		
		DumpToScreen(F,G);
		
		
				
		
	} // END timestep-loop
	
	G->T.ShutDownTimeHistory(G);
	
	
	
} // END SolveFieldEquation()


void SiteSolveDriver(struct FIELD *F, struct GRID *G){
	
	
	// Compute the derivatives of the potential,
	// used to solve the equation of motion.
	F->dpot(F,G);

	// Compute field derivatives
	for(int com = 0; com < F->ncom; com++){					
		F->c = com;
		// Get the info at this grid
		// location required to solve the field equations
		F->GetDerivs(F,G);
	} // END com-loop
	
	// Compute E(i)
	F->CalcE(F,G);
	// (2) Now update field value, by setting
	// dot(phi)_i = E_i, or
	// ddot(phi)_i = E_i.
	for(int com = 0; com < F->ncom; com++){
		F->c = com;
		F->UpdateField(F,G);
	}
	
	
} // END SiteSolveDriver()

void SetEvolRegion(struct GRID *G){
	
	if (G->params.bc.name == "periodic"){
		
		G->params.evol.imin = 0;
		G->params.evol.jmin = 0;
		G->params.evol.imax = G->imax;
		G->params.evol.jmax = G->jmax;
		
	}
	
	if (G->params.bc.name == "fixed"){
		
		G->params.evol.imin = 1;
		G->params.evol.jmin = 1;
		G->params.evol.imax = G->imax-1;
		G->params.evol.jmax = G->jmax-1;
		
	}
	
} // END SetEvolRegion()

void SetTimeHistory(struct GRID *G, struct FIELD *F){

	// Populate the time-history items
	
	G->T.timestep[G->T.ID_TimeHist] = G->timestep;
	G->T.time[G->T.ID_TimeHist] = G->timestep * G->params.ht;
	G->T.energy[G->T.ID_TimeHist] = F->energy;
	
	// Go and try to dump the timehistory items;
	bool testtime = G->T.DumpTime(F,G);
	
	// if we dumped timehistory, need to reset counter.
	if(testtime) G->T.ID_TimeHist = -1;
	
	// But, we always increment counter
	G->T.ID_TimeHist++;
	
} // END SetTimeHistory()