#include "daisy_seed.h"
#include <vector>
using namespace daisy::seed;
using namespace daisy;

struct cv
{
    int user_name;
    int jack;
    int adc_channel_num;
    Pin pin;
};
struct jack_gpio
{
    int user_name;
    int jack;
    int direction;
    Pin pin;
    GPIO gpio;
};

namespace daisy
{

class daisyCommon
{
    public:
    daisyCommon() {}
    ~daisyCommon() {}

    std::vector<struct cv> cvs;
    std::vector<struct jack_gpio> jack_gpios;

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
        audio_enable = 0,
        dac_enable,
        gate_enable,
        gpio_enable,
        adc_enable
    };

    struct jack_configs
    {
        int out_1 = audio_enable;
        int out_2 = audio_enable;
        int gate_1_in_4 = adc_enable;
        int gate_2_in_8 = adc_enable;
        int in_1 = adc_enable;
        int in_2 = adc_enable;
        int in_3 = adc_enable;
        int in_5 = adc_enable;
        int in_6 = adc_enable;
        int in_7 = adc_enable;
    }jack_config;

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
    void add_cv(int user_name, int jack);
    void add_jack_gpio(int user_name, int jack, int direction);
    void add_knob(int user_name, int knob_num);
    float get_knob_value(int user_name);
    void print_knob();

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
    int get_mux(int knob_user_name);
    int get_adc_chan(int knob_user_name);
};
}