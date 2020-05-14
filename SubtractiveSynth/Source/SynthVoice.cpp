/*
  ==============================================================================

    SynthVoice.cpp
    Created: 14 May 2020 11:22:52am
    Author:  Francesco

  ==============================================================================
*/

#include<JuceHeader.h>
#include "SynthVoice.h"

HashMap<int, OscillatorBase*> OscillatorBase::voiceMap;
