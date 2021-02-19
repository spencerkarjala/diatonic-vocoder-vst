# Diatonic Vocoder
A vocoder-type VST effect that processes the incoming audio buffer to attenuate non-diatonic (ie. outside of a given melodic scale) frequencies, effectively transforming atonal noise to design sounds that are harmonious in nature. The current iteration works as follows:
1. Continuously maintain an overlapping windowed STFT of the incoming audio.
2. Scale frequency bins inversely by their distance to the nearest diatonic frequency.
3. Perform the inverse FFT of the resulting spectrum.
4. Write the resulting time-domain data back to the audio buffer.

This program works on the JUCE framework, and so can be compiled to both a VST3 plugin and as a standalone program. It utilizes the built-in OpenGL support within JUCE to draw custom graphics using shaders.

## TODO: Add v1 GUI and processing samples