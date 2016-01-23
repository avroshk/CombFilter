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
    if( argc == 2 ) {
        printf("Reading audio file %s\n", argv[1]);
    }
    else if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
        return 0;
    }
    else {
        printf("Please provide the audio file path.\n");
        return 0;
    }
    
    // get audio file name
    sInputFilePath = argv[1];
    
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

