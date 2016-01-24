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
    ProcessAudio(int sampleRate, int blockSize, int hopSize); //constructor
    ~ProcessAudio(); //destructor
    
    float** blockAndProcessAudio(float **input, int length, int numChannels);
    void processBlock(float* window, int windowBufferSize, int nChannels);
    void unblockAudio();
    
    
private:
    int blockSize;
    int hopSize;
    int sampleRate;
    int numBlocks;
    float** block;
    float** output;
};


#endif /* ProcessAudio_hpp */
