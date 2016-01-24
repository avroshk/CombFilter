//
//  ProcessAudio.hpp
//  CombFilter
//
//  Created by Avrosh Kumar on 1/23/16.
//
//

#ifndef ProcessAudio_hpp
#define ProcessAudio_hpp

#include <stdio.h>

class ProcessAudio {
public:
    ProcessAudio(int blockSize, int sampleRate); //constructor
    ~ProcessAudio(); //destructor
    
    void blockAndProcessAudio(float *input, int length, int numChannels);
    void processBlock(float* window, int windowBufferSize, int nChannels);
    
    
private:
    int blockSize;
    int sampleRate;
    int numBlocks;
    float* block;
    
};


#endif /* ProcessAudio_hpp */
