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

CombFilterProject::CombFilterProject(int iBlockSize, int iOverlapRatio)
{
    // this never hurts
    this->reset ();
    
    this->iBlockSize = iBlockSize;
    this->iOverlapRatio = iOverlapRatio;
}


CombFilterProject::~CombFilterProject()
{
    this->reset ();
    
    phAudioFile->destroy(phAudioFile);
    phAudioFileOutput->destroy(phAudioFileOutput);
    
    
    for(int i = 0; i < iNumChannels; i++) {
        delete [] ppfAudioData[i];
    }
    delete [] ppfAudioData;
    
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

Error_t CombFilterProject::create(CombFilterProject*& pCombFilterProject, int iBlockSize, int iOverlapRatio)
{
    pCombFilterProject = new CombFilterProject(iBlockSize,iOverlapRatio);
    
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

Error_t CombFilterProject::init(string sInputFilePath, string sInputFileName, string sOutputFilePath, string sOutputFileName, float fFIRCoeff, float fIIRCoeff, int iDelayInMSecs)
{
    this->sInputFilePath = sInputFilePath;
    this->sInputFileName = sInputFileName;
    this->sOutputFilePath = sOutputFilePath;
    this->sOutputFileName = sOutputFileName;
    
    this->fFIRCoeff = fFIRCoeff;
    this->fIIRCoeff = fIIRCoeff;
    this->iDelayInMSecs = iDelayInMSecs;
    
    // Create the input and output wave file for reading/writing
    CAudioFileIf::create(phAudioFile);
    CAudioFileIf::create(phAudioFileOutput);
  
    return kNoError;
}

Error_t CombFilterProject::readAudio() {
    // Open the input wav file for reading
    phAudioFile->openFile(sInputFilePath+sInputFileName, CAudioFileIf::kFileRead, 0);
    
    // Allocate memory
    phAudioFile->getLength(iInFileLength);
    phAudioFile->getFileSpec(aInputFileSpec);
    iNumChannels = aInputFileSpec.iNumChannels;
    iSampleRate = aInputFileSpec.fSampleRateInHz;
    ppfAudioData = new float *[iNumChannels];
    for(int i = 0; i < iNumChannels; i++){
        ppfAudioData[i] = new float[iInFileLength];
    }
    
    iDelayInSamples = (float)iSampleRate*(float)iDelayInMSecs/1000.0f;
    
    if (iDelayInSamples > iBlockSize) {
        int maxAllowedDelay = (float)iBlockSize*1000.0f/(float)iSampleRate;
        cout<<"\nDelay should not be longer than "<<maxAllowedDelay<<"ms with current block size."<<endl;
        
        return kDelayTooLongError;
    }
    
    // Get audio data
    phAudioFile->readData(ppfAudioData, iInFileLength);
    
    // Done reading safe to close
    phAudioFile->closeFile();
    
    return kNoError;
}

Error_t CombFilterProject::processAudio() {
    
    pAudioProcessor = new ProcessAudio(iSampleRate, iBlockSize, iBlockSize/iOverlapRatio);
    pAudioProcessor->SetFilterProperties(fFIRCoeff, fIIRCoeff, iDelayInSamples);
    pAudioProcessor->blockAndProcessAudio(ppfAudioData, iInFileLength, iNumChannels);
    
    return kNoError;
}

Error_t CombFilterProject::writeAudio() {
    aOutputFileSpec = aInputFileSpec;
    phAudioFileOutput->openFile(sOutputFilePath+sOutputFileName, CAudioFileIf::kFileWrite,&aOutputFileSpec);
    phAudioFileOutput->writeData(ppfAudioData, iInFileLength);
    phAudioFileOutput->closeFile();
    
    return kNoError;
}

Error_t CombFilterProject::writeAudioToText(string fileName) {

    //Create file
    ofCSVfile.open(sOutputFilePath+fileName);
    
    for (int i=0; i<iInFileLength; i++) {
        for (int j=0 ; j<iNumChannels; j++) {
            if (j!=0) {
                ofCSVfile << ",";
            }
            ofCSVfile << ppfAudioData[j][i];
        }
        ofCSVfile <<"\n";
    }
    
    return kNoError;
}

int CombFilterProject::getDelayinSamples() {
    return iDelayInSamples;
}

Error_t CombFilterProject::reset ()
{
    // reset buffers and variables to default values
    
    fFIRCoeff = 0;
    fIIRCoeff = 0;
    iDelayInMSecs = 0;
    iDelayInSamples = 0;
    iSampleRate = 44100;
    iBlockSize = 1024;
    iOverlapRatio = 1;
    
    iInFileLength = 0;
    iNumChannels = 1;
    ppfAudioData = 0;
    phAudioFile = 0;
    phAudioFileOutput = 0;
    aInputFileSpec = (CAudioFileIf::FileSpec_t){};
    aOutputFileSpec = (CAudioFileIf::FileSpec_t){};
    
    return kNoError;
}




