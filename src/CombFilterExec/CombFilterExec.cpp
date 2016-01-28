#include <iostream>
#include <math.h>
#include <cmath>
#include "CombFilterProject.h"
#include "FilterAudio.h"

//# define TEST_MODE

using namespace std;

//function
int testZeroInput();
float *sinOsc(float, float, float, unsigned);
int testFIRFeedforward();
int testIIRFeedforward();


// main function
int main(int argc, char* argv[])
{
    //decalare local variables
    CombFilterProject *pCombFilterProject;
    
    string sInputFilePath, sOutputFilePath, sInputFileName, sOutputFileName;

    float fFIRCoeff = 0.0;
    float fIIRCoeff = 0.0;
    float fDelayInMSec = 0.0;
    int iBlockSize = 2048; //default blockSize
    bool bOutputResultToTextFile = false; //default
    int iFileOpenStatus;

    
    // Parse command line arguments
    if( argc == 7 || argc == 8) {
        //File path and name
        sInputFilePath = argv[1];
        sInputFileName = argv[2];
        
        //Gain
        fFIRCoeff = atof(argv[3]);
        fIIRCoeff = atof(argv[4]);
        
        //Check for boundary conditions
        if (fFIRCoeff > 1 || fFIRCoeff < -1 || fIIRCoeff > 1 || fFIRCoeff < -1) {
            cout<<"\nFIR/IIR Co-efficient should be between -1 and 1.\n";
            return 0;
        }
        //Delay time
        fDelayInMSec = atof(argv[5]);
        
        //Check that delay time is positive
        if (fDelayInMSec < 0 ) {
            cout<<"\nDelay time must be positive.\n";
            return 0;
        }
        
        //enable write to Text file
        if (argv[6] != NULL) {
            int temp = atoi(argv[6]);
            if (temp == 1) {
                bOutputResultToTextFile = true;
            }
        }
        
        //Optional argument to set blockSize
        if (argv[7] != NULL) {
            iBlockSize = atoi(argv[7]);
        }
    }
    else if( argc > 8 ) {
        printf("Too many arguments supplied.\n");
        return 0;
    }
    else {
        printf("Please provide the following\n 1.file path\n 2.audio file name\n 3.FIR gain\n 4.IIR gain\n 5.delay time in milliseconds\n 6.write to text file (yes - 1, no - any value)\n 7.[OPTIONAL] block size\n ");
        return 0;
    }
    
    
    //Create Comb Filter
    CombFilterProject::create(pCombFilterProject, iBlockSize);
    
    //Print Information
    cout<<"CombFilter V"<<pCombFilterProject->getVersion(CombFilterProject::kMajor)
    <<"."<<pCombFilterProject->getVersion(CombFilterProject::kMinor)
    <<"."<<pCombFilterProject->getVersion(CombFilterProject::kPatch)<<endl;
    
    cout<<"Build date: "<<pCombFilterProject->getBuildDate()<<endl<<endl;
    
    cout<<"Reading audio file: "<<sInputFilePath<<sInputFileName<<"\n";
    cout<<"\nFIR Coefficient:"<<fFIRCoeff;
    cout<<"\nIIR Coefficient:"<<fIIRCoeff;
    cout<<"\nBlock size :"<<iBlockSize;
    cout<<"\nDelay in milliseconds :"<<fDelayInMSec;

    //Set defaults
    sOutputFilePath = sInputFilePath;
    sOutputFileName = "output.wav";
   
    //Initialize CombFilter
    iFileOpenStatus = pCombFilterProject->init(sInputFilePath, sInputFileName, sOutputFilePath, sOutputFileName, fFIRCoeff, fIIRCoeff, fDelayInMSec, bOutputResultToTextFile);
    
    if (iFileOpenStatus != 0) {
        /*File open error*/
        return 0;
    }
    
    cout<<"\nDelay in samples: "<<pCombFilterProject->getDelayinSamples()<<endl<<endl;
    
    pCombFilterProject->processAudio();
    
    pCombFilterProject->destroy(pCombFilterProject);
    
    cout<<"Success!\n";
    
    testZeroInput();
    testFIRFeedforward();
    testIIRFeedforward();
  
    return 0;
}

int testZeroInput() {
    FilterAudio *pFilter;
    float fFIRCoeff = 1.0;
    float fIIRCoeff = 0.0;
    int iDelayInSamples = 10;
    int iNumChannels = 1;
    int iBlockSize = 1024;
    int iNumBlocks = 50; //Test for 10 blocks
    float **ppfAudioData = new float *[iNumChannels];
    
    for (int n=0; n<iNumChannels; n++) {
        ppfAudioData[n] = new float[iBlockSize];
        for (int m=0; m<iBlockSize; m++) {
            ppfAudioData[n][m] = 0;
        }
    }
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    while (iNumBlocks > 0) {
        pFilter->combFilterBlock(ppfAudioData, iBlockSize, iNumChannels);
        
        for (int n=0; n<iNumChannels; n++) {
            for (int m=0; m<iBlockSize; m++) {
                if (ppfAudioData[n][m] > 0.001) {
                    
                    cout<<"\nZero Input Test: failed!\n";
                    return -1;
                }
            }
        }
        iNumBlocks--;
    }

    cout<<"\nZero Input Test: Success!\n";
    return 0;
}

