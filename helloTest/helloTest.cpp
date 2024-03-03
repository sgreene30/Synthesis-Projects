#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

daisyCommon board;

Oscillator osc[2];
OnePole filter[2];
Wavefolder folder[2];

enum
{
	freq_1 = 1, fold_gain_1, filt_1, fold_off_1, freq_2, fold_gain_2, filt_2, fold_off_2
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float signal[2];

	for (size_t i = 0; i < size; i++)
	{
		signal[0] = osc[0].Process();
		signal[0] = folder[0].Process(signal[0]);
		signal[0] = filter[0].Process(signal[0]);

		signal[1] = osc[1].Process();
		signal[1] = folder[1].Process(signal[1]);
		signal[1] = filter[1].Process(signal[1]);

		out[0][i] = signal[0];
		out[1][i] = signal[1];
	}
}

int main(void)
{
	board.Init();
	board.seed.SetAudioBlockSize(4); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	board.seed.StartAudio(AudioCallback);

	//board.seed.StartLog();
	//board.seed.PrintLine("hello world");
	board.add_knob(freq_1, board.knob_2);
	board.add_knob(freq_2, board.knob_7);
	board.add_knob(fold_off_1, board.knob_4);
	board.add_knob(fold_off_2, board.knob_5);
	board.add_knob(filt_1, board.knob_1);
	board.add_knob(filt_2, board.knob_6);
	board.add_knob(fold_gain_1, board.knob_3);
	board.add_knob(fold_gain_2, board.knob_8);

	//board.print_knob();
	
	float samplerate = board.seed.AudioSampleRate();
	osc[0].Init(samplerate);
	osc[0].SetWaveform(osc[0].WAVE_POLYBLEP_TRI);
	osc[0].SetAmp(1.0f);
	osc[0].SetFreq(1000);

	osc[1].Init(samplerate);
	osc[1].SetWaveform(osc[1].WAVE_POLYBLEP_SAW);
	osc[1].SetAmp(1.0f);
	osc[1].SetFreq(1000);

	filter[0].Init();
	filter[0].SetFilterMode(filter[0].FILTER_MODE_LOW_PASS);

	filter[1].Init();
	filter[1].SetFilterMode(filter[1].FILTER_MODE_LOW_PASS);

	folder[0].Init();
	folder[1].Init();

	float osc_freq[2], filt_freq[2];
	while(1) {
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);

		filt_freq[0] = mtof(board.get_knob_value(filt_1)*127);
		filt_freq[1] = mtof(board.get_knob_value(filt_2)*127);
		filter[0].SetFrequency(filt_freq[0]);
		filter[1].SetFrequency(filt_freq[1]);

		folder[0].SetGain(board.get_knob_value(fold_gain_1)*10.0f+1.0f);
		folder[0].SetOffset(board.get_knob_value(fold_off_1)*2.0f-1.0f);
		folder[1].SetGain(board.get_knob_value(fold_gain_2)*10.0f+1.0f);
		folder[1].SetOffset(board.get_knob_value(fold_off_2)*2.0f-1.0f);

		osc_freq[0] = mtof(board.get_knob_value(freq_1)*127);
		osc[0].SetFreq(osc_freq[0]);

		osc_freq[1] = mtof(board.get_knob_value(freq_2)*127);
		osc[1].SetFreq(osc_freq[1]);	


		switch(board.toggle_left.Read())
		{
			case Switch3::POS_UP: osc[0].SetWaveform(osc[0].WAVE_POLYBLEP_TRI); break;
			case Switch3::POS_CENTER: osc[0].SetWaveform(osc[0].WAVE_POLYBLEP_SQUARE); break;
			case Switch3::POS_DOWN: osc[0].SetWaveform(osc[0].WAVE_POLYBLEP_SAW); break;
		}

		switch(board.toggle_right.Read())
		{
			case Switch3::POS_UP: osc[1].SetWaveform(osc[1].WAVE_POLYBLEP_TRI); break;
			case Switch3::POS_CENTER: osc[1].SetWaveform(osc[1].WAVE_POLYBLEP_SQUARE); break;
			case Switch3::POS_DOWN: osc[1].SetWaveform(osc[1].WAVE_POLYBLEP_SAW); break;
		}
		//board.seed.PrintLine("hello world");
		//System::Delay(1000);
	}
}

