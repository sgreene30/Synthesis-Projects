#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy::seed;
using namespace daisy;


namespace daisy
{

class daisyCommon
{
    public:
    daisyCommon() {}
    ~daisyCommon() {}

    enum jacks
    {
	    in_1 = 0, in_2, in_3, gate_1_in_4, in_5, in_6, in_7, gate_2_in_8
    };
    enum mux
    {
        mux_x = 0, mux_y
    };
    enum mux_x_channel
    {
        knob_1_chan = 0, knob_3_chan, knob_5_chan, knob_4_chan
    };
    enum mux_y_channel
    {
        knob_2_chan = 0, knob_8_chan, knob_6_chan, knob_7_chan
    };
    enum knobs
    {
        knob_1 = 0, knob_2, knob_3, knob_4, knob_5, knob_6, knob_7, knob_8
    };
    enum jack_gpio_direction
    {
        input = 0, output
    };
    enum jack_config
    {
        audio_enable = 0, dac_enable, gate_enable, gpio_enable, adc_enable, disable
    };

    struct input_configs
    {

    }cv_config;

    GPIO OUT_1_AUDIO_EN;
	GPIO OUT_2_AUDIO_EN;
	GPIO OUT_1_DAC_EN;
	GPIO OUT_2_DAC_EN;
    GPIO GATE_1_EN;
	GPIO GATE_2_EN;
	GPIO IN_4_EN;
	GPIO IN_8_EN;

    GPIO IN_1;
    GPIO IN_2;
    GPIO IN_3;
    GPIO GATE_1_IN_4;
    GPIO IN_5;
    GPIO IN_6;
    GPIO IN_7;
    GPIO GATE_2_IN_8;

    Switch3 toggle_left;
    Switch3 toggle_right;
    Switch button;
    knobs knob;

    void Init(bool boost = false);
    void add_knob(int user_name, int knob_num);
    float get_knob(int user_name);
    void print_knob();
    void add_cv(int user_name, int jack_num);
    float get_cv(int user_name);
    void add_dual_control(int user_name, int jack_num, int knob_num);
    float get_dual_control(int user_name);

    //void add_dac(int user_name, int jack);
    DaisySeed seed;

    private:
    void InitGPIO_MUX();
    void InitToggle();
    void InitADC();
    void InitButton();
    Pin getCVPin(int jack);
    Pin getGPIOPin(int jack);
    GPIO getGPIO(int jack);
    int knob_1_user_name = -1, 
        knob_2_user_name = -1,
        knob_3_user_name = -1, 
        knob_4_user_name = -1, 
        knob_5_user_name = -1, 
        knob_6_user_name = -1, 
        knob_7_user_name = -1, 
        knob_8_user_name = -1;
    int jack_1_user_name = -1,
        jack_2_user_name = -1,
        jack_3_user_name = -1,
        jack_4_user_name = -1,
        jack_5_user_name = -1,
        jack_6_user_name = -1,
        jack_7_user_name = -1,
        jack_8_user_name = -1;
    int out_1_mode = audio_enable;
    int out_2_mode = audio_enable;
    int gate_1_in_4_mode = disable;
    int gate_2_in_8_mode = disable;
    int in_1_mode = disable;
    int in_2_mode = disable;
    int in_3_mode = disable;
    int in_5_mode = disable;
    int in_6_mode = disable;
    int in_7_mode = disable;
    int in_1_adc_chan = -1,
        in_2_adc_chan = -1,
        in_3_adc_chan = -1,
        in_4_adc_chan = -1,
        in_5_adc_chan = -1,
        in_6_adc_chan = -1,
        in_7_adc_chan = -1,
        in_8_adc_chan = -1;

    int get_mux(int knob_user_name);
    int get_knob_adc_chan(int knob_user_name);
    int get_cv_adc_chan(int adc_user_name);
    int adc_channel_count = 2;

};
}