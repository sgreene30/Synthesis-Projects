#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "envelope.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;

//DaisySeed hw;
int blocksize = 32;
float fs = 96000;
envelope env[2];

enum
{
	a1 = 1, d1, s1, r1, a2, d2, s2, r2, out_1, out_2
};

enum
{
	gate_1 = 1, gate_2
};


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float audio_signal[2];
	for (size_t i = 0; i < size; i++)
	{
		env[0].value = env[0].Process();
		audio_signal[0] = env[0].value*in[0][i];

		env[1].value = env[1].Process();
		audio_signal[1] = env[1].value*in[1][i];

		out[0][i] = 1.24f*audio_signal[0];
		out[1][i] = 1.24f*audio_signal[1];
	}
}

int main(void)
{
	board.add_knob(a1, board.knob_1);
	board.add_knob(d1, board.knob_2);
	board.add_knob(s1, board.knob_4);
	board.add_knob(r1, board.knob_3);
	board.add_knob(a2, board.knob_6);
	board.add_knob(d2, board.knob_7);
	board.add_knob(s2, board.knob_5);
	board.add_knob(r2, board.knob_8);

	board.add_cv(out_1, board.out_1);
	board.add_cv(out_2, board.out_2);
	
	board.add_gate_in(gate_1, board.in_2);
	board.add_gate_in(gate_2, board.in_6);

	env[0].Init(fs);
	env[1].Init(fs);

	board.Init();
	board.seed.SetAudioBlockSize(blocksize); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	board.seed.StartAudio(AudioCallback);

	while(1) 
	{
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);		

    	env[0].setAttack(board.get_knob(a1));
		env[0].setDecay(board.get_knob(d1));
		env[0].setRelease(board.get_knob(r1));
		env[0].setSustain(board.get_knob(s1));

		env[1].setAttack(board.get_knob(a2));
		env[1].setDecay(board.get_knob(d2));
		env[1].setRelease(board.get_knob(r2));
		env[1].setSustain(board.get_knob(s2));

		switch(board.toggle_right.Read())
		{
			case Switch3::POS_UP: 
				env[0].curve = env[0].exponential; 
				env[1].curve = env[1].exponential;
				break;
			case Switch3::POS_CENTER: 
				env[0].curve = env[0].linear; 
				env[1].curve = env[1].linear; 
				break;
			case Switch3::POS_DOWN: 
				env[0].curve = env[0].logarithmic; 
				env[1].curve = env[1].logarithmic;
				break;
		}

		switch(board.toggle_left.Read())
		{
			case Switch3::POS_UP: //vca mode
				board.audio_enable_mux(out_1);
				board.audio_enable_mux(out_2);
				break;
			case Switch3::POS_CENTER: //envelope output
				board.dac_enable_mux(out_1);
				board.dac_enable_mux(out_2);
				board.write_dac(env[0].value, out_1);
				board.write_dac(env[1].value, out_2);
				break;
			case Switch3::POS_DOWN:	//vca and envelope
				board.audio_enable_mux(out_1);
				board.dac_enable_mux(out_2);
				board.write_dac(env[0].value, out_2);
				break;
		}

		env[0].gate = board.get_gate_in(gate_1);
		env[1].gate = board.get_gate_in(gate_2);


		
	}
}
