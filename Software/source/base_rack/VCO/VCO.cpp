#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;
int blocksize = 32;
float fs = 96000;
voice osc_1, osc_2;
bool mix = false; 

enum
{
	freq_1, freq_2, fine_1, fine_2, shape_1, shape_2, morph_1, morph_2
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	if(!mix)
	{
		for (size_t i = 0; i < size; i++)
		{
			out[0][i] = osc_1.Process();
			out[1][i] = osc_2.Process();
		}
	}
	else
	{
		for (size_t i = 0; i < size; i++)
		{
			out[0][i] = osc_1.Process()/2.0f + osc_2.Process()/2.0f;
			out[1][i] = osc_2.Process();
		}
	}

}

int main(void)
{
	board.add_dual_control(freq_1, board.in_1, board.knob_1);
	board.add_dual_control(freq_2, board.in_5, board.knob_6);
	board.add_dual_control(fine_1, board.in_2, board.knob_2);
	board.add_dual_control(fine_2, board.in_6, board.knob_7);
	board.add_dual_control(shape_1, board.in_3, board.knob_3);
	board.add_dual_control(shape_2, board.in_7, board.knob_8);
	board.add_dual_control(morph_1, board.gate_1_in_4, board.knob_4);
	board.add_dual_control(morph_2, board.gate_2_in_8, board.knob_5);


	board.Init();
	board.seed.SetAudioBlockSize(4); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	board.seed.StartAudio(AudioCallback);

	//board.seed.StartLog(true);
	//board.seed.PrintLine("Hello World");

	osc_1.Init(fs);
	osc_2.Init(fs);

	while(1) 
	{
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);
		
		if(mix)
		{
			osc_1.set_freq(board.get_dual_control(freq_1));
			osc_2.set_freq(board.get_dual_control(freq_1));
		}
		else
		{
			osc_1.set_freq(board.get_dual_control(freq_1));
			osc_2.set_freq(board.get_dual_control(freq_2));
		}
		
		
		osc_1.set_fine(board.get_dual_control(fine_1));
		osc_2.set_fine(board.get_dual_control(fine_2));
		osc_1.set_shape(board.get_dual_control(shape_1));
		osc_2.set_shape(board.get_dual_control(shape_2));
		


		/*board.seed.DelayMs(200);
		board.seed.PrintLine("shape: %f", board.get_dual_control(shape_1));
		board.seed.PrintLine("sin: %f", osc_1.get_sin_amp());
		board.seed.PrintLine("tri: %f", osc_1.get_tri_amp());
		board.seed.PrintLine("square: %f", osc_1.get_square_amp());
		board.seed.PrintLine("saw: %f", osc_1.get_saw_amp());
		board.seed.PrintLine(" ");*/

		switch(board.toggle_left.Read())
		{
			case Switch3::POS_UP:
				osc_1.set_freq_curve(osc_1.voct);
				break;
			case Switch3::POS_CENTER:
				osc_1.set_freq_curve(osc_1.linear);
				break;
			case Switch3::POS_DOWN:
				osc_1.set_freq_curve(osc_1.lfo);
				break;
		}

		switch(board.toggle_right.Read())
		{
			case Switch3::POS_UP:
				osc_2.set_freq_curve(osc_2.voct);
				break;
			case Switch3::POS_CENTER:
				osc_2.set_freq_curve(osc_2.linear);
				break;
			case Switch3::POS_DOWN:
				osc_2.set_freq_curve(osc_2.lfo);
				break;
		}

		board.button.Debounce();

		if(board.button.RisingEdge())
		{
			mix = !mix;
		}
		board.LED_LEFT.Write(!mix);
		board.LED_RIGHT.Write(mix);
	}
}
