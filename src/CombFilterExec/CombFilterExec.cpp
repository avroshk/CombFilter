#include <iostream>
#include <math.h>
#include <cmath>
#include "CombFilterProject.h"
#include "FilterAudio.h"

# define TEST_MODE

using namespace std;

//function declarations
float *sinOsc(float, float, float, unsigned);
int testZeroInput();
int testFIRFeedforward();
int testIIRDestructiveFeedforward();
int testIIRConstructiveFeedforward();
int testDifferntBlockSizes();
int testZeroDelay(int, float, float);
int testFIRImpulseResponse();
int testIIRImpulseResponse();

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
    
#ifdef TEST_MODE
    
    cout<<"\n====== Test Results ======\n";
    testFIRFeedforward();
    testIIRDestructiveFeedforward();
    testIIRConstructiveFeedforward();
    testDifferntBlockSizes();
    testZeroInput();
    
    //--- Some extra tests --- //
    if (testZeroDelay(1024, 1.0, 0)==0) {
        cout<<"\nZero Delay Test: Success!\n";
    }
    testFIRImpulseResponse();
    testIIRImpulseResponse();
    
#endif
  
    return 0;
}

int testZeroInput() {
    FilterAudio *pFilter;
    float fFIRCoeff = 1.0;
    float fIIRCoeff = 0.5;
    int iDelayInSamples = 10;
    int iNumChannels = 1;
    int iBlockSize = 1024;
    int iNumBlocks = 50; //Length of sample is 50 blocks
    
    //Allocate memory
    float **ppfAudioData = new float *[iNumChannels];
    for (int n=0; n<iNumChannels; n++) {
        ppfAudioData[n] = new float[iBlockSize];
        for (int m=0; m<iBlockSize; m++) {
            ppfAudioData[n][m] = 0; //Initialize all samples to zero
        }
    }
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    while (iNumBlocks > 0) {
        
        //Get filtered data
        ppfAudioData = pFilter->combFilterBlock(ppfAudioData, iBlockSize, iNumChannels);
        
        for (int n=0; n<iNumChannels; n++) {
            for (int m=0; m<iBlockSize; m++) {
                
                //Check if all the values are 0
                if (abs(ppfAudioData[n][m]) > 0.001) {
                    cout<<"\nZero Input Test: failed!\n";
                    return -1;
                }
            }
        }
        iNumBlocks--;
    }

    cout<<"\nZero Input Test: Success!\n";
    
    //Free memory
    delete pFilter;
    pFilter = 0;
    for (int n=0; n<iNumChannels; n++) {
        delete [] ppfAudioData[n];
    }
    delete ppfAudioData;
    
    return 0;
}

int testFIRFeedforward() {
    
    FilterAudio *pFilter;
    float fFIRCoeff = 1.0;
    
    float fIIRCoeff = 0.0;
    int iNumChannels = 1;
    int iBlockSize = 400;
    int iNumBlocks;
    
    float fFreq = 441.0; //in Hz - Test sine wave
    float fAmp = 1.0;
    float fTimeInSecs = 2.0; // 2 seconds long
    unsigned uSampleRate = 44100;
    int iPeriodInSamples = uSampleRate/fFreq; // 100 samples in this case
    
    int iDelayInSamples = iPeriodInSamples/2; //Delay = 50 samples to create destructive interference
    
    //Allocate memory
    float **buffer = new float *[iNumChannels];
    for (int i=0;i<iNumChannels ;i++) {
        buffer[i] = new float[iBlockSize];
    }
    
    //Create sine wave
    float *sinWave = sinOsc(fFreq, fAmp, fTimeInSecs, uSampleRate);
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    int iLengthInSamples = ceil(fTimeInSecs * uSampleRate);
 
    //To block the sine wave audio
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

            //Ignore first part of the first block
            start = 0;
            if (n == 0){
                start = iDelayInSamples+1;
            }
            
            for (int m=start; m<iBlockSize; m++) {
                //Check for Complete destructive interference - so all values are zero
                if (std::abs(buffer[c][m]) > 0.001) {
                    cout<<"\nFIR Feedforward Test: failed!\n";
                    return -1;
                }
            }
            
        }
        
        cout<<"\nFIR Feedforward Test: Success!\n";
    }
    
    //Free memory
    delete pFilter;
    pFilter = 0;
    for (int i=0;i<iNumChannels ;i++) {
        delete [] buffer[i];
    }
    delete buffer;
    
    delete [] sinWave;
    
    return 0;
}

