# CombFilter
Assignment 1 - Implementation of two types of Comb Filter

<h3>Comb filter library (FIR & IIR)</h3>

1. <b>Design a class interface based on the template MyProject.h [20]</b>
Use parameter units that are understandable by the user, e.g., seconds instead of samples. Properly document all functions of the class interface. Consider the following questions: What parameters can be adjusted during processing? When (and by whom) is the maximum allowed delay set? What parameters can be known during instantiation? How/Where do you set them? Will your interface work for all samplerates and all channel configurations? How can the user access both types (see 2. + 3. below) from one interface? Is your process function inplace or not? Document your thoughts on the design, solutions, and answers to these questions.

Our class comb filter contained in the class FilterAudio is called from ProcessAudio, which handles reading, blocking, and writing of an audio file. During processing in the ProcessAudio class, an instance of filterAudio can be updated by calling the public function SetFilterProperties(float fFIRCoeff, float fIIRCoeff, int iDelayInSamples, int iNumChannels). This is currently called only once at the beginning of CombFilterProject::processAudio(). One can process multichannel audio files with this tool.

We've set our maximum delay to 2s because a "comb filter" will simply become a delay if the delay time is large. The property fMaxAllowedDelay can be updated. Our processing function is in place, so the delay line can be larger than the block size. 


2. <b>Implement FIR comb filter (DAFX: fircomb.m) [15]</b> See FilterAudio::combFilterBlock
3. <b>Implement IIR comb filter (DAFX: iircomb.m)  [15]</b> See FilterAudio::combFilterBlock
4. <b>Implement main command line function  [12]</b> 
The command line application instantiates your class with the correct parameters, read the parameters from the command line (file name, effect-parameters), reads and processes audio data in blocks and writes the result to an output file.

If running the program with too few or too many arguments, we tell you what arguments to provide. 

5. <b>verify functionality for 2 different input audio files by comparison with matlab   [08]</b>

See directory "/Matlab resource/test"

6. <b>write the following test functions to ensure functionality [30]</b> <br />
    a. zero output if input frequeny matches feedforward (FIR)         
    b. magnitude increase/decrease if input frequency matches feedback (IIR)        
    c. varying input block size (FIR & IIR)                                                                              
    d. zero input signal (FIR & IIR)                                                                                        
    e. additional test to verify correct functionality       
