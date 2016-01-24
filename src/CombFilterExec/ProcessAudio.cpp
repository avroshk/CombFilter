//
//  ProcessAudio.cpp
//  CombFilter
//
//  Created by Avrosh Kumar on 1/23/16.
//
//

#include "ProcessAudio.hpp"

//constructor
ProcessAudio::ProcessAudio(int blockSize, int sampleRate) {
    //set block size, sample rate and stuff
    this->blockSize = blockSize;
    this->sampleRate = sampleRate;
}

ProcessAudio::~ProcessAudio() {
    //delete all the pointers to buffers
}

void ProcessAudio::blockAndProcessAudio(float *input, int inputLength, int numChannels) {
  
//    int numBlocks = inputLength/blockSize;
    
// 
//    if (numHops > OVERLAPMULTIPLE-1) {
//        processBlock(block,BUFFERSIZE*OVERLAPMULTIPLE,nChannels);
//        //Shift block data to left
//        copy(block+BUFFERSIZE, block+BUFFERSIZE*OVERLAPMULTIPLE, &block[0]);
//        //Push the last hop into the block
//        copy(input, input + BUFFERSIZE, &block[BUFFERSIZE*(OVERLAPMULTIPLE-1)]);
//    }
//    else {
//        copy(input, input + BUFFERSIZE, &block[NUMHOPS*BUFFERSIZE]);
//        numHops++;
//    }
//    
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