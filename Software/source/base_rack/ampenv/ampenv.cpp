#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;
int blocksize = 32;
float fs = 96000;

voice ampenv;

enum
{
	volume, env_amount, attack, decay, sustain, release, gate, env_out, lpf_freq, lpf_amount
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = ampenv.Process(in[0][i]);
		//out[1][i] = in[1][i];
	}
}

int main(void)
{
	board.add_dual_control(volume, board.in_2, board.knob_1);
	board.add_dual_control(env_amount, board.in_3, board.knob_2);
	board.add_knob(attack, board.knob_6);
	board.add_knob(decay, board.knob_7);
	board.add_knob(sustain, board.knob_5);
	board.add_knob(release, board.knob_8);

	board.add_gate_in(gate, board.in_5);
	board.add_cv(env_out, board.out_2);

	board.Init();
	board.seed.SetAudioBlockSize(blocksize); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	board.seed.StartAudio(AudioCallback);
	
	while(1) 
	{	
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);	

		ampenv.set_volume(board.get_dual_control(volume));
		ampenv.set_env_amount(board.get_dual_control(env_amount));
		ampenv.set_attack(board.get_knob(attack));
		ampenv.set_decay(board.get_knob(decay));
		ampenv.set_sustain(board.get_knob(sustain));
		ampenv.set_release(board.get_knob(release));

		//search for gate input
		ampenv.set_gate(board.get_gate_in(gate));
		board.LED_LEFT.Write(ampenv.get_gate());

		board.write_dac(ampenv.get_env_value(), env_out);

	}
}
