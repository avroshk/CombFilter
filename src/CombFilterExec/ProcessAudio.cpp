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

//constructor
ProcessAudio::ProcessAudio(int sampleRate, int blockSize) {
    //set block size, sample rate and stuff
    this->blockSize = blockSize;
    this->sampleRate = sampleRate;

}

ProcessAudio::ProcessAudio(int sampleRate, int blockSize, int hopSize) {
    //set block size, sample rate and stuff
    this->blockSize = blockSize;
    this->hopSize = hopSize;
    this->sampleRate = sampleRate;
}


ProcessAudio::~ProcessAudio() {
    //delete all the pointers to buffers
    delete pFilter;
    pFilter = 0;
}

void ProcessAudio::SetFilterProperties(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples, int iNumChannels) {
    pFilter = new FilterAudio(fFIRCoeff,fIIRCoeff,iDelayInSamples, iNumChannels);
}

void ProcessAudio::blockAndProcessAudio(CAudioFileIf *phAudioInputFile, CAudioFileIf *phAudioOutputFile, std::ofstream *txtFile) {
    
    // Check if Filter properties have been set before proceeding
    if (pFilter) {
        
        phAudioInputFile->getFileSpec(aFileSpec);
    
        //Allocate single block for processing
        ppfBlock = new float *[aFileSpec.iNumChannels];
        for (int k=0; k<aFileSpec.iNumChannels; k++) {
            ppfBlock[k] = new float[blockSize];
        }
        
        while (!phAudioInputFile->isEof()) {
            
            long long iNumFrames = blockSize;
            phAudioInputFile->readData(ppfBlock, iNumFrames);
            
            // Call comb filter
            ppfBlock = pFilter->combFilterBlock(ppfBlock, blockSize, aFileSpec.iNumChannels);
            
            //Write to output file
            phAudioOutputFile->writeData(ppfBlock, blockSize);
           
            if (txtFile) {
                for (int i=0; i<blockSize; i++) {
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
        
        //free memory allocated for block
        for (int k=0; k<aFileSpec.iNumChannels; k++) {
            delete [] ppfBlock[k];
        }
        delete [] ppfBlock;
            
    }
}

