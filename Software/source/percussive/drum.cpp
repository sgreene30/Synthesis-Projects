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
	freq, amp_decay, freq_decay, gate, impact, effect_value, noise_mix, noise_effect_1, noise_effect_2
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
	board.add_dual_control(impact, board.in_3, board.knob_2);
	board.add_dual_control(effect_value, board.gate_1_in_4, board.knob_3);
	board.add_knob(freq_decay, board.knob_5);
	board.add_dual_control(amp_decay, board.in_5, board.knob_4);
	board.add_dual_control(noise_effect_1, board.in_6, board.knob_6);
	board.add_dual_control(noise_effect_2, board.in_7, board.knob_7);
	board.add_dual_control(noise_mix, board.gate_2_in_8, board.knob_8);

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
		osc.set_freq_env_decay(board.get_knob(freq_decay));
		osc.set_amp_env_decay(board.get_dual_control(amp_decay));
		osc.set_impact(board.get_dual_control(impact));
		osc.set_effect_value(board.get_dual_control(effect_value));
		osc.set_noise_mix(board.get_dual_control(noise_mix));
		osc.set_noise_effect_1(board.get_dual_control(noise_effect_1));
		osc.set_noise_effect_2(board.get_dual_control(noise_effect_2));

		switch(board.toggle_left.Read())
		{
			case Switch3::POS_UP:
				osc.set_effect_mode(osc.DISTORT);
				break;
			case Switch3::POS_CENTER:
				osc.set_effect_mode(osc.FOLD);
				break;
			case Switch3::POS_DOWN:
				osc.set_effect_mode(osc.DISTORT);
				break;
		}

		switch(board.toggle_right.Read())
		{
			case Switch3::POS_UP: 
				//osc.set_wave_shape(osc.SIN);
				osc.set_noise_mode(osc.NOISE);
				break;
			case Switch3::POS_CENTER:
				osc.set_noise_mode(osc.GONG);
				//osc.set_wave_shape(osc.TRI);
				break;
			case Switch3::POS_DOWN:
				osc.set_noise_mode(osc.FM);
				//osc.set_wave_shape(osc.SAW);
				break;
		}

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
