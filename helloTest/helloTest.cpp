#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

daisyCommon board;

Oscillator osc[2];
Overdrive drive[2];
Wavefolder folder[2];

enum
{
	freq_1 = 1, fold_gain_1, overdrive_1, fold_off_1, freq_2, fold_gain_2, overdrive_2, fold_off_2
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float signal[2];

	for (size_t i = 0; i < size; i++)
	{
		signal[0] = osc[0].Process();
		signal[0] = folder[0].Process(signal[0]);
		signal[0] = drive[0].Process(signal[0]);

		signal[1] = osc[1].Process();
		signal[1] = folder[1].Process(signal[1]);
		signal[1] = drive[1].Process(signal[1]);

		out[0][i] = signal[0];
		out[1][i] = signal[1];
	}
}

int main(void)
{
	board.add_dual_control(freq_1, board.in_1, board.knob_1);
	board.add_dual_control(freq_2, board.in_5, board.knob_6);
	board.add_dual_control(overdrive_1, board.gate_1_in_4, board.knob_4);
	board.add_dual_control(overdrive_2, board.gate_2_in_8, board.knob_5);
	board.add_dual_control(fold_gain_1, board.in_3, board.knob_3);
	board.add_dual_control(fold_gain_2, board.in_7, board.knob_8);
	board.add_dual_control(fold_off_1, board.in_2, board.knob_2);
	board.add_dual_control(fold_off_2, board.in_6, board.knob_7);
	
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

	drive[0].Init();
	drive[1].Init();

	folder[0].Init();
	folder[1].Init();

	float osc_freq[2];
	while(1) {
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);

		drive[0].SetDrive(board.get_dual_control(overdrive_1)+1.0f);
		drive[1].SetDrive(board.get_dual_control(overdrive_2)+1.0f);

		folder[0].SetGain(board.get_dual_control(fold_gain_1)*10.0f+1.0f);
		folder[0].SetOffset(board.get_dual_control(fold_off_1)*2.0f-1.0f);
		folder[1].SetGain(board.get_dual_control(fold_gain_2)*10.0f+1.0f);
		folder[1].SetOffset(board.get_dual_control(fold_off_2)*2.0f-1.0f);

		osc_freq[0] = mtof(board.get_dual_control(freq_1)*127);
		osc[0].SetFreq(osc_freq[0]);

		osc_freq[1] = mtof(board.get_dual_control(freq_2)*127);
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
		//System::Delay(1000);
		//board.seed.PrintLine("tail");
	}
}

