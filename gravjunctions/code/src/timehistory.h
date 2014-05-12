
// timehistory.h

#ifndef TIMEHISTORY_H
#define TIMEHISTORY_H

void TimeInfoDump(ostream& whereto, struct FIELD *F, struct GRID *G, int HowMany);

struct TIMEHISTORY{

	double *timestep;
	double *time;
	double *energy;
	int ID_TimeHist;
	
	struct THIST{
		ofstream out;
		string filename;
	}timehist;
	
		
	void SetupTimeHistory(struct GRID *G){
		
		G->T.ID_TimeHist = 0;
		G->T.timehist.filename = G->params.str.OutDir +  G->params.str.RunID + "_timehistory.log";
		G->T.timehist.out.open(G->T.timehist.filename);
		
	} // END SetupTimeHistory()
	
	void ShutDownTimeHistory(struct GRID *G){
		G->T.timehist.out.close();
	} // END ShutDownTimeHistory();
		
	bool DumpTime(struct FIELD *F, struct GRID *G){
		
		// By default, dont write the time-history
		bool ret = false;
		
		// If we are writing the time-history,
		// do that now:
		if(G->timestep%G->params.freq.time == 0 && G->timestep != 0){
			TimeInfoDump(G->T.timehist.out, F, G, G->params.freq.time);
			ret = true;
		}	
		
		// At the end of the simulation, dump everything
		if(G->timestep == G->ntimesteps-1)
			TimeInfoDump(G->T.timehist.out, F, G, G->T.ID_TimeHist+1);
		
			
		return ret;
		
	} // END DumpTime()

	void TimeInfoDump(ostream& whereto, struct FIELD *F, struct GRID *G, int HowMany){
		
		for(int ID = 0; ID < HowMany; ID++)
			G->T.write(whereto, F, G, ID);
		
		
	} // END TimeInfoDump()

	void write(ostream& whereto, struct FIELD *F, struct GRID *G, int ID){
		
		whereto << G->T.timestep[ID] << " " << G->T.time[ID] << " " << G->T.energy[ID] << endl;
		
	} // END writewrite()

};

#endif