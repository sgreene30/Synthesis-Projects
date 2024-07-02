#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;
int blocksize = 32;
float fs = 96000;
voice osc;
bool trigger = false;
bool last_gate_in = false;
bool gate_in = false;

enum
{
	freq, waveshape, attack, decay, gate, impact
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = osc.Process();
		out[1][i] = in[1][i];
	}
}

int main(void)
{
	board.add_dual_control(freq, board.in_2, board.knob_1);
	board.add_dual_control(impact, board.gate_1_in_4, board.knob_4);
	board.add_dual_control(waveshape, board.in_3, board.knob_3);
	board.add_dual_control(attack, board.in_6, board.knob_6);
	board.add_dual_control(decay, board.in_7, board.knob_7);

	board.add_gate_in(gate, board.in_1);


	board.Init();
	board.seed.SetAudioBlockSize(blocksize); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	board.seed.StartAudio(AudioCallback);

	osc.Init(fs);

	while(1) 
	{
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);	
		osc.set_freq(board.get_dual_control(freq));
		osc.set_amp_env_attack(board.get_dual_control(attack));
		osc.set_amp_env_decay(board.get_dual_control(decay));
		osc.set_impact(board.get_dual_control(impact));

		board.button.Debounce();
		gate_in = board.get_gate_in(gate);
		trigger = gate_in & !last_gate_in;
		trigger = trigger || board.button.RisingEdge();
		last_gate_in = gate_in;

		if(trigger)
		{
			osc.env_trigger();
		}
	
		board.LED_LEFT.Write(trigger);
		board.LED_RIGHT.Write(trigger);
	}
}
