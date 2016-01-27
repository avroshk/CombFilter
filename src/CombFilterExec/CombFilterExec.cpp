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
    int iBlockSize = 2048; //default blockSize
    
    
    //Create Comb Filter
    CombFilterProject::create(pCombFilterProject, iBlockSize);
    
    //Print Build version
    cout<<"CombFilter V"<<pCombFilterProject->getVersion(CombFilterProject::kMajor)
    <<"."<<pCombFilterProject->getVersion(CombFilterProject::kMinor)
    <<"."<<pCombFilterProject->getVersion(CombFilterProject::kPatch)<<endl;
    
    cout<<"Build date: "<<pCombFilterProject->getBuildDate()<<endl<<endl;

    
    // Parse command line arguments
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
        printf("Please provide the following\n 1.file path\n 2.audio file name\n 3.FIR gain\n 4.IIR gain\n 5.delay time in miliseconds\n and 6.block size (optional)\n");
        return 0;
    }
    
    // Get audio file name
    sInputFilePath = argv[1];
    sInputFileName = argv[2];
    fFIRCoeff = atof(argv[3]);
    fIIRCoeff = atof(argv[4]);
    iDelayInMSec = atoi(argv[5]);
    if (argv[6] != NULL) {
        iBlockSize = atoi(argv[3]);
    }
    sOutputFilePath = sInputFilePath;
    sOutputFileName = "output.wav";
   
    pCombFilterProject->init(sInputFilePath, sInputFileName, sOutputFilePath, sOutputFileName, fFIRCoeff, fIIRCoeff, iDelayInMSec, true);
    
    cout<<"Delay in samples: "<<pCombFilterProject->getDelayinSamples()<<endl<<endl;

    pCombFilterProject->processAudio();
    
    pCombFilterProject->destroy(pCombFilterProject);
    
    cout<<"Success!\n";
  
    return 0;
}

void testZeroInputSignal(string sInputFilePath, string sInputFileName) {
    
    CombFilterProject *pCombFilterProject;
    
    float fFIRCoeff = 0.9;
    float fIIRCoeff = 0.9;
    int iSampleRate = 44100;
    int iNumChannels = 2;
    
    float **zeroInput = new float *[iNumChannels];
    for (int i=0; i<iNumChannels;i++) {
        zeroInput[i] = new float[5*iSampleRate]; //5 seconds
        for (int j=0; j<5*iSampleRate; j++) {
            zeroInput[i][j] = 0;
        }
    }
    
    CombFilterProject::create(pCombFilterProject, 2048);
    pCombFilterProject->init(sInputFilePath, sInputFileName, sInputFilePath, "zero_input_result_fir.wav", fFIRCoeff, 0, 15);
//    pCombFilterProject->readAudio();
    pCombFilterProject->processAudio();
//    pCombFilterProject->writeAudio();
    
    pCombFilterProject->reset();
    pCombFilterProject->init(sInputFilePath, sInputFileName, sInputFilePath, "zero_input_result_iir.wav", 0, fIIRCoeff, 15);
//    pCombFilterProject->readAudio();
    pCombFilterProject->processAudio();
//    pCombFilterProject->writeAudio();
    
    
    pCombFilterProject->destroy(pCombFilterProject);
    
}

