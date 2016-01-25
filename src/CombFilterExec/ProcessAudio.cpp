//
//  ProcessAudio.cpp
//  CombFilter
//
//  Created by Avrosh Kumar on 1/23/16.
//
//


#include <math.h>
//#include <algorithm>    // std::copy
#include <iostream>

#include "ProcessAudio.h"

//constructor
ProcessAudio::ProcessAudio(int sampleRate, int blockSize, int hopSize) {
    //set block size, sample rate and stuff
    this->blockSize = blockSize;
    this->hopSize = hopSize;
    this->sampleRate = sampleRate;
}


ProcessAudio::~ProcessAudio() {
    //delete all the pointers to buffers
}
    
void ProcessAudio::SetFilterProperties(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples) {
    pFilter = new FilterAudio(fFIRCoeff,fIIRCoeff,iDelayInSamples);
}

void ProcessAudio::blockAndProcessAudio(float **input, int inputLength, int iNumChannels) {
    
    // Check if Filter properties have been set before proceeding
    if (pFilter) {
        
        //Allocate memory block for the output (same as length of input)
        ////Future task - we can try inplace substitution instead
//        float **output = new float*[iNumChannels];
//        for (int i=0; i<iNumChannels; i++) {
//            output[i] = new float[inputLength];
//        }
        
        //Number of blocks
        iNumBlocks = ceil(inputLength/hopSize);
        
        //Allocate single block for processing
        block = new float *[iNumChannels];
        for (int k=0; k<iNumChannels; k++) {
            block[k] = new float[inputLength];
        }
        
        //Iterate through every block
        for (int i=0; i<iNumBlocks-3; i++) {
            
            for (int n=0; n<iNumChannels; n++) {
                // Last block edge case
                if (i*hopSize+blockSize > inputLength) {
                    memcpy(block[n], &input[n][i*hopSize], (inputLength-i*hopSize)  * sizeof(float));
                }
                else {
                    memcpy(block[n], &input[n][i*hopSize], blockSize * sizeof(float));
                }
            }
            
            // Call comb filter
            block = pFilter->combFilterBlock(block, blockSize, iNumChannels);
            int iDelayLength = pFilter->getDelayInSamples();
            int iFiniteOffset = 1;
            
            // Check whether to offset by delayInSamples
            if( pFilter->getFIRCoeff() > 0){
                iFiniteOffset = 0;
            }
            
            // Unblock
            for (int n=0; n<iNumChannels; n++) {
                // Last block edge case
                if (i*hopSize+blockSize > inputLength) {
                    memcpy(&input[n][i*hopSize], &block[n][0], (inputLength-(i*hopSize)) * sizeof(float));
                } else {
                    memcpy(&input[n][i*hopSize+iFiniteOffset*iDelayLength], &block[n][iDelayLength], (hopSize) * sizeof(float));
                }
            
            }
        }
            
    }
}

