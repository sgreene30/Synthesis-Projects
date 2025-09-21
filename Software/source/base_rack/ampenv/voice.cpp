#include "voice.h"

void voice::Init(float fs) 
{
    fs_ = fs;
    env_.Init(fs_);
    lpf_.Init();
    lpf_.SetFilterMode(OnePole::FILTER_MODE_LOW_PASS);
    lpf_.SetFrequency(0.497f);
}

float voice::Process(float sample) 
{
    float ret;
    env_value_ = env_.Process(true);


    lpf_.SetFrequency(lpf_freq_);
    ret = sample * (volume_ + env_value_*env_amount_);

    return ret;
}
void voice::set_attack(float time)
{        
    time = powf(time, 3.3)*(SEGMENT_MAX - SEGMENT_MIN);
    env_.SetAttackTime(fclamp(time, SEGMENT_MIN, SEGMENT_MAX));
}

void voice::set_decay(float time)
{   
    time = powf(time, 3.3)*(SEGMENT_MAX - SEGMENT_MIN);
    env_.SetDecayTime(fclamp(time, SEGMENT_MIN, SEGMENT_MAX));
}

void voice::set_sustain(float level)
{
    env_.SetSustainLevel(level);
}

void voice::set_release(float time)
{
    time = powf(time, 3.3)*(SEGMENT_MAX - SEGMENT_MIN);
    env_.SetReleaseTime(fclamp(time, SEGMENT_MIN, SEGMENT_MAX));
}

void voice::set_gate(bool gate)
{
    gate_ = gate;
}

bool voice::get_gate()
{
    return gate_;
}

void voice::set_env_amount(float amount)
{
    env_amount_ = amount;
}

float voice::get_env_value()
{
    return env_value_;
}

void voice::set_volume(float volume)
{
    volume_ = volume;   
}

void voice::set_lpf_freq(float freq)
{
    freq = fmap(freq, LPF_FREQ_MIN, LPF_FREQ_MAX, Mapping::EXP) / fs_;
    lpf_freq_ = freq;
}

void voice::set_lpf_amount(float amount)
{
    lpf_amount_ = amount;
}