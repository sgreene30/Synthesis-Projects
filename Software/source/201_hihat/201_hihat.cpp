#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;
int blocksize = 32;
float fs = 96000;
bool trigger = false;
bool last_gate_in = false;
bool gate_in = false;

voice hihat;

enum
{
	bandpass_freq, bandpass_q, highpass_freq, decay, accent, gate, oscillator_tune
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = hihat.Process();
		out[1][i] = 0.0f;
	}
}

int main(void)
{
	board.add_gate_in(gate, board.in_1);

	board.add_dual_control(bandpass_freq, board.in_2, board.knob_1);
	board.add_dual_control(bandpass_q, board.in_3, board.knob_2);
	board.add_dual_control(highpass_freq, board.in_5, board.knob_3);
	board.add_dual_control(decay, board.in_6, board.knob_4);
	board.add_dual_control(accent, board.in_7, board.knob_5);
	board.add_dual_control(oscillator_tune, board.gate_2_in_8, board.knob_6);
	
	board.Init();
	board.seed.SetAudioBlockSize(blocksize); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	board.seed.StartAudio(AudioCallback);

	hihat.Init(fs);

	while(1) 
	{
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);

		hihat.set_bandpass_freq(board.get_dual_control(bandpass_freq));
		hihat.set_bandpass_res(board.get_dual_control(bandpass_q));
		hihat.set_highpass_freq(board.get_dual_control(highpass_freq));
		hihat.set_decay(board.get_dual_control(decay));
		hihat.set_global_tune(board.get_dual_control(oscillator_tune));


		switch(board.toggle_left.Read())
		{
			case Switch3::POS_UP: 
				//case 1
				break;
			case Switch3::POS_CENTER:
				//case 2
				break;
			case Switch3::POS_DOWN:
				//case 3
				break;
		}

		switch(board.toggle_right.Read())
		{
			case Switch3::POS_UP: 
				//case 1
				break;
			case Switch3::POS_CENTER:
				//case 2
				break;
			case Switch3::POS_DOWN:
				//case 3
				break;
		}

		board.button.Debounce();
		gate_in = board.get_gate_in(gate);
		trigger = gate_in & !last_gate_in;
		trigger = trigger || board.button.RisingEdge();
		last_gate_in = gate_in;

		if(trigger)
		{
			hihat.env_trigger();
		}

		board.LED_LEFT.Write(trigger);
		board.LED_RIGHT.Write(trigger);
	}
}
