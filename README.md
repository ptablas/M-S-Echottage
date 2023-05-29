# M/S Echo-ttage

M/S Echo-ttage is a Mid/Side processing unit that features stereo widening and independent filter and variable-time delay through LFO capabilities per channel.

This allows for seamless transitions between different popular DSP algorithms - from a simple delay to a flanger, phaser, chorus or extreme feedback soundscapes -, and everything in between, performed independently to the Mid or Side channels. This is performed through the use of an LFO per channel that modulates the delay time of the fractional delay.

# Wiki

## Guide

### Input and Output Selection (i & o)

Input selection ("i" on GUI) allows for:
 - Stereo: Encode an Stereo Track for Mid/Side processing.
 - Mid/Side: Process a Mid/Side Track.
 
 Output selection ("o" on GUI) allows for:
 - Stereo: Decode the Mid/Side channels into Stereo ready for listening.
 - Mid/Side: Keep in Mid/Side format for further external processing.
 
 *Note Mid/Side input options assume Mid is the incoming track's left and Side is the right, while Mid/Side output will output again in that order.
 
###### Tip: Daisy-chain. 

Decode to Mid/Side and add an extra M/S Echo-ttage (with Mid/Side input encoding) to daisy-chain variable delays!

### Stereo Widening

Mixes the mid and side signals:
 - at the centre, mid and side are equally audible
 - at the left limit, only the mid signal will be audible
 - at the right limit, only the right signal will be audible
 
Note that when Mid/Side output is chosen, stereo widening will act as a panning fader.

### Filter

A simple state variable filter with low-pass, band-pass and high-pass capabilities.
Resonance is kept low to avoid clipping.

- "f" changes the filter's centre frequency
- The comboBox chooses the filter type.
- The vertical slider assings a resonance value.

### Delay 

- "fx" modifies the amount of dry and wet signal. The dials' left limit is completely dry, right limit is wet.
- "t" changes the amount of delay in samples. Dials' left limit is 0, while dial's right limit is 2000 samples.
- "fb" changes the amount of previous delay fed back into the signal (feedback). max limit is 0.7 of the signal to avoid infinite loop.

### LFO Section

The LFO section defines an LFO that varies the 

- "hz" varies the LFO frequency. Maximum value is 20 Hz.
- "A" varies the depth of the LFO. Measured in samples since it varies the delay time (in samples). Max value is 1000.

*note the lower limit is 0, using this value in "hz" or "A" effectively means by-passing the LFO.

###### Tip: != bound behaviour.

When the LFO pushes the delay "t" by the left limit, delay time bounces back in order to avoid negative values. However, the right limit can be crossed, allowing for an extra 1000 samples of delay time.

## Concept 

### Mid/Side Processing

Mid/Side processing decodes the information into two separate channels: mid and side. While the Mid channel includes all the information that apperas in both channels, side has the information that is panned to one of the channels only - in other words, that differs from one channel to another (Dory R., 2011).

Mid/Side channels separation can be useful in many situations. Vocals or kickdrums are usually panned centre: introducing a filter to only the mid channel can be a helpful tool to remove frequency content - as is done for Karaoke (original vocal removing) or Djing (Kick Drum removal) - with a smaller impact to the overall mix. 

### Delay-based sound effects.

Delay is the essence of many other sound effects. Filters and Comb filters make use of very short delays - usually one from a few samples (Smith, J.O., 2007); chorus, phasers and flangers use delays of about 20ms instead that modulate progressively over time (Bartlett, 1970).

Although some of these effects require external capabilities (such as feedforward for flangers or multi-tap delay for filters), this VST aims to show pragmatically the wide scope of delay effects. Allowing this control in combination with Mid/Side capabilities allows the user to introduce this variety of effects to the independent Mid and Side parts accurately, allowing huge amounts of control to already-finished stereo tracks.


## Notes

### Interpolation issues and fast delay time changes.

Although it is possible to modulate the delay time at extreme LFO limits, artifacts occur. This is even after stages of linear interpolation performed in the parameters that modulate delay time, which, moreover, goes through low-pass filtering and, eventually, 3rd order Lagrange interpolation through the delay line class itself.

It is likely these sort of capabilities should not be performed on a sample per sample basis - instead they might be better dealt with as blocks.

Either way, this appears to be subject of literature https://core.ac.uk/download/344942451.pdf - and probably outwith the scope of the project at hand. However, further work in this direction should be undertaken.


## Bibliography:

- B. Bartlett, ``A scientific explanation of phasing (flanging),'' Journal of the Audio Engineering Society, vol. 18, pp. 674-675, Jun 1970.
- J. O. Smith, Introduction to Digital Filters with Audio Applications, http://ccrma.stanford.edu/~jos/filters/, Sept. 2007, online book.(Accessed: April 28, 2023). 
- Dow, R. (2023) Creative Mid/Side Processing, Creative mid/side processing. Available at: https://www.soundonsound.com/techniques/creative-midside-processing (Accessed: April 28, 2023). 
