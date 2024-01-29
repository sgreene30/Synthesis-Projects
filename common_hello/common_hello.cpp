#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

DaisySeed hw;

enum adc_channel 
{
	mux_x = 0, mux_y, in_1,	in_2, in_3, in_4, in_5, in_6, in_7, in_8, NUM_ADC_CHANNELS
};
enum mux_x_channel
{
	knob_1 = 0, knob_3, knob_5, knob_4
};
enum mux_y_channel
{
	knob_2 = 0, knob_8, knob_6, knob_7
};
Switch3 toggle[2];

AdcChannelConfig adc_config[NUM_ADC_CHANNELS];
Oscillator osc[2];

float get_cv(int mux_chan, int knob, int cv_in_chan)
{
	float cv;
	cv = (hw.adc.GetMuxFloat(mux_chan, knob)+hw.adc.GetFloat(cv_in_chan))*(-1.0f)+1.5f;
	if(cv > 1.0f)
	{
		cv = 1.0f;
	}
	else if(cv < 0.0f)
	{
		cv = 0.0f;
	}
	cv = mtof(cv*127);
	return cv;
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	float osc_out[2], osc_freq[2];

	osc_freq[0] = mtof((hw.adc.GetMuxFloat(mux_x, knob_1)*(-1.0f)+1)*127);
	osc[0].SetFreq(osc_freq[0]);

	osc_freq[1] = get_cv(mux_x, knob_6, in_5);
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
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	bool high = true;
	bool low = false;

	float samplerate = hw.AudioSampleRate();

	//audio/DAC MUX
	GPIO OUT_1_AUDIO_EN;
	GPIO OUT_2_AUDIO_EN;
	GPIO OUT_1_DAC_EN;
	GPIO OUT_2_DAC_EN;

	OUT_1_AUDIO_EN.Init(D3, GPIO::Mode::OUTPUT);
	OUT_2_AUDIO_EN.Init(D5, GPIO::Mode::OUTPUT);
	OUT_1_DAC_EN.Init(D2, GPIO::Mode::OUTPUT);
	OUT_2_DAC_EN.Init(D4, GPIO::Mode::OUTPUT);

	OUT_1_AUDIO_EN.Write(high);
	OUT_2_AUDIO_EN.Write(high);
	OUT_1_DAC_EN.Write(low);
	OUT_2_DAC_EN.Write(low);

	//GATE CV MUX
	GPIO GATE_1_EN;
	GPIO GATE_2_EN;
	GPIO IN_4_EN;
	GPIO IN_8_EN;

	GATE_1_EN.Init(D27, GPIO::Mode::OUTPUT);
	GATE_2_EN.Init(D30, GPIO::Mode::OUTPUT);
	IN_4_EN.Init(D26, GPIO::Mode::OUTPUT);
	IN_8_EN.Init(D29, GPIO::Mode::OUTPUT);

	GATE_1_EN.Write(low);
	GATE_2_EN.Write(low);
	IN_4_EN.Write(high);	
	IN_8_EN.Write(high);
	
	adc_config[mux_x].InitMux(A11, 4, D6, D7);
	adc_config[mux_y].InitMux(A10, 4, D6, D7);
	adc_config[in_1].InitSingle(A0);
	adc_config[in_2].InitSingle(A4);
	adc_config[in_3].InitSingle(A9);
	adc_config[in_4].InitSingle(A6);
	adc_config[in_5].InitSingle(A1);
	adc_config[in_6].InitSingle(A5);
	adc_config[in_7].InitSingle(A2);
	adc_config[in_8].InitSingle(A3);

	toggle[0].Init(D9, D8);
	toggle[1].Init(D14, D13);

	hw.adc.Init(adc_config, NUM_ADC_CHANNELS);
	hw.adc.Start();

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
		hw.SetLed(slow_blink);

		switch(toggle[0].Read())
		{
			case Switch3::POS_UP: osc[0].SetWaveform(osc[0].WAVE_POLYBLEP_TRI); break;
			case Switch3::POS_CENTER: osc[0].SetWaveform(osc[0].WAVE_POLYBLEP_SQUARE); break;
			case Switch3::POS_DOWN: osc[0].SetWaveform(osc[0].WAVE_POLYBLEP_SAW); break;
		}

		switch(toggle[1].Read())
		{
			case Switch3::POS_UP: osc[1].SetWaveform(osc[1].WAVE_POLYBLEP_TRI); break;
			case Switch3::POS_CENTER: osc[1].SetWaveform(osc[1].WAVE_POLYBLEP_SQUARE); break;
			case Switch3::POS_DOWN: osc[1].SetWaveform(osc[1].WAVE_POLYBLEP_SAW); break;
		}

	}
}

