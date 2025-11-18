#include "voice.h"

void voice::Init(float fs) 
{
    fs_ = fs;
    env_.Init(fs_);
    reverb_.Init(fs_);
}

float voice::Process(float sample) 
{
    float ret, ret2;;
    env_value_ = env_.Process(gate_);
    ret = sample * (volume_ + env_value_*env_amount_);

    //ret = reverb_.Process(ret, ret, &ret, &ret2);

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
    reverb_.SetFeedback(feedback);
}

void voice::set_dampening_filter(float dampening)
{  
    dampening = powf(dampening, 3.3f)*(LPF_FREQ_MAX - LPF_FREQ_MIN);
    dampening = fclamp(dampening, LPF_FREQ_MIN, LPF_FREQ_MAX);
    reverb_.SetLpFreq(dampening);
}