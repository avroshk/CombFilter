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
    FilterAudio(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples); // Constructor
    ~FilterAudio(); // Destructor
    
    float **combFilterBlock(float **input, int blockSize, int numChannels);
    int getDelayInSamples();
    float getFIRCoeff();
    float getIIRCoeff();

private:
    float fFIRCoeff;
    float fIIRCoeff;
    int iDelayInSamples;
    
    
};

#endif /* FilterAudio_h */
