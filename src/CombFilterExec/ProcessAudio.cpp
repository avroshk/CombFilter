//
//  ProcessAudio.cpp
//  CombFilter
//
//  Created by Avrosh Kumar on 1/23/16.
//
//

#include "ProcessAudio.h"
#include <math.h>
#include <algorithm>    // std::copy


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

float** ProcessAudio::blockAndProcessAudio(float **input, int inputLength, int iNumChannels) {
  
    int iNumBlocks = ceil(inputLength/hopSize);
    
    //Initialize block memory
    block = new float *[iNumChannels];
    
    for (int k=0; k<iNumChannels; k++) {
        block[k] = new float[inputLength];
    }
    
    //process for every block
    for (int i=0; i<iNumBlocks; i++) {

        for (int m=0; m<iNumChannels; m++) {
            std::copy(input[m+i*blockSize],input[m+i*blockSize]+blockSize,&block[m]);
        }
        
        //Call comb filter
        
        //unblock
        
    }
    
    return output;
    
}

void ProcessAudio::processBlock(float* window, int windowBufferSize, int nChannels){
//    
//    for (int i=0; i<windowBufferSize; i++) {
//        leftInput[i] = window[i];
//    }
//    
//    soundMutex.lock();
//    middleInput = leftInput;
//    soundMutex.unlock();
//    
    // apply comb filter to the block
    
}

void ProcessAudio::unblockAudio() {

}
