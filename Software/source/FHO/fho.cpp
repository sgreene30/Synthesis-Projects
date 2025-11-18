#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;
voice fho_1, fho_2;
float fs = 96000.0f; 
CpuLoadMeter loadMeter;

enum
{freq_1, freq_2, fine_1, fine_2, fold_1, fold_2, distort_1, distort_2};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	loadMeter.OnBlockStart();
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = fho_1.Process();
		out[1][i] = 0.0f;//fho_2.Process();
	}
	loadMeter.OnBlockEnd();
}

int main(void)
{
	board.add_dual_control(freq_1, board.in_1, board.knob_1);
	board.add_dual_control(freq_2, board.in_5, board.knob_6);
	board.add_knob(fine_1, board.knob_2);
	board.add_knob(fine_2, board.knob_7);
	board.add_dual_control(fold_1, board.in_3, board.knob_3);
	board.add_dual_control(fold_2, board.in_7, board.knob_8);
	board.add_dual_control(distort_1, board.gate_1_in_4, board.knob_4);
	board.add_dual_control(distort_2, board.gate_2_in_8, board.knob_5);
	
	fho_1.Init(fs);
	fho_2.Init(fs);

	board.Init();
	board.seed.SetAudioBlockSize(128); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	board.seed.StartAudio(AudioCallback);
	
	board.seed.StartLog();
	loadMeter.Init(board.seed.AudioSampleRate(), board.seed.AudioBlockSize());
	//board.seed.PrintLine("made it to while loop");

	
	
	while(1) 
	{
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);	
		
		fho_1.set_freq(board.get_dual_control(freq_1));
		fho_1.set_fine(board.get_knob(fine_1));
		fho_1.set_fold(board.get_dual_control(fold_1));
		fho_1.set_distort(board.get_dual_control(distort_1));
		fho_2.set_freq(board.get_dual_control(freq_2));
		fho_2.set_fine(board.get_knob(fine_2));
		fho_2.set_fold(board.get_dual_control(fold_2));
		fho_2.set_distort(board.get_dual_control(distort_2));


		const float avgLoad = loadMeter.GetAvgCpuLoad();
        const float maxLoad = loadMeter.GetMaxCpuLoad();
        const float minLoad = loadMeter.GetMinCpuLoad();
		board.seed.PrintLine("Processing Load %:");
        board.seed.PrintLine("Max: " FLT_FMT3, FLT_VAR3(maxLoad * 100.0f));
        board.seed.PrintLine("Avg: " FLT_FMT3, FLT_VAR3(avgLoad * 100.0f));
        board.seed.PrintLine("Min: " FLT_FMT3, FLT_VAR3(minLoad * 100.0f));
		System::Delay(500);

	}
}