int testIIRDestructiveFeedforward() {
    
    FilterAudio *pFilter;
    float fFIRCoeff = 0.0;
    float fIIRCoeff = 0.1;
    int iNumChannels = 1;
    int iBlockSize = 1024;
    int iNumBlocks;
    
    float fFreq = 441.0; //Hz - Test sine wave
    float fAmp = 1.0;
    float fTimeInSecs = 2.0;
    unsigned uSampleRate = 44100;
    
    int iPeriodInSamples = uSampleRate/fFreq; //100 samples
    
    int iDelayInSamples = iPeriodInSamples/2; //50 samples - Causing Destructive interference by 0.1 margin
    
    float **buffer = new float *[iNumChannels];
    for (int i=0;i<iNumChannels ;i++) {
        buffer[i] = new float[iBlockSize];
    }
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    float *sinWave = sinOsc(fFreq, fAmp, fTimeInSecs, uSampleRate);
    
    int iLengthInSamples = ceil(fTimeInSecs * uSampleRate);
    
    iNumBlocks = ceil((float)iLengthInSamples/(float)iBlockSize);
    
    for (int c=0; c<iNumChannels; c++) {
        
        int start = 0;
        float fMaxAmp = 0;
        
        for (int n=0; n<iNumBlocks-1; n++) {
            
            if (n*iBlockSize >  iLengthInSamples) {
                memcpy(buffer[c], &sinWave[n*iBlockSize], (iLengthInSamples-n*iBlockSize)*sizeof(float));
            }
            else {
                memcpy(buffer[c], &sinWave[n*iBlockSize], iBlockSize*sizeof(float));
            }
            
            buffer = pFilter->combFilterBlock(buffer, iBlockSize, iNumChannels);
            
            if (n == 0){
                start = iPeriodInSamples+1;
            }
            
            for (int m=0+start; m<iBlockSize; m++) {
                
                if (buffer[c][m] >= fMaxAmp) {
                    fMaxAmp = buffer[c][m];
                }
            }
            
            //Max amplitude of output should always be less than input
            if (fMaxAmp >= fAmp) {
                cout<<"\nIIR Desructive Feedforward Test: failed!\n";
                return -1;
            }
            
        }
    
        cout<<"\nIIR Destructive Feedforward Test: Success!\n";
    }
    
    //Free memory
    delete pFilter;
    pFilter = 0;
    for (int i=0;i<iNumChannels ;i++) {
        delete [] buffer[i];
    }
    delete buffer;
    
    delete [] sinWave;
    
    return 0;
}



