//
//  ProcessAudio.cpp
//  CombFilter
//
//  Created by Avrosh Kumar on 1/23/16.
//
//


#include <math.h>
#include <iostream>

#include "ProcessAudio.h"

/* The constructor method for ProcessAudio */
ProcessAudio::ProcessAudio(int sampleRate, int blockSize) {
    //set block size, sample rate and stuff
    this->iBlockSize = blockSize;
    this->iSampleRate = sampleRate;

}

/* An overloaded constructor method for ProcessAudio that includes hopSize */
ProcessAudio::ProcessAudio(int sampleRate, int blockSize, int hopSize) {
    //set block size, sample rate and stuff
    this->iBlockSize = blockSize;
    this->iHopSize = hopSize;
    this->iSampleRate = sampleRate;
}

/* The destructor method for ProcessAudio */
ProcessAudio::~ProcessAudio() {
    //delete all the allocated pointers
    
    delete pFilter;
    pFilter = 0;
}

/* The public method to setup filtering and delay buffer information */
void ProcessAudio::SetFilterProperties(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples, int iNumChannels) {
    pFilter = new FilterAudio(fFIRCoeff,fIIRCoeff,iDelayInSamples, iNumChannels);
}

/* The public method to perform the read blocks of audio, process and optionally write an output file */
void ProcessAudio::blockAndProcessAudio(CAudioFileIf *phAudioInputFile, CAudioFileIf *phAudioOutputFile, std::ofstream *txtFile) {
    
    // Check if Filter properties have been set before proceeding
    if (pFilter) {
        
        phAudioInputFile->getFileSpec(aFileSpec);
    
        allocateBlockMemory(aFileSpec.iNumChannels);
        
        while (!phAudioInputFile->isEof()) {
            
            long long iNumFrames = iBlockSize;
            phAudioInputFile->readData(ppfBlock, iNumFrames);
            
            // Apply comb filter to block
            ppfBlock = pFilter->combFilterBlock(ppfBlock, iBlockSize, aFileSpec.iNumChannels);
            
            //Write to output file
            phAudioOutputFile->writeData(ppfBlock, iBlockSize);
           
            //Write to text file if a pointer is made available
            if (txtFile) {
                for (int i=0; i<iBlockSize; i++) {
                    for (int j=0 ; j<aFileSpec.iNumChannels; j++) {
                        if (j!=0) {
                            *txtFile << ",";
                        }
                        *txtFile << ppfBlock[j][i];
                    }
                    *txtFile <<"\n";
                }
            }
        }
        
        freeBlockMemory(aFileSpec.iNumChannels);
            
    }
}

/* The private method to allocate a multichannel buffer of audio */
void ProcessAudio::allocateBlockMemory(int iNumChannels) {
    ppfBlock = new float *[iNumChannels];
    for (int k=0; k<iNumChannels; k++) {
        ppfBlock[k] = new float[iBlockSize];
    }
}

/* The private method to destroy the memory for the multichannel buffer of audio */
void ProcessAudio::freeBlockMemory(int iNumChannels) {
    for (int k=0; k<iNumChannels; k++) {
        delete [] ppfBlock[k];
    }
    delete [] ppfBlock;
}

