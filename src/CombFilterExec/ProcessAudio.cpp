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
//        output = new float*[iNumChannels];
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
        for (int i=0; i<iNumBlocks; i++) {
            
            for (int n=0; n<iNumChannels; n++) {
                if (i*hopSize+blockSize > inputLength) {
                    memcpy(block[n], &input[n][i*hopSize], (inputLength-i*hopSize)  * sizeof(float));
                }
                else {
                    memcpy(block[n], &input[n][i*hopSize], blockSize * sizeof(float));
                }
            }
            
            //Call comb filter
            block = pFilter->combFilterBlock(block, blockSize, iNumChannels);
            int iDelayLength = pFilter->getDelayInSamples();
            
            //unblock
            for (int n=0; n<iNumChannels; n++) {
                if (i==0) {
                    memcpy(block[n], &input[n][0], (hopSize+iDelayLength) * sizeof(float));
                }
                else if (i*hopSize+iDelayLength+hopSize+iDelayLength > inputLength) {
                    memcpy(block[n], &input[n][i*hopSize+iDelayLength], (inputLength-(i*hopSize+iDelayLength)) * sizeof(float)); //buggy - check again
                }
                else {
                    memcpy(&input[n][i*hopSize+iDelayLength], block[n], (hopSize+iDelayLength) * sizeof(float));
                }
            }
        }
            
    }
}

