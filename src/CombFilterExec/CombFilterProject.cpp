//
//  CombFilterProject.cpp
//  CombFilter
//
//  Created by Avrosh Kumar on 1/24/16.
//
//


// standard headers


// project headers
#include "CombFilterProject.h"
#include "ErrorDef.h"

static const char*  kCombFilterProjectBuildDate = __DATE__;

static const int    CombFilterProject_VERSION_MAJOR = 1,
                    CombFilterProject_VERSION_MINOR = 1,
                    CombFilterProject_VERSION_PATCH = 1;

CombFilterProject::CombFilterProject(int iBlockSize)
{
    this->reset();
    
    this->iBlockSize = iBlockSize;
}


CombFilterProject::~CombFilterProject()
{
    this->reset ();
    
    phAudioInputFile->destroy(phAudioInputFile);
    phAudioFileOutput->destroy(phAudioFileOutput);
    
    delete pAudioProcessor;
    pAudioProcessor = 0;
}

const int CombFilterProject::getVersion (const Version_t eVersionIdx)
{
    int iVersion = 0;
    
    switch (eVersionIdx)
    {
        case kMajor:
            iVersion    = CombFilterProject_VERSION_MAJOR;
            break;
        case kMinor:
            iVersion    = CombFilterProject_VERSION_MINOR;
            break;
        case kPatch:
            iVersion    = CombFilterProject_VERSION_PATCH;
            break;
        case kNumVersionInts:
            iVersion    = -1;
            break;
    }
    
    return iVersion;
}
const char*  CombFilterProject::getBuildDate ()
{
    return kCombFilterProjectBuildDate;
}

Error_t CombFilterProject::create(CombFilterProject*& pCombFilterProject, int iBlockSize)
{
    pCombFilterProject = new CombFilterProject(iBlockSize);
    
    if (!pCombFilterProject)
        return kUnknownError;

    return kNoError;
}

Error_t CombFilterProject::destroy (CombFilterProject*& pCombFilterProject)
{
    if (!pCombFilterProject)
        return kUnknownError;
    
    pCombFilterProject->reset ();
    
    delete pCombFilterProject;
    pCombFilterProject = 0;
    
    return kNoError;
    
}

Error_t CombFilterProject::init(string sInputFilePath, string sInputFileName, string sOutputFilePath, string sOutputFileName, float fFIRCoeff, float fIIRCoeff, float fDelayInMSecs, bool bWriteToTxtFile)
{
    
    this->sInputFilePath = sInputFilePath;
    this->sInputFileName = sInputFileName;
    this->sOutputFilePath = sOutputFilePath;
    this->sOutputFileName = sOutputFileName;
    sOutputTextFileName = "output.csv";
    
    this->fFIRCoeff = fFIRCoeff;
    this->fIIRCoeff = fIIRCoeff;
    this->fDelayInMSecs = fDelayInMSecs;
    
    // Create the input and output wave file for reading/writing
    CAudioFileIf::create(phAudioInputFile);
    CAudioFileIf::create(phAudioFileOutput);
    
    // Prepare the input wav file for reading
    iFileOpenStatus = phAudioInputFile->openFile(sInputFilePath+sInputFileName, CAudioFileIf::kFileRead, 0);
    if (iFileOpenStatus != 0) {
        return kFileOpenError;
    }
    
    phAudioInputFile->getLength(iInFileLength);
    phAudioInputFile->getFileSpec(aInputFileSpec);
    aOutputFileSpec = aInputFileSpec;
    iNumChannels = aInputFileSpec.iNumChannels;
    iSampleRate = aInputFileSpec.fSampleRateInHz;
    
    // Prepare the output wav file for reading
    iFileOpenStatus = phAudioFileOutput->openFile(sOutputFilePath+sOutputFileName, CAudioFileIf::kFileWrite,&aOutputFileSpec);
    if (iFileOpenStatus != 0) {
        return kFileOpenError;
    }
    
    iDelayInSamples = (float)iSampleRate*fDelayInMSecs/1000.0f;
    
    if (bWriteToTxtFile) {
        //Create file
        isWriteToTxtFileEnabled = true;
        ofCSVfile.open(sOutputFilePath+sOutputTextFileName);
    }
    
    if (fDelayInMSecs > fMaxAllowedDelay) {
        
        cout<<"\nDelay should not be longer than "<<fMaxAllowedDelay<<" ms."<<endl;
        
        return kDelayTooLongError;
    }
  
    return kNoError;
}

Error_t CombFilterProject::processAudio() {
    
    pAudioProcessor = new ProcessAudio(iSampleRate, iBlockSize);
    pAudioProcessor->SetFilterProperties(fFIRCoeff, fIIRCoeff, iDelayInSamples, iNumChannels);
    if(isWriteToTxtFileEnabled) {
        pAudioProcessor->blockAndProcessAudio(phAudioInputFile, phAudioFileOutput, &ofCSVfile);
    } else {
        pAudioProcessor->blockAndProcessAudio(phAudioInputFile, phAudioFileOutput);
    }
    
    phAudioInputFile->closeFile();
    phAudioFileOutput->closeFile();
    
    return kNoError;
}

int CombFilterProject::getDelayinSamples() {
    return iDelayInSamples;
}

Error_t CombFilterProject::reset ()
{
    // reset buffers and variables to default values
    
    fFIRCoeff = 0.0;
    fIIRCoeff = 0.0;
    fDelayInMSecs = 0.0;
    iDelayInSamples = 0;
    iSampleRate = 44100;
    iBlockSize = 1024;
    fMaxAllowedDelay = 2000; //2 seconds
    
    iInFileLength = 0;
    iNumChannels = 1;
    phAudioInputFile = 0;
    phAudioFileOutput = 0;
    aInputFileSpec = (CAudioFileIf::FileSpec_t){};
    aOutputFileSpec = (CAudioFileIf::FileSpec_t){};
    
    return kNoError;
}

void CombFilterProject::setMaxAllowedDelay(float fMaxDelay) {
    fMaxAllowedDelay = fMaxDelay;
}

float CombFilterProject::getMaxAllowedDelay() {
    return fMaxAllowedDelay;
}




