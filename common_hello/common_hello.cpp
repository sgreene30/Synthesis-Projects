#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

daisyCommon board;


Oscillator osc[2];
//LadderFilter filter[2];

enum
{
	freq_1 = 1, fold_1, filt_1, amp_1, freq_2, fold_2, filt_2, amp_2
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float osc_out[2], osc_freq[2];//, osc_amp[2], filt_freq[2], amp[2];


	osc_freq[0] = mtof(board.get_knob_value(freq_1)*127);
	osc[0].SetFreq(osc_freq[0]);

	osc_freq[1] = mtof(board.get_knob_value(freq_2)*127);
	osc[1].SetFreq(osc_freq[1]);	
	
	for (size_t i = 0; i < size; i++)
	{
		osc_out[0] = osc[0].Process();
		osc_out[1] = osc[1].Process();
		out[0][i] = osc_out[0];
		out[1][i] = osc_out[1];
	}
}

int main(void)
{
	board.add_knob(freq_1, board.knob_1);
	board.add_knob(freq_2, board.knob_6);
	board.add_knob(amp_1, board.knob_4);
	board.add_knob(amp_2, board.knob_5);
	board.add_knob(filt_1, board.knob_2);
	board.add_knob(filt_2, board.knob_7);
	board.add_knob(fold_1, board.knob_3);
	board.add_knob(fold_2, board.knob_8);

	board.Init();
	board.seed.SetAudioBlockSize(4); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	board.seed.StartAudio(AudioCallback);
	
	float samplerate = board.seed.AudioSampleRate();
	osc[0].Init(samplerate);
	osc[0].SetWaveform(osc[0].WAVE_POLYBLEP_TRI);
	osc[0].SetAmp(1.0f);
	osc[0].SetFreq(1000);

	osc[1].Init(samplerate);
	osc[1].SetWaveform(osc[1].WAVE_POLYBLEP_SAW);
	osc[1].SetAmp(1.0f);
	osc[1].SetFreq(1000);

	while(1) {
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);

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

	}
}

