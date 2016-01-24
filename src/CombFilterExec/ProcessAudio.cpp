//
//  ProcessAudio.cpp
//  CombFilter
//
//  Created by Avrosh Kumar on 1/23/16.
//
//


#include <math.h>
#include <algorithm>    // std::copy

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
        int iNumBlocks = ceil(inputLength/hopSize);
        
        //Allocate single block for processing
        block = new float *[iNumChannels];
        for (int k=0; k<iNumChannels; k++) {
            block[k] = new float[inputLength];
        }
        
        //Iterate through every block
        for (int i=0; i<iNumBlocks; i++) {
            
            for (int n=0; n<iNumChannels; n++) {
                std::copy(input[n][i*hopSize],input[n][i*hopSize]+blockSize,block[n][0]);
            }
            
            //Call comb filter
            block = pFilter->combFilterBlock(block, blockSize, iNumChannels);
            
            //unblock
            
            for (int n=0; n<iNumChannels; n++) {
                
                //temp - delay length //
                int delaylength = 100;
                
                std::copy(block[n][0],block[n][blockSize],input[n][i*hopSize+delaylength]);
            }
        }
            
    }
}

