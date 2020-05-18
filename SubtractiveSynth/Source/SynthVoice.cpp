

#include<JuceHeader.h>
#include "SynthVoice.h"

std::map<int, std::unique_ptr<OscillatorBase>> OscillatorBase::voiceMap;
