//
//  FilterAudio.cpp
//  CombFilter
//
//  Created by Christopher Latina on 1/23/16.
//
//

#include "FilterAudio.h"
#include <iostream>

// Constructor
FilterAudio::FilterAudio(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples) {
    // Initialize coefficients
    this->fFIRCoeff = fFIRCoeff;
    this->fIIRCoeff = fIIRCoeff;
    this->iDelayInSamples = iDelayInSamples;
}

FilterAudio::~FilterAudio() {
    // Free all memory
}

float ** FilterAudio::combFilterBlock(float **fInput, int iBlockSize, int iNumChannels){
    
    // Allocate memory for output
    float **fOutput = new float *[iNumChannels];
    
    for (int i = 0; i < iNumChannels ; i ++)
    {
        fOutput[i] = new float[iBlockSize];
    }
    
    // Filter each channel
    for(int i = 0; i<iNumChannels; i++){
        float *fFIRDelay = new float [iDelayInSamples];
        float *fIIRDelay = new float [iDelayInSamples];
        
        // Initialize delay lines
        for(int k = 0; k < iDelayInSamples; k++){
            fFIRDelay[k] = 0;
            fIIRDelay[k] = 0;
        }
        
        // Perform filtering
        for(int j = 0; j < iBlockSize; j++){
            fOutput[i][j] = fInput[i][j] + fFIRCoeff*fFIRDelay[iDelayInSamples-1] + fIIRCoeff*fIIRDelay[iDelayInSamples-1];
            
            for(int k = iDelayInSamples-1; k>0; k--){
                fIIRDelay[k] = fIIRDelay[k-1];
                fFIRDelay[k] = fFIRDelay[k-1];
            }
            fFIRDelay[0] = fInput[i][j];
            fIIRDelay[0] = fOutput[i][j];
        }
    }
    
    return fOutput;
}

int FilterAudio::getDelayInSamples() {
    return iDelayInSamples;
}