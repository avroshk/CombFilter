#include <iostream>

#include "CombFilterProject.h"

using namespace std;

// main function
int main(int argc, char* argv[])
{
    //decalare local variables
    
    CombFilterProject *pCombFilterProject;
    
    string sInputFilePath, sOutputFilePath, sInputFileName, sOutputFileName;

    float fFIRCoeff = 0.0;
    float fIIRCoeff = 0.0;
    int iDelayInMSec = 0;
    int iBlockSize = 2048;
    int iOverlapRatio = 2;
    
    //Create Comb Filter
    CombFilterProject::create(pCombFilterProject, iBlockSize, iOverlapRatio);

    cout<<"CombFilter V"<<pCombFilterProject->getVersion(CombFilterProject::kMajor)
    <<"."<<pCombFilterProject->getVersion(CombFilterProject::kMinor)
    <<"."<<pCombFilterProject->getVersion(CombFilterProject::kPatch)<<endl;
    
    cout<<"Build date: "<<pCombFilterProject->getBuildDate()<<endl<<endl;
    
    
    // Parse command line arguments
    
    // Check argc
    if( argc == 6 ) {
        printf("Reading audio file: %s%s\n", argv[1],argv[2]);
        printf("\nFIR Coefficient %f",atof(argv[3]));
        printf("\nIIR Coefficient %f",atof(argv[4]));
        printf("\nDelay in milliseconds %i",atoi(argv[5]));
        printf("\n");
        
        // Make sure gain is between -1 and 1, a number
        
        // Check that delay time is positive and nonzero.
        
    }
    else if( argc > 6 ) {
        printf("Too many arguments supplied.\n");
        return 0;
    }
    else {
        printf("Please provide the file path, audio file name, FIR gain, IIR gain, and delay time in miliseconds.\n");
        return 0;
    }
    
    // Get audio file name
    sInputFilePath = argv[1];
    sInputFileName = argv[2];
    fFIRCoeff = atof(argv[3]);
    fIIRCoeff = atof(argv[4]);
    iDelayInMSec = atoi(argv[5]);
    sOutputFilePath = sInputFilePath;
    sOutputFileName = "output.wav";
    
    pCombFilterProject->init(sInputFilePath, sInputFileName, sOutputFilePath, sOutputFileName, fFIRCoeff, fIIRCoeff, iDelayInMSec);
    pCombFilterProject->readAudio();
    pCombFilterProject->processAudio();
    pCombFilterProject->writeAudio();
    
    //Write Audio to CSV file for comparison with matlab
    pCombFilterProject->writeAudioToText("output.csv");
    
    cout<<"\nDelay in samples: "<<pCombFilterProject->getDelayinSamples()<<endl;
    cout<<"\nBuild date: "<<pCombFilterProject->getBuildDate()<<endl;
    
    pCombFilterProject->destroy(pCombFilterProject);
  
    return 0;
}


