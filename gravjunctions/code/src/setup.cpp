
// setup.cpp

#include "setup.h"

void setup(int argc, char* argv[], struct FIELD *F, struct GRID *G){
	
	setupID(F,G);
	
	getparams(argc, argv, F, G);
	
	
	// Serious setup of arrays and identifiers
	F->vals = new double [ 2 * F->ncom * G->imax * G->jmax ];
	F->df = new double [ 2 * F->ncom ];
	F->ddf = new double [ 2 * F->ncom ];
	F->loc = new int[4];
	F->E = new double[ F->ncom ];
	F->Vp = new double[ F->ncom ];	
	
	F->FileCounter = 100000;
	
	G->params.hh = pow(G->params.h,2.0);
	G->params.h2 = 2.0 * G->params.h;
	G->params.htht = pow(G->params.htht, 2.0);
	
	G->T.timestep = new double[G->params.freq.time];
	G->T.time = new double[G->params.freq.time];
	G->T.energy = new double[G->params.freq.time];
	
	checkdirexists(G->params.OutDir);
	
} // END setup()

void getparams(int argc, char* argv[], struct FIELD *F, struct GRID *G){
	
  	// Get the parameter file:
    IniReader inifile;
    // If the user specified a param file at runtime
    //  then use that, else, use the default, params.ini
    //   (User specifies via ./EXE my_param.par)
    
    if (argc > 1)
        inifile.read(argv[1]);
    else
        inifile.read("params.ini");
	
	// User defined parameters:
	F->ncom = int(inifile.getiniDouble("ncom",1));
	G->imax = int(inifile.getiniDouble("imax",10));
	G->jmax = int(inifile.getiniDouble("jmax",10));
	G->ntimesteps = int(inifile.getiniDouble("ntimesteps",10));
	G->params.ht = inifile.getiniDouble("ht",0.5);
	G->params.h = inifile.getiniDouble("h",0.1);
	
	G->params.str.OutDir = inifile.getiniString("OutDir","out/");	
	G->params.str.RunID = inifile.getiniString("RunID","run");
	G->params.str.Field = inifile.getiniString("FieldFile","field");
	G->params.str.Log = inifile.getiniString("LogFile","log");

	string UsersBCType = inifile.getiniString("BCType","periodic");	
	string UsersEvolType = inifile.getiniString("EvolType","GradientFlow");
	string UsersPotType = inifile.getiniString("PotType","HiggsPotential");
	string UsersInitType = inifile.getiniString("InitType","simple");
	string UsersEOMType =  inifile.getiniString("EoMtype","WaveEquation");
	
	G->params.freq.time = int(inifile.getiniDouble("timedumpfreq",10));
	G->params.freq.file = int(inifile.getiniDouble("filedumpfreq",10));
	G->params.freq.screen = int(inifile.getiniDouble("screendumpfreq",10));
	
	if(UsersBCType == "periodic") {
		G->params.bc.type = G->ID.periodic;
		G->params.bc.name = G->ID.periodicName;
	}
	
	if(UsersBCType == "fixed") {
		G->params.bc.type = G->ID.fixed;
		G->params.bc.name = G->ID.fixedName;
	}
	
	// Setup initial condition types
	if(UsersInitType == "simple") {
		G->params.init.type = G->ID.simple;
		G->params.init.name = G->ID.simpleName;
	}
	
	if(UsersInitType == "kink") {
		G->params.init.type = G->ID.kink;
		G->params.init.name = G->ID.kinkName;
	}
	
	if(UsersEOMType == "WaveEquation") {
		G->params.eom.type = G->ID.WaveEqn;
		G->params.eom.name = G->ID.WaveEqnName;
	}
	
	if(UsersPotType == "HiggsPotential" || UsersPotType == "Higgs"){
		G->params.pot.type =  G->ID.HIGGSpot;
		G->params.pot.name =  G->ID.HiggsPotName;
	}
	if(UsersPotType == "WZ" || UsersPotType == "WessZumino"){
		G->params.pot.type =  G->ID.WZpot;
		G->params.pot.name =  G->ID.WZPotName;
	}
	
	if(UsersEvolType == "GradientFlow"){
		G->params.evol.type =  G->ID.GradientFlow;
		G->params.evol.name =  G->ID.GFName;
	}
	if(UsersEvolType == "SecondOrderEvolution"){		
		G->params.evol.type =  G->ID.SecondOrderEvolution;
		G->params.evol.name =  G->ID.SOEName;
	}
	
}

void setupID(struct FIELD *F, struct GRID *G){
	
	int ID;
	
	// This function sets up all the identifiers
	
	ID = 0;
	F->ID.dxID = ID; ID++;
	F->ID.dyID = ID; ID++;
	
	ID = 0;
	G->ID.periodic = ID; ID ++;
	G->ID.fixed = ID; ID ++;
	
	ID = 0;
	G->ID.GradientFlow = ID; ID++;
	G->ID.SecondOrderEvolution = ID; ID ++;

	ID = 0;
	G->ID.HIGGSpot = ID; ID++;
	G->ID.WZpot = ID; ID ++;
	
	ID = 0;
	G->ID.simple = ID; ID++;
	G->ID.kink = ID; ID ++;
	
	ID = 0;
	G->ID.WaveEqn = ID; ID ++;
	
	G->ID.simpleName = "simple";
	G->ID.kinkName = "kink";
	G->ID.HiggsPotName = "Higgs";
	G->ID.WZPotName = "WessZumino";	
	G->ID.SOEName = "Second order evolution";
	G->ID.GFName = "GradientFlow";
	G->ID.WaveEqnName = "WaveEquation";
	G->ID.periodicName = "periodic";
	G->ID.fixedName = "fixed";
	
	ID = 0;
	F->ID.RePhi = ID; ID++;
	F->ID.ImPhi = ID; ID++;
	F->ID.A = ID; ID++;
	F->ID.nu = ID; ID ++;
	
}

void checkdirexists(string dir){
    
    using namespace boost::filesystem;
    
    if (!exists(dir + "/")) {
        cout << endl;
        cout << " --> Creating output directory" << endl;
        cout << endl;
        
        create_directory(dir);
    }
    
} // END checkdirexists()