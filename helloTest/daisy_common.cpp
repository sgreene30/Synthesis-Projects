#include "daisy_common.h"


using namespace daisy;
using namespace daisy::seed;
using namespace std;

constexpr Pin TOGGLE_LEFT_PIN_1 = seed::D9;
constexpr Pin TOGGLE_LEFT_PIN_2 = seed::D8;
constexpr Pin TOGGLE_RIGHT_PIN_1 = seed::D14;
constexpr Pin TOGGLE_RIGHT_PIN_2 = seed::D13;
constexpr Pin MUX_X_ADC_PIN = seed::A11;
constexpr Pin MUX_Y_ADC_PIN = seed::A10;
constexpr Pin IN_1_ADC_PIN = seed::A0;
constexpr Pin IN_2_ADC_PIN = seed::A4;
constexpr Pin IN_3_ADC_PIN = seed::A9;
constexpr Pin IN_4_ADC_PIN = seed::A6;
constexpr Pin IN_5_ADC_PIN = seed::A1;
constexpr Pin IN_6_ADC_PIN = seed::A5;
constexpr Pin IN_7_ADC_PIN = seed::A2;
constexpr Pin IN_8_ADC_PIN = seed::A8;
constexpr Pin GATE_1_IN_4_PIN = seed::D21;
constexpr Pin GATE_2_IN_8_PIN = seed::D23;
constexpr Pin IN_1_GPIO_PIN = seed::D15;
constexpr Pin IN_2_GPIO_PIN = seed::D19;
constexpr Pin IN_3_GPIO_PIN = seed::D24;
constexpr Pin IN_5_GPIO_PIN = seed::D16;
constexpr Pin IN_6_GPIO_PIN = seed::D20;
constexpr Pin IN_7_GPIO_PIN = seed::D17;

void daisyCommon::add_dual_control(int user_name, int jack_num, int knob_num)
{
    add_cv(user_name, jack_num);
    add_knob(user_name, knob_num);
}
float daisyCommon::get_dual_control(int user_name)
{
    float ret;
    //seed.PrintLine("cv: " FLT_FMT3, FLT_VAR3(get_cv(user_name)));
    //seed.PrintLine("knob: " FLT_FMT3, FLT_VAR3(get_knob(user_name)));
    ret = get_cv(user_name)-0.5f;
    ret = ret + get_knob(user_name);
    //seed.PrintLine("ret: " FLT_FMT3, FLT_VAR3(ret));
    if(ret > 1.0f)
    {
        ret = 1.0f;
    }
    else if(ret < 0.0f)
    {
        ret = 0.0f;
    }
    return ret;
}


float daisyCommon::get_cv(int user_name)
{
    return seed.adc.GetFloat(get_cv_adc_chan(user_name))*-1+1;
}

int daisyCommon::get_cv_adc_chan(int adc_user_name)
{
    if(adc_user_name == jack_1_user_name) return in_1_adc_chan;
    if(adc_user_name == jack_2_user_name) return in_2_adc_chan;
    if(adc_user_name == jack_3_user_name) return in_3_adc_chan;
    if(adc_user_name == jack_4_user_name) return in_4_adc_chan;
    if(adc_user_name == jack_5_user_name) return in_5_adc_chan;
    if(adc_user_name == jack_6_user_name) return in_6_adc_chan;
    if(adc_user_name == jack_7_user_name) return in_7_adc_chan;
    if(adc_user_name == jack_8_user_name) return in_8_adc_chan;
    return -1;
}

