#include <iostream>
#include <G4RunManager.hh>
#include <G4UIExecutive.hh>
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UImanager.hh>
#include <QGSP_BERT.hh>

#include "MyDetectorConstruction.hh"
#include "MyPrimaryGenerator.hh"


int main(int argc, char** argv) {
	G4RunManager *runManager = new G4RunManager();
	runManager->SetUserInitialization(new MyDetectorConstruction());
	runManager->SetUserInitialization(new QGSP_BERT());//new MyPhysicsList()
	runManager->SetUserAction(new MyPrimaryGenerator());
	runManager->Initialize();
			
	G4UIExecutive *ui = nullptr;
	if (argc == 1) {
		ui = new G4UIExecutive(argc, argv);	
	}
	
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();
	
	G4UImanager *uiManager = G4UImanager::GetUIpointer();
	if (ui) {
		uiManager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
	} else {
		std::string commandStr = "/control/execute ";
		std::string parameterStr = argv[1];
		uiManager->ApplyCommand(commandStr + parameterStr);	 
	}
	 
	delete visManager;
  	delete runManager;
  	
	return 0;
}