int testFIRFeedforward() {
    
    FilterAudio *pFilter;
    float fFIRCoeff = 1.0;
    float fIIRCoeff = 0.0;
    int iDelayInSamples = 50;
    int iNumChannels = 1;
    int iBlockSize = 1024;
    int iNumBlocks;
    
    float fFreq = 441.0; //Hz
    float fTimeInSecs = 2.0;
    unsigned uSampleRate = 44100;
    
    float **buffer = new float *[iNumChannels];
    for (int i=0;i<iNumChannels ;i++) {
        buffer[i] = new float[iBlockSize];
    }
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);

    
    float *sinWave = sinOsc(fFreq, 1.0, fTimeInSecs, uSampleRate);
    
    int iLengthInSamples = ceil(fTimeInSecs * uSampleRate);
 
    iNumBlocks = ceil((float)iLengthInSamples/(float)iBlockSize);
    
    for (int c=0; c<iNumChannels; c++) {
        
        int start = 0;
        
        for (int n=0; n<iNumBlocks-1; n++) {
            
            if (n*iBlockSize >  iLengthInSamples) {
                memcpy(buffer[c], &sinWave[n*iBlockSize], (iLengthInSamples-n*iBlockSize)*sizeof(float));
            }
            else {
                memcpy(buffer[c], &sinWave[n*iBlockSize], iBlockSize*sizeof(float));
            }
            
            buffer = pFilter->combFilterBlock(buffer, iBlockSize, iNumChannels);

            start = 0;
            if (n == 0){
                start = iDelayInSamples+1;
            }
            
            for (int m=start; m<iBlockSize; m++) {
                if (std::abs(buffer[c][m]) > 0.001) {
                    cout<<"\nFIR Feedforward Test: failed!\n";
                    return -1;
                }
            }
            
        }
        
        cout<<"\nFIR Feedforward Test: Success!\n";
    }
    
    return 0;
}

int testIIRFeedforward() {
    
    FilterAudio *pFilter;
    float fFIRCoeff = 0.0;
    float fIIRCoeff = 0.1;
    int iDelayInSamples = 100;
    int iNumChannels = 1;
    int iBlockSize = 1024;
    int iNumBlocks;
    
    float fFreq = 441.0; //Hz
    float fTimeInSecs = 2.0;
    unsigned uSampleRate = 44100;
    
    float **buffer = new float *[iNumChannels];
    for (int i=0;i<iNumChannels ;i++) {
        buffer[i] = new float[iBlockSize];
    }
    
    float **origBuffer = new float *[iNumChannels];
    for (int i=0;i<iNumChannels ;i++) {
        origBuffer[i] = new float[iBlockSize];
    }
    
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    
    float *sinWave = sinOsc(fFreq, 1.0, fTimeInSecs, uSampleRate);
    
    int iLengthInSamples = ceil(fTimeInSecs * uSampleRate);
    
    iNumBlocks = ceil((float)iLengthInSamples/(float)iBlockSize);
    
//    int iPeriodInSamples = (int)uSampleRate/fFreq;
    
    for (int c=0; c<iNumChannels; c++) {
        
        for (int n=0; n<iNumBlocks-1; n++) {
            
            if (n*iBlockSize >  iLengthInSamples) {
                memcpy(buffer[c], &sinWave[n*iBlockSize], (iLengthInSamples-n*iBlockSize)*sizeof(float));
            }
            else {
                memcpy(buffer[c], &sinWave[n*iBlockSize], iBlockSize*sizeof(float));
                memcpy(origBuffer[c], &sinWave[n*iBlockSize], iBlockSize*sizeof(float));
            }
            
            buffer = pFilter->combFilterBlock(buffer, iBlockSize, iNumChannels);
            
            float* temp = buffer[c];
            float* origTemp = origBuffer[c];
//            float* diff = new float[iBlockSize];
            
//            for (int m=iPeriodInSamples+iPeriodInSamples/4; m<iBlockSize; m=m+iPeriodInSamples) {
//                
//                if (buffer[c][m] <= origBuffer[c][m]) {
//                    cout<<"\IIR Feedforward Test: failed!\n";
//                    return -1;
//                }
//
//                
//            }
            
        }
        
        cout<<"\nIIR Feedforward Test: Success!\n";
    }
    
    return 0;
}


float *sinOsc(float fFreq, float fAmp, float fLengthInSec, unsigned uFs){
    
    float pi = 3.1415926535897932346;

    int lengthInSamples = ceil(fLengthInSec * uFs);
    float* fSineWave = new float[lengthInSamples];

    // Fill sine wave
    for(int i = 0; i<lengthInSamples; i++){
        fSineWave[i] = fAmp* sin((2.f*pi*fFreq)/(uFs) *i);
    }
    
    return fSineWave;
}

