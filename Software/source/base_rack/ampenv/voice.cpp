#include "voice.h"

void voice::Init(float fs) 
{
    fs_ = fs;
    env_.Init(fs_);
    reverb_.Init(fs_);
    delay_line_.Init();
}

float voice::Process(float sample) 
{
    float ret, delay_out;
    env_value_ = env_.Process(gate_);
    ret = 0.5f*sample * (volume_ + env_value_);

    delay_out = delay_line_.Read()* 0.4f;
    //ret = reverb_.Process(ret, ret, &ret, &ret2);
    
    delay_line_.Write(delay_out * feedback_ + ret);
    ret += delay_out * delay_mix_;
    return ret;
}
void voice::set_attack(float time)
{    
    /*float max = 10.0f;
    float min = 0.002f;    
    time = powf(time, 3.3)*(max - min);
    env_.SetAttackTime(fclamp(time, min, max));   */ 
    
    time = powf(time, 3.3)*(SEGMENT_MAX - SEGMENT_MIN);
    env_.SetAttackTime(fclamp(time, SEGMENT_MIN, SEGMENT_MAX));
}

void voice::set_decay(float time)
{   
    /*float max = 10.0f;
    float min = 0.002f;    
    time = powf(time, 3.3)*(max - min);
    env_.SetDecayTime(fclamp(time, min, max));*/

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
    volume_ = fclamp(volume * 1.1f - 0.05f, 0.0f, 1.0f);   
}

void voice::set_feedback(float feedback)
{
    feedback = fclamp(feedback * 1.1f - 0.05f, 0.0f, 0.99f);
    feedback_ = feedback;
    reverb_.SetFeedback(feedback);
}

void voice::set_dampening_filter(float dampening)
{  
    dampening = powf(dampening, 3.3f)*(LPF_FREQ_MAX - LPF_FREQ_MIN);
    dampening = fclamp(dampening, LPF_FREQ_MIN, LPF_FREQ_MAX);
    reverb_.SetLpFreq(dampening);
}

void voice::set_delay_time(float time)
{
    float max_delay_seconds = static_cast<float>(MAX_DELAY) / fs_;
    time = time * max_delay_seconds;
    float delay_in_samples = time * fs_;
    delay_line_.SetDelay(delay_in_samples);
}

void voice::set_delay_mix(float mix)
{
    mix = fclamp(mix * 1.1f - 0.05f, 0.0f, 1.0f);
    delay_mix_ = mix;
}