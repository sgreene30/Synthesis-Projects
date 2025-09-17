#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;
int blocksize = 32;
float fs = 96000;
voice filter;
bool button_trigger = false;

enum 
{
	cutoff, res, env_amount, attack, decay, sustain, release, gate, env_out, noise_mix
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = filter.Process(in[0][i]);
		//out[1][i] = filter.Process(in[1][i]);
	}
}

int main(void)

{
	board.add_dual_control(cutoff, board.in_2, board.knob_1);
	board.add_dual_control(res, board.in_3, board.knob_2);
	board.add_dual_control(env_amount, board.gate_1_in_4, board.knob_4);
	board.add_dual_control(noise_mix, board.in_6, board.knob_3);

	board.add_knob(attack, board.knob_6);
	board.add_knob(decay, board.knob_7);
	board.add_knob(sustain, board.knob_5);
	board.add_knob(release, board.knob_8);

	board.add_gate_in(gate, board.in_5);

	board.add_cv(env_out, board.out_2);
	
	board.Init();
	board.seed.SetAudioBlockSize(4); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	board.seed.StartAudio(AudioCallback);

	board.dac_enable_mux(board.out_2);

	filter.Init(fs);
	while(1) 
	{
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);	
		filter.set_cutoff(board.get_dual_control(cutoff));
		filter.set_res(board.get_dual_control(res));
		filter.set_env_amount(board.get_dual_control(env_amount));
		filter.set_attack(board.get_knob(attack));
		filter.set_decay(board.get_knob(decay));
		filter.set_sustain(board.get_knob(sustain));
		filter.set_release(board.get_knob(release));
		filter.set_noise_level(board.get_dual_control(noise_mix));

		switch(board.toggle_left.Read())
		{
			case Switch3::POS_UP:
				filter.set_passband(filter.LOWPASS);
				break;
			case Switch3::POS_CENTER:
				filter.set_passband(filter.BANDPASS);
				break;
			case Switch3::POS_DOWN:
				filter.set_passband(filter.HIGHPASS);
				break;
		}

		switch(board.toggle_right.Read())
		{
			case Switch3::POS_UP:
				filter.set_noise_mode(filter.BLUE);
				break;
			case Switch3::POS_CENTER:
				filter.set_noise_mode(filter.WHITE);
				break;
			case Switch3::POS_DOWN:
				filter.set_noise_mode(filter.PINK);
				break;
		}

		board.button.Debounce();
		button_trigger = board.button.RisingEdge() ? true : false;
		if(button_trigger) 
		{
			filter.enable_4pole(!filter.get_pole());
			board.LED_RIGHT.Write(filter.get_pole());
		}

		//search for gate input
		filter.set_gate(board.get_gate_in(gate));
		board.LED_LEFT.Write(filter.get_gate());

		board.write_dac(filter.get_env_value(), env_out);
	}
}