int testIIRConstructiveFeedforward() {
    
    FilterAudio *pFilter;
    float fFIRCoeff = 0.0;
    float fIIRCoeff = 0.1;
    int iNumChannels = 1;
    int iBlockSize = 1024;
    int iNumBlocks;
    
    float fFreq = 441.0; //Hz
    float fAmp = 1.0;
    float fTimeInSecs = 2.0;
    unsigned uSampleRate = 44100;
    
    int iPeriodInSamples = uSampleRate/fFreq; //100 samples
    
    int iDelayInSamples = iPeriodInSamples; //100 samples - Constructive interference
    
    float **buffer = new float *[iNumChannels];
    for (int i=0;i<iNumChannels ;i++) {
        buffer[i] = new float[iBlockSize];
    }
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    float *sinWave = sinOsc(fFreq, fAmp, fTimeInSecs, uSampleRate);
    
    int iLengthInSamples = ceil(fTimeInSecs * uSampleRate);
    
    iNumBlocks = ceil((float)iLengthInSamples/(float)iBlockSize);
    
    for (int c=0; c<iNumChannels; c++) {
        
        int start = 0;
        float fMaxAmp = 0;
        
        for (int n=0; n<iNumBlocks-1; n++) {
            
            if (n*iBlockSize >  iLengthInSamples) {
                memcpy(buffer[c], &sinWave[n*iBlockSize], (iLengthInSamples-n*iBlockSize)*sizeof(float));
            }
            else {
                memcpy(buffer[c], &sinWave[n*iBlockSize], iBlockSize*sizeof(float));
            }
            
            buffer = pFilter->combFilterBlock(buffer, iBlockSize, iNumChannels);
            
            if (n == 0){
                start = iPeriodInSamples+1;
            }
            
            for (int m=0+start; m<iBlockSize; m++) {
                
                if (buffer[c][m] >= fMaxAmp) {
                    fMaxAmp = buffer[c][m];
                }
            }
            
            //Max amplitude of output should always be greater than input
            if (fMaxAmp <= fAmp) {
                cout<<"\nIIR Constructive Feedforward Test: failed!\n";
                return -1;
            }
        }
        cout<<"\nIIR Constructive Feedforward Test: Success!\n";
    }
    
    //Free memory
    delete pFilter;
    pFilter = 0;
    for (int i=0;i<iNumChannels ;i++) {
        delete [] buffer[i];
    }
    delete buffer;
    
    delete [] sinWave;
    
    return 0;
}


int testDifferntBlockSizes() {
    
    
    if(testZeroDelay(1024, 1.0, 0)!=0) {
        cout<<"\nVarying block size test @ 1024 : failed!\n";
        return -1;
    }
    if(testZeroDelay(256, 0.5, 0.5)!=0) {
        cout<<"\nVarying block size test @ 1024 : failed!\n";
        return -1;
    }
    if(testZeroDelay(2048, 0.5, 1.0)!=0) {
        cout<<"\nVarying block size test @ 1024 : failed!\n";
        return -1;
    }
    if(testZeroDelay(4096, 0, 1.0)!=0) {
        cout<<"\nVarying block size test @ 1024 : failed!\n";
        return -1;
    }

    cout<<"\nVarying block size test : Success!\n";
    return 0;
}


int testZeroDelay(int iBlockSize, float fFIRCoeff, float fIIRCoeff) {
    
    FilterAudio *pFilter;
    int iNumChannels = 1;
    int iNumBlocks;
    
    float fFreq = 441.0; //Hz -  Test sine wave
    float fAmp = 1.0;
    float fTimeInSecs = 2.0;
    unsigned uSampleRate = 44100;
    
    int iDelayInSamples = 0; //No delay
    
    float **buffer = new float *[iNumChannels];
    for (int i=0;i<iNumChannels ;i++) {
        buffer[i] = new float[iBlockSize];
    }
    
    float **origBuffer = new float *[iNumChannels];
    for (int i=0;i<iNumChannels ;i++) {
        origBuffer[i] = new float[iBlockSize];
    }
    
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    float *sinWave = sinOsc(fFreq, fAmp, fTimeInSecs, uSampleRate);
    
    int iLengthInSamples = ceil(fTimeInSecs * uSampleRate);
    
    iNumBlocks = ceil((float)iLengthInSamples/(float)iBlockSize);
    
    for (int c=0; c<iNumChannels; c++) {
        
        for (int n=0; n<iNumBlocks-1; n++) {
            
            if (n*iBlockSize >  iLengthInSamples) {
                memcpy(buffer[c], &sinWave[n*iBlockSize], (iLengthInSamples-n*iBlockSize)*sizeof(float));
                memcpy(origBuffer[c], &sinWave[n*iBlockSize], (iLengthInSamples-n*iBlockSize)*sizeof(float));
            }
            else {
                memcpy(buffer[c], &sinWave[n*iBlockSize], iBlockSize*sizeof(float));
                memcpy(origBuffer[c], &sinWave[n*iBlockSize], iBlockSize*sizeof(float));
            }
            
            buffer = pFilter->combFilterBlock(buffer, iBlockSize, iNumChannels);
            
            for (int m=0; m<iBlockSize; m++) {
                //Output should be same as input
                if (buffer[c][m] != origBuffer[c][m]) {
                    cout<<"\nZero Delay Test: failure!\n";
                    return -1;
                }
            }
            
        }
    }
    
    //Free memory
    delete pFilter;
    pFilter = 0;
    for (int i=0;i<iNumChannels ;i++) {
        delete [] buffer[i];
    }
    delete buffer;
    
    for (int i=0;i<iNumChannels ;i++) {
        delete [] origBuffer[i];
    }
    delete origBuffer;
    
    delete [] sinWave;
    
    return 0;
}

