
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>

//Root 
#include <TFile.h>          
#include <TTree.h>

//GEANT4 
#include "globals.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryContainer.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4Run.hh"
#include "G4Proton.hh"
#include "G4HadronicProcessStore.hh"
#include "G4NistManager.hh"

#include "HadAnalysis.hh"

using namespace std;

HadAnalysis* HadAnalysis::instance = 0;

//------------------------------------------------------------------------------------
HadAnalysis::HadAnalysis()
{
#ifdef G4ANALYSIS_USE
#endif
 }
//------------------------------------------------------------------------------------
HadAnalysis::~HadAnalysis()
{ 


#ifdef G4ANALYSIS_USE
#endif
}
//------------------------------------------------------------------------------------
HadAnalysis* HadAnalysis::getInstance()
{
  if (instance == 0) instance = new HadAnalysis;
  return instance;
}
//------------------------------------------------------------------------------------
void HadAnalysis::book(G4long id0,G4long id1)
{
    sprintf(NtupleFileName,"ntuple_%04d.root",int(id0));

    G4cout<<"Random Seeds: "<<id0<<" "<<id1<<G4endl;

    FileNtuple = new TFile(NtupleFileName, "RECREATE","hadronic interactions hadron-Target");
   
    ProdTree = new TTree("nTarget","neutron_Target info");
    ProdTree->Branch("npart",&g4Proddata.NPart,"NPart/I");
    ProdTree->Branch("pdg", &g4Proddata.PDG,"PDG[NPart]/I");
    ProdTree->Branch("kinEn", &g4Proddata.kinEn,"kinEn[NPart]/D");
    ProdTree->Branch("intType", &g4Proddata.InterType,"IntType[NPart]/I");
    ProdTree->Branch("x",  &g4Proddata.X,"X[NPart][3]/D");
    ProdTree->Branch("p",  &g4Proddata.P,"P[NPart][4]/D");
    ProdTree->Branch("enerPrimGen",&enerPrimGen,"enerPrimGen/D");
    
    //Get and store general information about this run: 
    HeaderTree = new TTree("RunInfo","Run Info");
    HeaderTree->Branch("inelasticXS",&inelasticXS,"inelXS/D");
    HeaderTree->Branch("elasticXS",&elasticXS,"elXS/D");
    HeaderTree->Branch("tickness",&tickness,"tickness/D");
    HeaderTree->Branch("height",&height,"height/D");
    HeaderTree->Branch("width",&width,"width/D");
    HeaderTree->Branch("density",&rho,"density/D");
    HeaderTree->Branch("material",&material);
    HeaderTree->Branch("enerPrimGen",&enerPrimGen,"enerPrimGen/D");
    HeaderTree->Branch("PartName",&PartName);
    HeaderTree->Branch("numberEvts",&numberEvts,"numberEvts/I");
}


//------------------------------------------------------------------------------------
void HadAnalysis::finish()
{
    FileNtuple->cd();
     
    ProdTree->Write();
    WriteHeader();
    FileNtuple->Close();
    //delete FileNtuple;
}

//------------------------------------------------------------------------------------

void HadAnalysis::FillNtuple(std::vector<TrackInfo_t> trackInfoVec)
{
 g4Proddata.NPart= trackInfoVec.size();
 
 Int_t partNum = 0;

 std::vector<TrackInfo_t>::iterator iteTrackInfo = trackInfoVec.begin();
 for(;iteTrackInfo != trackInfoVec.end();iteTrackInfo++){  

   g4Proddata.PDG[partNum] = (*iteTrackInfo).PDGcode;
   g4Proddata.InterType[partNum] = (*iteTrackInfo).interType;
   g4Proddata.kinEn[partNum] = (*iteTrackInfo).kinEn;
   
   g4Proddata.X[partNum][0]= (*iteTrackInfo).Pos.X();
   g4Proddata.X[partNum][1]= (*iteTrackInfo).Pos.Y();
   g4Proddata.X[partNum][2]= (*iteTrackInfo).Pos.Z();

   g4Proddata.P[partNum][0]= (*iteTrackInfo).Mom.X();
   g4Proddata.P[partNum][1]= (*iteTrackInfo).Mom.Y();
   g4Proddata.P[partNum][2]= (*iteTrackInfo).Mom.Z();
   g4Proddata.P[partNum][3]= (*iteTrackInfo).Mom.E();

   partNum++; 
 }
    
 if (g4Proddata.NPart>0)WriteNtuple();
}

void HadAnalysis::WriteNtuple(){
    
    ProdTree->Fill();
    
}

void HadAnalysis::GetDetConstInfo(std::vector<Double_t> DCinfo,G4Material* mat){
  tickness = DCinfo[0]; //z
  height   = DCinfo[1]; //y
  width    = DCinfo[2]; //x
  rho 	   = DCinfo[3]; //density
  mate = mat;
  material = mate->GetName();
}

void HadAnalysis::GetPrimGenInfo(Double_t mean,G4ParticleDefinition* Part){
  enerPrimGen = mean; 
  //G4cout << enerPrimGen << G4endl;
  particle    = Part;
  PartName    = particle->GetParticleName();
}

void HadAnalysis::GetRunActInfo(Int_t nevt){
  numberEvts = nevt;
}

void HadAnalysis::WriteHeader(){

  //XS:
  G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();
 // int pos = material.find("_");
  //G4cout << "Hello Material: "<< material << G4endl;
 // std::string elemName = material.substr(pos+1);
  std::string elemName = "Ar";
  const G4Element* elm  = G4NistManager::Instance()->FindOrBuildElement(elemName);
  inelasticXS = 1.e25*(store->GetInelasticCrossSectionPerAtom(particle,enerPrimGen,elm)); 
  elasticXS   = 1.e25*(store->GetElasticCrossSectionPerAtom(particle,enerPrimGen,elm));
  
  HeaderTree->Fill();
  HeaderTree->Write();
    
}
