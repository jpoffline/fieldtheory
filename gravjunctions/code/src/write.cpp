
// write.cpp

#include "write.h"

void printparams(ostream& whereto, struct FIELD *F, struct GRID *G){
	
	whereto << "h = " << G->params.h << endl;
	whereto << "ht = " << G->params.ht << endl;
	whereto << "ncom = " << F->ncom << endl;	
	whereto << "imax = " << G->imax << endl;		
	whereto << "jmax = " << G->jmax << endl;		
	whereto << "ntimesteps = " << G->ntimesteps << endl;
	whereto << "BCType = " << G->params.bc.type << " :: " << G->params.bc.name << endl;
	whereto << "InitType = " << G->params.init.type << " :: " << G->params.init.name << endl;
	whereto << "EoMType = " << G->params.eom.type << " :: " << G->params.eom.name << endl;
	whereto << "PotType = " << G->params.pot.type << " :: " << G->params.pot.name << endl;
	whereto << "EvolType = " << G->params.evol.type << " :: " << G->params.evol.name << endl;
	
}  // END printparams()

void DumpToScreen(struct FIELD *F, struct GRID *G){
	
	if(G->timestep%G->params.freq.screen == 0){
		
		// Output %-progress
		cout << "(" << double(G->timestep) / double(G->ntimesteps) * 100 << "%) ";
		
		// Need to get the correct array element of the timehistory items
		int WhichID = G->timestep%G->params.freq.time;
		
		// dump the time-history items to screen 
		G->T.write(cout, F, G, WhichID);
		
	}	
	
} // END DumpToScreen()