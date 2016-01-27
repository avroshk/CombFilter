//
//  FilterAudio.hpp
//  CombFilter
//
//  Created by Christopher Latina on 1/23/16.
//
//

#ifndef FilterAudio_hpp
#define FilterAudio_hpp

#include <stdio.h>


class FilterAudio {
public:
    FilterAudio(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples, int iNumChannels); // Constructor
    ~FilterAudio(); // Destructor
    
    float **combFilterBlock(float **input, int blockSize, int numChannels);
    int getDelayInSamples();
    float getFIRCoeff();
    float getIIRCoeff();
    
    void clearDelayLines();

private:
    float fFIRCoeff;
    float fIIRCoeff;
    int iDelayInSamples;
    int iNumChannels;
    float **fFIRDelay;
    float **fIIRDelay;
};

#endif /* FilterAudio_h */
