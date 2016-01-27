//
//  CombFilterProject.hpp
//  CombFilter
//
//  Created by Avrosh Kumar on 1/24/16.
//
//

#ifndef CombFilterProject_hpp
#define CombFilterProject_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>


#include "ErrorDef.h"
#include "AudioFileIf.h"
#include "ProcessAudio.h"
#include "FilterAudio.h"

using namespace std;

class CombFilterProject {
public:
    /*! version number */
    enum Version_t
    {
        kMajor,                         //!< major version number
        kMinor,                         //!< minor version number
        kPatch,                         //!< patch version number
        
        kNumVersionInts = 3
    };
    
    enum WriteMode_t
    {
        kWriteInputArray,
        kWriteOutputArray
    };
    
    static const int  getVersion (const Version_t eVersionIdx);
    static const char* getBuildDate ();
    
    static Error_t create (CombFilterProject*& pCKortIf, int iBlockSize);
    static Error_t destroy (CombFilterProject*& pCKortIf);
    
    Error_t init(string sInputFilePath, string sInputFileName, string sOutputFilePath, string sOutputFileName, float fFIRCoeff, float fIIRCoeff, int iDelayInMSecs, bool bWriteToTxtFile = false);
    Error_t reset ();
//    Error_t readAudio();
    Error_t processAudio();
//    Error_t writeAudio();
    Error_t writeAudioToText(string fileName);
    int getDelayinSamples();
    
    
private:
    string sInputFilePath;
    string sOutputFilePath;
    string sInputFileName;
    string sOutputFileName;
    string sOutputTextFileName;
    
    float fFIRCoeff;
    float fIIRCoeff;
    int iDelayInSamples;
    int iDelayInMSecs;
    int iSampleRate;
    int iBlockSize;
    bool isWriteToTxtFileEnabled;
    
    ProcessAudio *pAudioProcessor;
    
    long long iInFileLength;
    float **ppfAudioData;
    CAudioFileIf *phAudioFile;
    CAudioFileIf *phAudioFileOutput;
    int iNumChannels;
    CAudioFileIf::FileSpec_t aInputFileSpec;
    CAudioFileIf::FileSpec_t aOutputFileSpec;
    ofstream ofCSVfile;

   
protected:
    CombFilterProject(int);
    virtual ~CombFilterProject();

};

#endif /* CombFilterProject_hpp */








