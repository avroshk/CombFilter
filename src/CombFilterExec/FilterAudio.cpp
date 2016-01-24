//
//  FilterAudio.cpp
//  CombFilter
//
//  Created by Christopher Latina on 1/23/16.
//
//

#include "FilterAudio.h"

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
    
    
    // Filter each channel
    for(int i = 0; i<numChannels; i++){
        // Initial sample
        for(int j = 0; i < iDelayInSamples; j++){
            output[i][j] = input[i][j];
        }
        
        // Perform filtering
        for(int j = 0; i < blockSize; j++){
            output[i][j] = input[i][j] + fFIRCoeff*input[i][j-iDelayInSamples] + fIIRCoeff*input[i][j-iDelayInSamples];
        }
    }
    
    return output;
}