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
#include "AudioFileIf.h"
#include "FilterAudio.h"

class ProcessAudio {
public:
    ProcessAudio(int sampleRate, int blockSize); //constructor
    ProcessAudio(int sampleRate, int blockSize, int hopSize); //contructor - not used in this project
    ~ProcessAudio(); //destructor
    
    void blockAndProcessAudio(CAudioFileIf *phAudioInputFile, CAudioFileIf *phAudioOutputFile, std::ofstream *txtFile = 0);
    void SetFilterProperties(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples, int iNumChannels);
    
private:
    int iBlockSize;
    int iHopSize;
    int iSampleRate;
    float** ppfBlock;
    
    CAudioFileIf::FileSpec_t aFileSpec;
    FilterAudio *pFilter;
    
    void allocateBlockMemory(int iNumChannels);
    void freeBlockMemory(int iNumChannels);
    
};


#endif /* ProcessAudio_hpp */
