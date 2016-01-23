#include <iostream>
#include <fstream>
#include <ctime>

#include "AudioFileIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
    sOutputFilePath;
    
    float                   fFIRCoeff           = 0.0;
    float                   fIIRCoeff           = 0.0;
    int                     iDelayInMSec        = 0;
    int                     iSampleRate         = 0;
    int                     iBlockSize          = 1024;
    int                     iHopSize            = 512;
    
    long long               iInFileLength       = 0;        //!< length of input file
    clock_t                 time                = 0;
    float                   **ppfAudioData      = 0;
    CAudioFileIf            *phAudioFile        = 0;
    int                     iNumChannels        = 1;
    CAudioFileIf::FileSpec_t aFileSpec          = {};
    
    showClInfo ();
    
    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    
    // check argc
    if( argc == 4 ) {
        printf("Reading audio file %s\n", argv[1]);
        printf("\nFIR Coefficient %f",atof(argv[2]));
        printf("\nIIR Coefficient %f",atof(argv[3]));
        printf("\nDelay in milliseconds %i",atoi(argv[4]));
        
        //Make sure gain is between -1 and 1, a number
        
        // Check that delay time is positive and nonzero. Set upper and lower limit 0 - 100ms
        
    }
    else if( argc > 4 ) {
        printf("Too many arguments supplied.\n");
        return 0;
    }
    else {
        printf("Please provide the audio file path, FIR gain, IIR gain, and delay time in miliseconds.\n");
        return 0;
    }
    
    // get audio file name
    sInputFilePath = argv[1];
    fFIRCoeff = atof(argv[2]);
    fIIRCoeff = atof(argv[3]);
    iDelayInMSec = atoi(argv[4]);
    
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead, 0);
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    phAudioFile->getLength(iInFileLength);
    phAudioFile->getFileSpec(aFileSpec);
    iNumChannels = aFileSpec.iNumChannels;
    ppfAudioData = new float *[iNumChannels];
    for(int i = 0; i < iNumChannels; i++){
        ppfAudioData[i] = new float[iInFileLength];
    }
    
    // get audio data
    phAudioFile->readData(ppfAudioData, iInFileLength);
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio info and print it to stdout
    std::ofstream myfile;
    myfile.open ("example.txt");
    for(int i = 0; i < iNumChannels; i++){
        for (int j = 0; j < iInFileLength; j++){
            myfile << ppfAudioData[i][j];
        }
    }
    myfile.close();
    
    //////////////////////////////////////////////////////////////////////////////
    // do processing
    cout << "All Done!" << endl << endl;
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    
    CAudioFileIf::destroy(phAudioFile);
    free(ppfAudioData);
    
    return 0;
    
}


void     showClInfo()
{
    cout<<"Comb Filter ver1.0\n";
    
    return;
}

