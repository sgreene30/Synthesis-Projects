#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;

FractalRandomGenerator<ClockedNoise, 5> fract;
Oscillator                              lfo[2];
float sample_rate = 96000;

enum
{
	freq, color
};

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    for(size_t i = 0; i < size; i++)
    {
        //fract.SetFreq(fabsf(lfo[0].Process()));
		fract.SetFreq(fmap(board.get_knob(freq), 0.0f, sample_rate/2.0f));
        fract.SetColor(board.get_knob(color));
        out[0][i] = out[1][i] = fract.Process();
    }
}

int main(void)
{
    board.add_knob(freq, board.knob_1);
	board.add_knob(color, board.knob_2);

	board.Init();
    board.seed.SetAudioBlockSize(4);
    board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);

    fract.Init(sample_rate);
    fract.SetFreq(sample_rate / 10.f);

    lfo[0].Init(sample_rate);
    lfo[1].Init(sample_rate);

    lfo[0].SetFreq(.25f);
    lfo[0].SetAmp(sample_rate / 3.f);
    lfo[1].SetFreq(.1f);
    lfo[1].SetAmp(1.f);


    board.seed.StartAudio(AudioCallback);
    while(1) 
	{
		//lfo[0].SetAmp(fmap(board.get_knob(freq), 0.0f, sample_rate/2.0f, Mapping::LINEAR));
	}
}
