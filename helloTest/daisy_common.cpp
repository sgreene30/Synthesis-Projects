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

float daisyCommon::get_knob_value(int user_name)
{
    return seed.adc.GetMuxFloat(get_mux(user_name), get_adc_chan(user_name));
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
int daisyCommon::get_adc_chan(int knob_user_name)
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
    InitGPIO_MUX();
    InitToggle();
    InitButton();
    InitADC();
    
}

void daisyCommon::add_cv(int user_name, int jack)
{
    struct cv ctrl;
    ctrl.user_name = user_name;
    ctrl.jack = jack;
    ctrl.adc_channel_num = cvs.size();
    ctrl.pin = getCVPin(jack);
    cvs.push_back(ctrl);
}

Pin daisyCommon::getCVPin(int jack)
{
    Pin pin;
    switch(jack)
    {
        case in_1: pin = IN_1_ADC_PIN;
        case in_2: pin = IN_2_ADC_PIN;
        case in_3: pin = IN_3_ADC_PIN;
        case gate_1_in_4: pin = GATE_1_IN_4_PIN;
        case in_5: pin = IN_5_GPIO_PIN;
        case in_6: pin = IN_6_GPIO_PIN;
        case in_7: pin = IN_7_GPIO_PIN;
        case gate_2_in_8: pin = GATE_2_IN_8_PIN;
        default: break;
    }
    return pin;
}

void daisyCommon::add_jack_gpio(int user_name, int jack, int direction)
{
    struct jack_gpio gpio;
    gpio.jack = jack;
    gpio.user_name = user_name;
    gpio.direction = direction;
    gpio.pin = getGPIOPin(jack);
    gpio.gpio = getGPIO(jack);
    if(direction == output)
    {
        gpio.gpio.Init(gpio.pin, GPIO::Mode::OUTPUT);
    }
    else
    {
        gpio.gpio.Init(gpio.pin, GPIO::Mode::INPUT);
    }
    
}

GPIO daisyCommon::getGPIO(int jack)
{
    switch(jack)
    {
        case in_1: return IN_1;
        case in_2: return IN_2;
        case in_3: return IN_3;
        case gate_1_in_4: return GATE_1_IN_4;
        case in_5: return IN_5;
        case in_6: return IN_6;
        case in_7: return IN_7;
        case gate_2_in_8: return GATE_2_IN_8;
        default: break;
    }
    return IN_1;
}

Pin daisyCommon::getGPIOPin(int jack)
{
    Pin pin;
    switch(jack)
    {
        case in_1: pin = IN_1_GPIO_PIN;
        case in_2: pin = IN_2_GPIO_PIN;
        case in_3: pin = IN_3_GPIO_PIN;
        case gate_1_in_4: pin = IN_4_ADC_PIN;
        case in_5: pin = IN_5_ADC_PIN;
        case in_6: pin = IN_6_ADC_PIN;
        case in_7: pin = IN_7_ADC_PIN;
        case gate_2_in_8: pin = IN_8_ADC_PIN;
        default: break;
    }
    return pin;
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

    if(jack_config.out_1 == audio_enable)
    {
        OUT_1_AUDIO_EN.Write(true);
        OUT_1_DAC_EN.Write(false);
    } 
    else if(jack_config.out_1 == dac_enable)
    {
        OUT_1_AUDIO_EN.Write(false);
        OUT_1_DAC_EN.Write(true);
    }

    if(jack_config.out_2 == audio_enable)
    {
        OUT_2_AUDIO_EN.Write(true);
        OUT_2_DAC_EN.Write(false);
    } 
    else if(jack_config.out_2 == dac_enable)
    {
        OUT_2_AUDIO_EN.Write(false);
        OUT_2_DAC_EN.Write(true);
    }

    if((jack_config.gate_1_in_4 == adc_enable) || (jack_config.gate_1_in_4 == gpio_enable))
    {
        IN_4_EN.Write(true);
        GATE_1_EN.Write(false);
    }
    else if(jack_config.gate_1_in_4 == gate_enable)
    {
        IN_4_EN.Write(false);
        GATE_1_EN.Write(true);       
    }

    if((jack_config.gate_2_in_8 == adc_enable) || (jack_config.gate_2_in_8 == gpio_enable))
    {
        IN_8_EN.Write(true);
        GATE_2_EN.Write(false);
    }
    else if(jack_config.gate_2_in_8 == gate_enable)
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
    AdcChannelConfig adc_config[cvs.size()+2];
    adc_config[mux_x].InitMux(MUX_X_ADC_PIN, 4, D6, D7);
	adc_config[mux_y].InitMux(MUX_Y_ADC_PIN, 4, D6, D7);
    
    vector<struct cv>::iterator iter;

    for(iter = cvs.begin(); iter != cvs.end(); ++iter)
    {
        adc_config[(*iter).adc_channel_num + 2].InitSingle((*iter).pin);
    }
    
    seed.adc.Init(adc_config, cvs.size()+2);
    seed.adc.Start();
}