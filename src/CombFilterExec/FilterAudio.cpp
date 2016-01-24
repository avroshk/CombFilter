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
    
    //Padding?
    
    //Padded output
    float **paddedOutput = new float *[numChannels];
    
    for (int i = 0; i < numChannels ; i ++)
    {
        paddedOutput[i] = new float[blockSize+(iDelayInSamples*2)];
    }
    //Padded input
    float **paddedInput = new float *[numChannels];
    
    for (int i = 0; i < numChannels ; i ++)
    {
        paddedInput[i] = new float[blockSize+(iDelayInSamples*2)];
        memcpy(&paddedInput[i][iDelayInSamples],input[i],blockSize * sizeof(float));
    }
    
    
    // Filter each channel
    for(int i = 0; i<numChannels; i++){
        // Initial sample
        for(int j = 0; j < iDelayInSamples; j++){
            output[i][j] = input[i][j];
        }
        
        // Perform filtering
        for(int j = iDelayInSamples; j < blockSize+iDelayInSamples; j++){
            paddedOutput[i][j] = input[i][j] + fFIRCoeff*input[i][j-iDelayInSamples] + fIIRCoeff*paddedOutput[i][j-iDelayInSamples];
        }
    }
    
    for (int i = 0; i < numChannels ; i ++)
    {
        memcpy(output[i],&paddedOutput[i][iDelayInSamples],blockSize * sizeof(float));
    }
    
    return output;
}

int FilterAudio::getDelayInSamples() {
    return iDelayInSamples;
}