void daisyCommon::add_cv(int user_name, int jack_num)
{
    if(jack_num == in_1)
    {
        jack_1_user_name = user_name;
        in_1_adc_chan = adc_channel_count;
        in_1_mode = adc_enable;
    } 
    if(jack_num == in_2)
    {
        jack_2_user_name = user_name;
        in_2_adc_chan = adc_channel_count;
        in_2_mode = adc_enable; 
    } 
    if(jack_num == in_3)
    {
        jack_3_user_name = user_name;
        in_3_adc_chan = adc_channel_count;
        in_3_mode = adc_enable;
    } 
    if(jack_num == gate_1_in_4)
    {
        jack_4_user_name = user_name;
        in_4_adc_chan = adc_channel_count;
        gate_1_in_4_mode = adc_enable;
    } 
    if(jack_num == in_5)
    {
        jack_5_user_name = user_name;
        in_5_adc_chan = adc_channel_count;
        in_5_mode = adc_enable;
    } 
    if(jack_num == in_6)
    {
        jack_6_user_name = user_name;
        in_6_adc_chan = adc_channel_count;
        in_6_mode = adc_enable;
    } 
    if(jack_num == in_7)
    {
        jack_7_user_name = user_name;
        in_7_adc_chan = adc_channel_count;
        in_7_mode = adc_enable;
    } 
    if(jack_num == gate_2_in_8)
    {
        jack_8_user_name = user_name;
        in_8_adc_chan = adc_channel_count;
        gate_2_in_8_mode = adc_enable;
    } 
    adc_channel_count++;
}

void daisyCommon::print_knob()
{
    seed.PrintLine("knob_1_user: %d", knob_1_user_name);
    seed.PrintLine("knob_2_user: %d", knob_2_user_name);
    seed.PrintLine("knob_3_user: %d", knob_3_user_name);
    seed.PrintLine("knob_4_user: %d", knob_4_user_name);
    seed.PrintLine("knob_5_user: %d", knob_5_user_name);
    seed.PrintLine("knob_6_user: %d", knob_6_user_name);
    seed.PrintLine("knob_7_user: %d", knob_7_user_name);
    seed.PrintLine("knob_8_user: %d", knob_8_user_name);
}

float daisyCommon::get_knob(int user_name)
{
    return seed.adc.GetMuxFloat(get_mux(user_name), get_knob_adc_chan(user_name))*-1+1;
}
void daisyCommon::add_knob(int user_name, int knob_num)
{
    if(knob_num == knob_1) knob_1_user_name = user_name;
    if(knob_num == knob_2) knob_2_user_name = user_name;
    if(knob_num == knob_3) knob_3_user_name = user_name;
    if(knob_num == knob_4) knob_4_user_name = user_name;
    if(knob_num == knob_5) knob_5_user_name = user_name;
    if(knob_num == knob_6) knob_6_user_name = user_name;
    if(knob_num == knob_7) knob_7_user_name = user_name;
    if(knob_num == knob_8) knob_8_user_name = user_name;
}
int daisyCommon::get_mux(int knob_user_name)
{
    if(knob_user_name == knob_1_user_name) return mux_x;
    if(knob_user_name == knob_3_user_name) return mux_x;
    if(knob_user_name == knob_4_user_name) return mux_x;
    if(knob_user_name == knob_5_user_name) return mux_x;
    if(knob_user_name == knob_2_user_name) return mux_y;
    if(knob_user_name == knob_6_user_name) return mux_y;
    if(knob_user_name == knob_7_user_name) return mux_y;
    if(knob_user_name == knob_8_user_name) return mux_y;
    return -1;
}
int daisyCommon::get_knob_adc_chan(int knob_user_name)
{
    if(knob_user_name == knob_1_user_name) return knob_1_chan;
    if(knob_user_name == knob_3_user_name) return knob_3_chan;
    if(knob_user_name == knob_4_user_name) return knob_4_chan;
    if(knob_user_name == knob_5_user_name) return knob_5_chan;
    if(knob_user_name == knob_2_user_name) return knob_2_chan;
    if(knob_user_name == knob_6_user_name) return knob_6_chan;
    if(knob_user_name == knob_7_user_name) return knob_7_chan;
    if(knob_user_name == knob_8_user_name) return knob_8_chan;
    return -1;
}

void daisyCommon::Init(bool boost)
{
    //seed.Configure();
    seed.Init(boost);
    //seed.StartLog(true);
    InitGPIO_MUX();
    InitToggle();
    InitButton();
    InitADC();
    seed.SetLed(false);
}

