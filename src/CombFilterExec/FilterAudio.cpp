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
FilterAudio::FilterAudio(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples, int iNumChannels) {
    // Initialize coefficients
    this->fFIRCoeff = fFIRCoeff;
    this->fIIRCoeff = fIIRCoeff;
    this->iDelayInSamples = iDelayInSamples;
    this->iNumChannels = iNumChannels;
    fFIRDelay = new float *[iNumChannels];
    fIIRDelay = new float *[iNumChannels];
    
    for(int n = 0; n < iNumChannels; n++){
        fFIRDelay[n] = new float[iDelayInSamples];
        fIIRDelay[n] = new float[iDelayInSamples];
    }
    
    clearDelayLines();
}

FilterAudio::~FilterAudio() {
    // Free all memory
    for (int k=0; k<iNumChannels; k++) {
        delete [] fFIRDelay[k];
        delete [] fIIRDelay[k];
    }

}

void FilterAudio::clearDelayLines(){
    // Initialize delay lines
    for(int n = 0; n < iNumChannels; n++){
        for(int k = 0; k < iDelayInSamples; k++){
            fFIRDelay[n][k] = 0;
            fIIRDelay[n][k] = 0;
        }
    }
   
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
        
        // Perform filtering
        for(int j = 0; j < iBlockSize; j++){
            fOutput[i][j] = fInput[i][j] + fFIRCoeff*fFIRDelay[i][iDelayInSamples-1] + fIIRCoeff*fIIRDelay[i][iDelayInSamples-1];
            
            for(int k = iDelayInSamples-1; k>0; k--){
                fFIRDelay[i][k] = fFIRDelay[i][k-1];
                fIIRDelay[i][k] = fIIRDelay[i][k-1];
            }
            fFIRDelay[i][0] = fInput[i][j];
            fIIRDelay[i][0] = fOutput[i][j];
        }
        
    }
    
    return fOutput;
}

int FilterAudio::getDelayInSamples() {
    return iDelayInSamples;
}

float FilterAudio::getFIRCoeff() {
    return fFIRCoeff;
}

float FilterAudio::getIIRCoeff() {
    return fIIRCoeff;
}