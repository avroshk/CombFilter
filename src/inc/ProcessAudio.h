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
#include "FilterAudio.h"

class ProcessAudio {
public:
    ProcessAudio(int sampleRate, int blockSize, int hopSize); //constructor
    ~ProcessAudio(); //destructor
    
    void blockAndProcessAudio(float **input, int length, int numChannels);
    void SetFilterProperties(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples);
    
   
    
    
private:
    int blockSize;
    int hopSize;
    int sampleRate;
    int numBlocks;
    float** block;
//    float** output;  //doing inplace processesing so not required
    
    FilterAudio *pFilter;
};


#endif /* ProcessAudio_hpp */
