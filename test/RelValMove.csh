#!/bin/tcsh

set val_rel=$1

mv TTbar_Startup/DQM_V0001_R000000001__POG__BTAG__BJET.root    BTagRelVal_TTbar_Startup_${val_rel}.root
mv TTbar_Startup_PU/DQM_V0001_R000000001__POG__BTAG__BJET.root BTagRelVal_TTbar_Startup_PU_${val_rel}.root
mv TTbar_FastSim/DQM_V0001_R000000001__POG__BTAG__BJET.root    BTagRelVal_TTbar_FastSim_${val_rel}.root
mv QCD_Startup/DQM_V0001_R000000001__POG__BTAG__BJET.root      BTagRelVal_QCD_Startup_${val_rel}.root
mv DATA/DQM_V0001_R000000001__POG__BTAG__BJET.root             BTagRelVal_DATA_${val_rel}.root
