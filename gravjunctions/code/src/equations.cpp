
// equations.cpp

// This contains all the model-specific equations

#include "equations.h"

void InitialConditions(struct FIELD *F, struct GRID *G){
	
	// Kink initial conditions
	if( G->params.init.type == G->ID.kink ){
		for(int com = 0; com < F->ncom; com++){					
			F->c = com;
			// Go get the array-index
			int ind = F->ai(F,G);
			double val = -1.0;
			if(G->i > 0.5 * G->imax) val = 1.0;
			F->vals[ind] = val;
	
		} // END com-loop
	}
	
	if( G->params.init.type == G->ID.simple ){
		for(int com = 0; com < F->ncom; com++){					
		
			F->c = com;
			// Go get the array-index
			int ind = F->ai(F,G);
		
			F->vals[ind] = G->i;
	
		} // END com-loop
	}
	
} // END InitialConditions()

// Function to compute the potential
void CalcV(struct FIELD *F, struct GRID *G){
	
	// Higgs potential
	if(G->params.pot.type == G->ID.HIGGSpot){
	
		double ModSq = 0.0;
		
		// Compute |phi|^2
		for(int c = 0; c < F->ncom; c++){
			F->c = c;
			ModSq += pow(F->vals[F->ai(F,G)],2.0);
		}
	
		F->V = 0.25 * pow(ModSq - 1.0, 2.0);
		
		
	} // END HIGGSPOT
	
	// Wess-Zumino potential
	if(G->params.pot.type == G->ID.WZpot){
	
		for(int c = 0; c < F->ncom; c++){
			F->c = c;
		}
		
		F->V = 0.0;
	}
	
} // END CalcV()

// Function to compute derivative of the potential
void CalcDV(struct FIELD *F, struct GRID *G){
	
	// Higgs potential
	if(G->params.pot.type == G->ID.HIGGSpot){
	
		double ModSq = 0.0;
		// Allocate some memory to hold a temporary value of the field
		// for easier reading of the code.
		double *field = new double [F->ncom];
		
		// Compute |phi|^2
		for(int c = 0; c < F->ncom; c++){
			F->c = c;
			field[c] = F->vals[F->ai(F,G)];
			ModSq += pow(field[c],2.0);
		}
	
		for(int c = 0; c < F->ncom; c++){
			F->c = c;
			// Now compute phi_i *(|phi|^2 - 1)
			
			F->Vp[F->c] = field[c] * (ModSq - 1.0);
		}
		
		// Delete that temporary memory
		delete field;
		
	} // END HIGGSPOT
	
	// Wess-Zumino potential
	if(G->params.pot.type == G->ID.WZpot){
	
		for(int c = 0; c < F->ncom; c++){
			F->c = c;
			// Compute the derivative of the potential, with respect to the given field component
			double dVdphi_i = 1.0;

			if(F->c == F->ID.RePhi){
				dVdphi_i = 0.0;
			}
			if(F->c == F->ID.ImPhi){
				dVdphi_i = 0.0;
			}
			if(F->c == F->ID.A){
				dVdphi_i = 0.0;
			}
			if(F->c == F->ID.nu){
				dVdphi_i = 0.0;
			}
	
			F->Vp[F->c] = dVdphi_i;
		}
		
	}
	
} // END CalcDV()

void ComputeEMT(struct FIELD *F, struct GRID *G){
	
	// Function to compute the mixed components of the energy-momentum tensor
	
	// Compute potential V(phi)
	F->pot(F,G);
	
	
	
	if( G->params.eom.name == "WaveEquation" ){
		
		double gradsq = 0.0;
		double gradsq_x = 0.0;
		double gradsq_y = 0.0;	
		double gradsq_xy = 0.0;
	
		for(int c = 0; c < F->ncom; c++){
			gradsq_x += pow(F->df[F->c + F->ID.dxID],2.0);
			gradsq_y += pow(F->df[F->c + F->ID.dyID],2.0);	
			gradsq_xy += F->df[F->c + F->ID.dxID] * F->df[F->c + F->ID.dyID];
		}
		
		F->emt.lagrangian_static = - 0.5 * ( gradsq_x + gradsq_y ) + F->V;
		F->emt.Ttt_static = 0.5 * gradsq + F->V;
		F->emt.Txx = - ( gradsq_x + F->emt.lagrangian_static );
		F->emt.Tyy = - ( gradsq_y + F->emt.lagrangian_static );	
		F->emt.Txy = - gradsq_xy;
		
	}
	
} // END ComputeEMT()

// Function to compute energy density
double CalcEnergyDensity(struct FIELD *F, struct GRID *G){
	
	double edens = 0.0;	
	
	if(G->params.eom.name == "WaveEquation")
		edens = F->emt.Ttt_static;
	
	return edens;
	
} // END CalcEnergyDensity

// Function to print field info to file.
void FieldDumpRoutine(struct FIELD *F, struct GRID *G){
	
	// These are the field items output to file,
	// at a given lattice location, at the given time-step.
	
	F->dump.fileout << G->i * G->params.h << " " << G->j * G->params.h << " ";

	for(int c = 0; c < F->ncom; c++){
		F->c = c;
		F->dump.fileout << F->vals[F->ai(F,G)] << " ";
	}

	F->dump.fileout << F->energydensity << " ";
	F->dump.fileout << F->emt.Txx << " " << F->emt.Tyy << " " << F->emt.Txy ;
	F->dump.fileout << endl;
	
	// The new-line at the end of a j-loop is printed
	// by the main driver function.
	
} // END FieldDump()

void CalcEoM(struct FIELD *F, struct GRID *G){

	if( G->params.eom.type == "WaveEquation" ){
	// Compute E_i (typically to set to phidot, or phidotdot)

		// E = \nabla^2\phi - dV/dphi_i
		for(int c=0; c < F->ncom; c++){
			F->c = c;
			F->E[F->c] = F->ddf[F->c + F->ID.dxID] + F->ddf[F->c + F->ID.dyID] - F->Vp[F->c];
		}
	
	}
	
	
	
} // END CalcE
