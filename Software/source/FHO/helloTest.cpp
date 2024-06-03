#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "fho.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

CpuLoadMeter loadMeter;
daisyCommon board;

int blocksize = 256;
fho osc[2];

enum
{
	freq_1 = 1, fold_gain_1, hyperbolic_1, fold_off_1, freq_2, fold_gain_2, hyperbolic_2, fold_off_2
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	//loadMeter.OnBlockStart();
	float signal[2];

	for (size_t i = 0; i < size; i++)
	{
		signal[0] = osc[0].Process();
		//signal[1] = osc[1].Process();
		
		out[0][i] = signal[0];
		out[1][i] = signal[0];
	}
	//loadMeter.OnBlockEnd();
}

int main(void)
{
	board.add_dual_control(freq_1, board.in_1, board.knob_1);
	board.add_dual_control(freq_2, board.in_5, board.knob_6);
	board.add_dual_control(hyperbolic_1, board.gate_1_in_4, board.knob_4);
	board.add_dual_control(hyperbolic_2, board.gate_2_in_8, board.knob_5);
	board.add_dual_control(fold_gain_1, board.in_3, board.knob_3);
	board.add_dual_control(fold_gain_2, board.in_7, board.knob_8);
	board.add_dual_control(fold_off_1, board.in_2, board.knob_2);
	board.add_dual_control(fold_off_2, board.in_6, board.knob_7);
	
	board.Init();
	board.seed.SetAudioBlockSize(blocksize); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	
	//loadMeter.Init(board.seed.AudioSampleRate(), board.seed.AudioBlockSize());
	board.seed.StartAudio(AudioCallback);
	

	float samplerate = board.seed.AudioSampleRate();
	//osc[0].Init(oversample*samplerate);
	//osc[1].Init(oversample*samplerate);
	osc[0].Init(samplerate);	
	osc[1].Init(samplerate);	
	

	while(1) {
		/*const float avgLoad = loadMeter.GetAvgCpuLoad();
		const float maxLoad = loadMeter.GetMaxCpuLoad();
		const float minLoad = loadMeter.GetMinCpuLoad();
		board.seed.PrintLine("Processing Load %");
		board.seed.PrintLine("Max: " FLT_FMT3, FLT_VAR3(maxLoad * 100.0f));
        board.seed.PrintLine("Avg: " FLT_FMT3, FLT_VAR3(avgLoad * 100.0f));
        board.seed.PrintLine("Min: " FLT_FMT3, FLT_VAR3(minLoad * 100.0f));*/


		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);

		osc[0].set_tanh_gain(board.get_dual_control(hyperbolic_1));
		osc[1].set_tanh_gain(board.get_dual_control(hyperbolic_2));

		osc[0].set_fold_gain(board.get_dual_control(fold_gain_1));
		osc[1].set_fold_gain(board.get_dual_control(fold_gain_2));

		osc[0].set_fold_off(board.get_dual_control(fold_off_1));
		osc[1].set_fold_off(board.get_dual_control(fold_off_2));
	
		switch(board.toggle_left.Read())
		{
			case Switch3::POS_UP: osc[0].set_freq_voct(board.get_dual_control(freq_1)); break;
			case Switch3::POS_CENTER: osc[0].set_freq_lin(board.get_dual_control(freq_1)); break;
			case Switch3::POS_DOWN: osc[0].set_freq_lfo(board.get_dual_control(freq_1)); break;
			/*case Switch3::POS_UP: osc[0].set_freq(65.41f); break;
			case Switch3::POS_CENTER: osc[0].set_freq(261.63f); break;
			case Switch3::POS_DOWN: osc[0].set_freq(2093.0f); break;*/
		}

		switch(board.toggle_right.Read())
		{
			case Switch3::POS_UP: osc[1].set_freq_voct(board.get_dual_control(freq_2)); break;
			case Switch3::POS_CENTER: osc[1].set_freq_lin(board.get_dual_control(freq_2)); break;
			case Switch3::POS_DOWN: osc[1].set_freq_lfo(board.get_dual_control(freq_2)); break;
		}
		//System::Delay(500);
		//board.seed.PrintLine("tail");
	}
}