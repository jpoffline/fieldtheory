
// field.h

#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <cmath>
#include <float.h>
#include <fstream>
#include <string>

using namespace std;

void CalcDV(struct FIELD *F, struct GRID *G);
void CalcV(struct FIELD *F, struct GRID *G);
void CalcEoM(struct FIELD *F, struct GRID *G);
void InitialConditions(struct FIELD *F, struct GRID *G);
double CalcEnergyDensity(struct FIELD *F, struct GRID *G);
void FieldDumpRoutine(struct FIELD *F, struct GRID *G);

struct FIELD{
	
	double *vals, *df, *ddf, *E, V, *Vp;
	int c, ncom;
	int FileCounter;
	int *loc;

	double energy,energydensity;
	
	#include "ids_f.h"
	struct IDENTIFIERS_F ID;
	
	struct ENERGYMOMENTUM{
		double lagrangian;
		double lagrangian_static;
		double Ttt;
		double Ttt_static;
		double Txx;
		double Tyy;
		double Txy;
	}emt;
	
	struct FIELDDUMP{
		bool fielddump;
		string filename;
		ofstream fileout;
	}dump;
	
	// Array location for this given lattice site
	int ai(struct FIELD *F, struct GRID *G){
		
		return F->ArrayIndex(F,G,G->t,G->i,G->j);
		
	} // END ai()
	
	// Array location for any lattice site
	int al(struct FIELD *F, struct GRID *G, int i, int j){

		return F->ArrayIndex(F,G,G->t,i,j);
		
	} // END al()
	
	// Array location for any lattice site, at any time
	int ArrayIndex(struct FIELD *F, struct GRID *G, int t, int i, int j){

		return t * F->ncom * G->imax * G->jmax
			   + F->c * G->imax * G->jmax
			   + i * G->jmax
			   + j;
		
	} // END alt()
	
	// Function to set the initial conditions at the given lattice site
	void SetICS(struct FIELD *F, struct GRID *G){
		
		for(int timestep = 0; timestep < 2; timestep++){
			G->t = timestep%2;
		
			for(int i = 0; i < G->imax; i++){
				G->i = i;
			
				for(int j = 0; j < G->jmax; j++){
					G->j = j;
				
					InitialConditions(F,G);
					
				} // END j-loop
			} // END i-loop
		} // END timestep-loop
		
	} // END SetICS()
	
	// Function to get the finite difference derivatives
	void GetDerivs(struct FIELD *F, struct GRID *G){
		// Get the first and second derivatives,
		// using finite difference
		double f = F->vals[F->al(F,G,G->i,G->j)];
		double f2 = 2.0 * f;
		double fip = F->vals[F->al(F,G,G->ip,G->j)];
		double fim = F->vals[F->al(F,G,G->im,G->j)];
		double fjp = F->vals[F->al(F,G,G->i,G->jp)];
		double fjm = F->vals[F->al(F,G,G->i,G->jm)];			
		
		F->df[F->c + F->ID.dxID] = (fip - fim) / G->params.h2;
		F->df[F->c + F->ID.dyID] = (fjp - fjm) / G->params.h2;		
		F->ddf[F->c + F->ID.dxID] = (fip + fim - f2) / G->params.hh;
		F->ddf[F->c + F->ID.dyID] = (fjp + fjm - f2) / G->params.hh;		
		
	} // END GetDerivs
	
	// Function to compute E
	void CalcE(struct FIELD *F, struct GRID *G){
		
		// Go call the CalcEoM function in equations.cpp
		CalcEoM(F,G);
		
	} // END CalcE()
	
	void dpot(struct FIELD *F, struct GRID *G){
		
		// Go call the CalcDV function in equations.cpp		
		CalcDV(F,G);
		
	} // END dpot()
	
	void pot(struct FIELD *F, struct GRID *G){
		
		// Go call the CalcV function in equations.cpp		
		CalcV(F,G);
		
	} // END pot()
	
	void UpdateField(struct FIELD *F, struct GRID *G){
		
		double Field_now = F->vals[F->ArrayIndex(F,G, G->t, G->i, G->j)];
		double Field_previous = F->vals[F->ArrayIndex(F,G, G->tm, G->i, G->j)];
		double Field_next;
		double EOM = F->E[F->c];
		
		if(G->params.evol.type == G->ID.GradientFlow)
			Field_next = EOM * G->params.ht + Field_now;
		
		
		if(G->params.evol.type == G->ID.SecondOrderEvolution)
			Field_next = EOM * G->params.htht - Field_previous + 2.0 * Field_now;
		
		
		F->vals[F->ArrayIndex(F,G, G->tp, G->i, G->j)] = Field_next;
		
	} // END UpdateField()
	
	void SetupFieldDumpFile(struct FIELD *F, struct GRID *G){
		
		
		// By default, we dont want to output to file.
		F->dump.fielddump = false;
		
		// But, if the time-step is the correct multiple
		// of the freq.file, we should output.
		if(G->timestep%G->params.freq.file == 0 || G->timestep == G->ntimesteps - 1){
			F->dump.fielddump = true;
		}
		
		// If we are outputting, need to get the file name & open it
		if(F->dump.fielddump){
			// Get the field-file name
			F->dump.filename = G->params.str.OutDir +  G->params.str.RunID + "_" + to_string(F->FileCounter) + "." + G->params.str.Field;
			if(G->timestep == G->ntimesteps - 1)
				F->dump.filename = G->params.str.OutDir +  G->params.str.RunID + "_final." + G->params.str.Field;
			
			// Open up the file to output to
			F->dump.fileout.open(F->dump.filename);

		}
		
		
	} // END SetupFieldDumpFile()

	void FieldDump(struct FIELD *F, struct GRID *G){
		
		if(F->dump.fielddump){
			// Go call the field dump routine,
			// in equations.cpp
			FieldDumpRoutine(F,G);
			// Put a new-line if at the end of a j-loop
			if(G->j == G->jmax - 1) F->dump.fileout << endl;
		}
		
	} // END FieldDump()

	void ResetFieldDumpFile(struct FIELD *F){
		
		if(F->dump.fielddump){
			// Close the output file
			F->dump.fileout.close();
			// Increment file counter for next output
			F->FileCounter++;
		}
		
	} // END ResetFieldDumpFile()
		
	void cleanup(struct FIELD *F){
		
		delete F->vals;
		delete F->loc;
		delete F->df;
		delete F->ddf;
		delete F->E;
		delete F->Vp;
		
	} // END cleanup()
};

#endif