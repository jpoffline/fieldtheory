
// grid.h

#ifndef GRID_H
#define GRID_H

struct GRID{

	int imax, jmax;
	int timestep,t,tp, tm;
	int i, j, ip, im, jp, jm;
	int ntimesteps;
	
	#include "params.h"	
	struct PARAMETERS params;
	
	#include "ids_g.h"
	struct IDENTIFIERS  ID;
	
	#include "timehistory.h"
	struct TIMEHISTORY T;
	
	
	void SetTime(struct GRID *G, int timestep){
		
		G->timestep = timestep;
		G->t=timestep%2;
		G->tp=0;

		if(G->t == 0) G->tp = 1;
		G->tm = G->tp;
		
	} // END SetTime()
	
	void SetLocation(struct GRID *G, int i, int j){
		
		G->i = i;
		G->j = j;
		G->ip = i+1;
		G->im = i-1;
		G->jp = j+1;
		G->jm = j-1;
		// Set periodic boundaries
		if( G->params.bc.name == "periodic" ){
			if( i == 0 ) G->im = G->imax - 1;
			if( j == G->jmax - 1 ) G->jp = 0;
			if( i == G->imax - 1 ) G->ip = 0;
			if( j == 0 ) G->jm = G->jmax - 1;
		}
	} // END SetLocation
	
	void cleanup(struct GRID *G){
		
		delete G->T.timestep;
		delete G->T.time;
		delete G->T.energy;
		
	} // END cleanup()
	
};

#endif