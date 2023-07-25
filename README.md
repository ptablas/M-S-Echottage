# M/S Echo-ttage.

M/S Echo-ttage is a Cross-platform VST3 audio plugin compatible with most Digital Audio Workstations (DAWs). It has been developed using the JUCE framework, given its extensive DSP library, GUI capabilities, and efficient API.

It splits an audio signal between the data that is common among left and right channels (Mid) and the data that is different, or in other words, is only on one of the stereo channels (Side). In addition, the now separated data can then be processed through a filter and a variable-time delay. 

Variable-Time Delays are the essence of many popular DSP algorithms, such as flangers, phasers, and choruses. By allowing time modulation to be user-definable, seamless transitions between different effects can be achieved. [More info here](https://github.com/ptablas/M-S-Echottage/wiki/Concept).

# Set-Up

1. Download the "MS Echo-ttage.vst3" file above.
2. Drag & Drop onto your DAW's VSTs folder.
3. Within your DAW, scan for new plugins.

# Acknowledgments

Most of the code pertaining to the "Osc" class used for the LFOs is heavily based on the set of lectures *C++ Real-Time Audio Programming with Bela* by Andrew McPherson.


