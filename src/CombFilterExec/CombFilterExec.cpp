#include <iostream>
#include <fstream>
#include <ctime>

#include "AudioFileIf.h"
#include "ProcessAudio.h"
#include "FilterAudio.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath,
                            sInputFileName,
                            sOutputFileName;
    
    float                   fFIRCoeff           = 0.0;
    float                   fIIRCoeff           = 0.0;
    int                     iDelayInMSec        = 0;
    int                     iSampleRate         = 0;
    int                     iBlockSize          = 1024;
//    int                     iHopSize            = 512;
    int                     iOverlapRatio       = 2;
    
    ProcessAudio            *pAudioProcessor;
    
    long long               iInFileLength       = 0;        //!< length of input file
    float                   **ppfAudioData      = 0;
    CAudioFileIf            *phAudioFile        = 0;
    CAudioFileIf            *phAudioFileOutput  = 0;
    int                     iNumChannels        = 1;
    CAudioFileIf::FileSpec_t aFileSpec          = {};
    CAudioFileIf::FileSpec_t aOutputFileSpec    = {};
    
    showClInfo ();
    
    // Parse command line arguments
    
    // Check argc
    if( argc == 6 ) {
        printf("Reading audio file %s%s\n", argv[1],argv[2]);
        printf("\nFIR Coefficient %f",atof(argv[3]));
        printf("\nIIR Coefficient %f",atof(argv[4]));
        printf("\nDelay in milliseconds %i",atoi(argv[5]));
        
        // Make sure gain is between -1 and 1, a number
        
        // Check that delay time is positive and nonzero. Set upper and lower limit 0 - 100ms
        
    }
    else if( argc > 6 ) {
        printf("Too many arguments supplied.\n");
        return 0;
    }
    else {
        printf("Please provide the file path, audio file name, FIR gain, IIR gain, and delay time in miliseconds.\n");
        return 0;
    }
    
    // Get audio file name
    sInputFilePath = argv[1];
    sInputFileName = argv[2];
    fFIRCoeff = atof(argv[3]);
    fIIRCoeff = atof(argv[4]);
    iDelayInMSec = atoi(argv[5]);
    sOutputFilePath = sInputFilePath;
    sOutputFileName = "output.wav";
    
    // Open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath+sInputFileName, CAudioFileIf::kFileRead, 0);
    
    // Open the output wav file for writing
    CAudioFileIf::create(phAudioFileOutput);
    
    // Allocate memory
    phAudioFile->getLength(iInFileLength);
    phAudioFile->getFileSpec(aFileSpec);
    iNumChannels = aFileSpec.iNumChannels;
    iSampleRate = aFileSpec.fSampleRateInHz;
    ppfAudioData = new float *[iNumChannels];
    for(int i = 0; i < iNumChannels; i++){
        ppfAudioData[i] = new float[iInFileLength];
    }
    
    // Get audio data
    phAudioFile->readData(ppfAudioData, iInFileLength);
    
    // Done reading safe to close
    phAudioFile->closeFile();
    
    // Instantiate a ProcessAudio object
    pAudioProcessor = new ProcessAudio(iSampleRate, iBlockSize, iBlockSize/iOverlapRatio);
    pAudioProcessor->SetFilterProperties(fFIRCoeff, fIIRCoeff, iDelayInMSec);
    pAudioProcessor->blockAndProcessAudio(ppfAudioData, iInFileLength, iNumChannels);
    
    // Do processing
    cout << "\nAll Done! COMB FILTERED" << endl << endl;
    
    // Write processed audio to a wav file
    aOutputFileSpec.fSampleRateInHz = aFileSpec.fSampleRateInHz;
    aOutputFileSpec.eFormat = CAudioFileIf::FileFormat_t::kFileFormatWav;
    aOutputFileSpec.eBitStreamType = CAudioFileIf::BitStream_t::kFileBitStreamInt16;
    aOutputFileSpec.iNumChannels = aFileSpec.iNumChannels;
    phAudioFileOutput->openFile(sOutputFilePath+sOutputFileName, CAudioFileIf::kFileWrite,&aOutputFileSpec);
    
    Error_t writeStatus = phAudioFileOutput->writeData(ppfAudioData, iInFileLength);
    
    cout<<"\nFile write status : "<<writeStatus<<endl;

    phAudioFileOutput->closeFile();
    
    // Clean-up and free memory
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phAudioFileOutput);
//    free(ppfAudioData);
    
    return 0;
}

void showClInfo()
{
    cout<<"Comb Filter ver1.0\n";
    return;
}

