#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_common.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

daisyCommon board;
int blocksize = 128;
float fs = 96000;
voice instrument;
bool trigger = false;
bool last_gate_in = false;
bool gate_in = false;

enum
{
	osc_freq, osc_spread, osc_sideband, fx, gate, filter_freq, filter_res, attack, decay
};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		//out[1][i] = osc_test.Process();
		out[0][i] = instrument.Process();
	}
}

int main(void)
{
	board.add_dual_control(osc_freq, board.in_2, board.knob_1);
	board.add_dual_control(osc_spread, board.in_3, board.knob_2);
	board.add_dual_control(osc_sideband, board.gate_1_in_4, board.knob_3);
	board.add_knob(attack, board.knob_4);
	board.add_dual_control(decay, board.in_5, board.knob_5);
	board.add_dual_control(filter_freq, board.in_6, board.knob_6);
	board.add_dual_control(filter_res, board.in_7, board.knob_7);
	board.add_dual_control(fx, board.gate_2_in_8, board.knob_8);

	board.add_gate_in(gate, board.in_1);

	instrument.Init(fs);

	board.Init();

	//board.seed.StartLog();
	//board.seed.PrintLine("start of log");

	board.seed.SetAudioBlockSize(4); // number of samples handled per callback
	board.seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	board.seed.StartAudio(AudioCallback);
	while(1) 
	{
		bool slow_blink = (System::GetNow() & 1023) < 511;
		board.seed.SetLed(slow_blink);
		
		instrument.set_osc_freq(board.get_dual_control(osc_freq));
		instrument.set_osc_spread(board.get_dual_control(osc_spread));
		instrument.set_osc_sideband(board.get_dual_control(osc_sideband));
		instrument.set_fx(board.get_dual_control(fx));
		instrument.set_filter_freq(board.get_dual_control(filter_freq));
		instrument.set_filter_res(board.get_dual_control(filter_res));
		instrument.set_attack(board.get_knob(attack));
		instrument.set_decay(board.get_dual_control(decay));

		switch(board.toggle_left.Read())
		{
			case Switch3::POS_UP:
				instrument.set_waveshape(instrument.Waveshape::TRIANGLE);
				break;
			case Switch3::POS_CENTER:
				instrument.set_waveshape(instrument.Waveshape::SQUARE);
				break;
			case Switch3::POS_DOWN:
				instrument.set_waveshape(instrument.Waveshape::SAW);
				break;
		}

		board.button.Debounce();
		gate_in = board.get_gate_in(gate);
		trigger = gate_in & !last_gate_in;
		trigger = trigger || board.button.RisingEdge();
		last_gate_in = gate_in;

		if(trigger)
		{
			instrument.env_trigger();
		}

		board.LED_LEFT.Write(trigger);
		board.LED_RIGHT.Write(trigger);

	}
}