void daisyCommon::InitGPIO_MUX()
{
    OUT_1_AUDIO_EN.Init(D3, GPIO::Mode::OUTPUT);
	OUT_2_AUDIO_EN.Init(D5, GPIO::Mode::OUTPUT);
	OUT_1_DAC_EN.Init(D2, GPIO::Mode::OUTPUT);
	OUT_2_DAC_EN.Init(D4, GPIO::Mode::OUTPUT);
    
    GATE_1_EN.Init(D27, GPIO::Mode::OUTPUT);
	GATE_2_EN.Init(D30, GPIO::Mode::OUTPUT);
	IN_4_EN.Init(D26, GPIO::Mode::OUTPUT);
	IN_8_EN.Init(D29, GPIO::Mode::OUTPUT);

    if(out_1_mode == audio_enable)
    {
        OUT_1_AUDIO_EN.Write(true);
        OUT_1_DAC_EN.Write(false);
    } 
    else if(out_1_mode == dac_enable)
    {
        OUT_1_AUDIO_EN.Write(false);
        OUT_1_DAC_EN.Write(true);
    }

    if(out_2_mode == audio_enable)
    {
        OUT_2_AUDIO_EN.Write(true);
        OUT_2_DAC_EN.Write(false);
    } 
    else if(out_2_mode == dac_enable)
    {
        OUT_2_AUDIO_EN.Write(false);
        OUT_2_DAC_EN.Write(true);
    }

    if((gate_1_in_4_mode == adc_enable) || (gate_1_in_4_mode == gpio_enable))
    {
        IN_4_EN.Write(true);
        GATE_1_EN.Write(false);
    }
    else if(gate_1_in_4_mode == gate_enable)
    {
        IN_4_EN.Write(false);
        GATE_1_EN.Write(true);       
    }

    if((gate_2_in_8_mode == adc_enable) || (gate_2_in_8_mode == gpio_enable))
    {
        IN_8_EN.Write(true);
        GATE_2_EN.Write(false);
    }
    else if(gate_2_in_8_mode == gate_enable)
    {
        IN_8_EN.Write(false);
        GATE_2_EN.Write(true);       
    }
}

void daisyCommon::InitToggle()
{
    toggle_left.Init(TOGGLE_LEFT_PIN_1, TOGGLE_LEFT_PIN_2);
	toggle_right.Init(TOGGLE_RIGHT_PIN_1, TOGGLE_RIGHT_PIN_2);
}

void daisyCommon::InitButton()
{
    button.Init(D10);
}

void daisyCommon::InitADC()
{
    AdcChannelConfig adc_config[adc_channel_count];
    adc_config[mux_x].InitMux(MUX_X_ADC_PIN, 4, D6, D7);
	adc_config[mux_y].InitMux(MUX_Y_ADC_PIN, 4, D6, D7);
    if(in_1_mode == adc_enable)
    {
        adc_config[in_1_adc_chan].InitSingle(IN_1_ADC_PIN);
    }
    if(in_2_mode == adc_enable)
    {
        adc_config[in_2_adc_chan].InitSingle(IN_2_ADC_PIN);
    }
    if(in_3_mode == adc_enable)
    {
        adc_config[in_3_adc_chan].InitSingle(IN_3_ADC_PIN);
    }
    if(gate_1_in_4_mode == adc_enable)
    {
        adc_config[in_4_adc_chan].InitSingle(IN_4_ADC_PIN);
    }
    if(in_5_mode == adc_enable)
    {
        adc_config[in_5_adc_chan].InitSingle(IN_5_ADC_PIN);
    }
    if(in_6_mode == adc_enable)
    {
        adc_config[in_6_adc_chan].InitSingle(IN_6_ADC_PIN);
    }
    if(in_7_mode == adc_enable)
    {
        adc_config[in_7_adc_chan].InitSingle(IN_7_ADC_PIN);
    }
    if(gate_2_in_8_mode == adc_enable)
    {
        adc_config[in_8_adc_chan].InitSingle(IN_8_ADC_PIN);
    }
    seed.adc.Init(adc_config, adc_channel_count);
    seed.adc.Start();
}