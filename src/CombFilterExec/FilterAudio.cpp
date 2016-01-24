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

float ** FilterAudio::combFilterBlock(float **input, int blockSize, int numChannels){
    
    // Allocate memory for output
    float **output = new float *[numChannels];
    
    for (int i = 0; i < numChannels ; i ++)
    {
        output[i] = new float[blockSize];
    }
    
//    float* test1 = input[0];

    
    float *fFIRDelay = new float [iDelayInSamples];
    float *fIIRDelay = new float [iDelayInSamples];
    
    // Filter each channel
    for(int i = 0; i<numChannels; i++){
        // Initialize delay lines
        for(int k = 0; k < iDelayInSamples; k++){
            fFIRDelay[k] = 0.0;
            fIIRDelay[k] = 0.0;
        }
        
        // Perform filtering
        for(int j = 0; j < blockSize; j++){
            output[i][j] = input[i][j] + fFIRCoeff*fFIRDelay[iDelayInSamples-1] + fIIRCoeff*fIIRDelay[iDelayInSamples-1];
            
            for(int k = iDelayInSamples-1; k>0; k--){
                fFIRDelay[k] = fFIRDelay[k-1];
                fIIRDelay[k] = fIIRDelay[k-1];
            }
            fFIRDelay[0] = input[i][j];
            fIIRDelay[0] = output[i][j];
        }
    }
    
    return output;
}

int FilterAudio::getDelayInSamples() {
    return iDelayInSamples;
}