int testFIRImpulseResponse() {
    FilterAudio *pFilter;
    float fFIRCoeff = 1.0;
    float fIIRCoeff = 0.0;
    int iDelayInSamples = 5;
    int iNumChannels = 1;
    int iBlockSize = 1024;
    int iNumBlocks = 1; //Do this test only for 1 block
    float **ppfAudioData = new float *[iNumChannels];
    
    for (int n=0; n<iNumChannels; n++) {
        ppfAudioData[n] = new float[iBlockSize];
        for (int m=0; m<iBlockSize; m++) {
            ppfAudioData[n][m] = 0;
        }
        ppfAudioData[n][0]=1.0;
    }

    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    for (int n=0; n<iNumBlocks; n++)  {
        
        ppfAudioData = pFilter->combFilterBlock(ppfAudioData, iBlockSize, iNumChannels);
  
        for (int n=0; n<iNumChannels; n++) {
            
            for (int m=0; m<iBlockSize; m++) {
               
                //Check for peak at the m=delaylength
                if (m==0 || m==iDelayInSamples) {
                    if (ppfAudioData[n][m] != 1.0F) {
                        cout<<"\nFIR Impulse Response Test: failed!\n";
                        return -1;
                    }
                }
                else {
                    if (ppfAudioData[n][m] > 0.001) {
                        cout<<"\nFIR Impulse Response Test: failed!\n";
                        return -1;
                    }
                
                }
            }
        }
    }
    
    cout<<"\nFIR Impulse Response Test: Success!\n";
    
    
    //Free memory
    delete pFilter;
    pFilter = 0;
    for (int i=0;i<iNumChannels ;i++) {
        delete [] ppfAudioData[i];
    }
    delete ppfAudioData;

    return 0;
}


int testIIRImpulseResponse() {
    FilterAudio *pFilter;
    float fFIRCoeff = 0.0;
    float fIIRCoeff = 1.0;
    int iDelayInSamples = 5;
    int iNumChannels = 1;
    int iBlockSize = 1024;
    int iNumBlocks = 1; //Do this test only for 1 block
    float **ppfAudioData = new float *[iNumChannels];
    
    for (int n=0; n<iNumChannels; n++) {
        ppfAudioData[n] = new float[iBlockSize];
        for (int m=0; m<iBlockSize; m++) {
            ppfAudioData[n][m] = 0;
        }
        ppfAudioData[n][0]=1.0;
    }
    
    pFilter = new FilterAudio(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    
    
    
    for (int n=0; n<iNumBlocks; n++)  {
        
        ppfAudioData = pFilter->combFilterBlock(ppfAudioData, iBlockSize, iNumChannels);
        
        for (int n=0; n<iNumChannels; n++) {
            
            for (int m=0; m<iBlockSize; m++) {
                
                
                if (m%5==0) {
                    //Check for peaks at multiples of delay length
                    if (ppfAudioData[n][m] != 1.0F) {
                        cout<<"\nIIR Impulse Response Test: failed!\n";
                        return -1;
                    }
                }
                else {
                    if (ppfAudioData[n][m] > 0.001) {
                        cout<<"\nIIR Impulse Response Test: failed!\n";
                        return -1;
                    }
                    
                }
            }
        }
    }
    
    cout<<"\nIIR Impulse Response Test: Success!\n";
    
    
    //Free memory
    delete pFilter;
    pFilter = 0;
    for (int i=0;i<iNumChannels ;i++) {
        delete [] ppfAudioData[i];
    }
    delete ppfAudioData;
    
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

