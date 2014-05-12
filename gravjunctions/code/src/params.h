
// params.h


#ifndef PARAMETERS_H
#define PARAMETERS_H

struct PARAMETERS{
	
	double h,ht,h2,hh,htht;
	string OutDir;
	string FieldFilePrefix;
	
	struct STRINGS{
		string OutDir;
		string RunID;
		string Field;
		string Log;
	}str;

	struct BCS{
		int type;
		string name;
	}bc;

	struct DUMPFREQ{
		int time;
		int file;	
		int screen;
	} freq;
	
	struct EV{
		int type;
		string name;
		int imin;
		int jmin;
		int imax;
		int jmax;
	} evol;
	
	struct EOM{
		int type;
		string name;
	} eom;
	
	struct POT{
		int type;
		string name;
	} pot;
	
	struct INIT{
		int type;
		string name;
	} init;
	
};

#endif

