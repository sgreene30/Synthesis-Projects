#include "voice.h"

void voice::Init(float fs) 
{
    filter_.Init(fs);
    filter2_.Init(fs);
    env_.Init(fs);
    noise_.Init();
    blue_noise_filter_.Init();
    blue_noise_filter_.SetFilterMode(OnePole::FILTER_MODE_HIGH_PASS);
    blue_noise_filter_.SetFrequency(0.0625f);

    four_pole_enabled_ = false;
}

void voice::set_cutoff(float cutoff) 
{
    // TODO: Implement cutoff frequency setting
    cutoff_ = mtof(cutoff * 132.0f);
}

void voice::set_res(float res) 
{
    // TODO: Implement resonance setting
    res_ = res;
    filter_.SetRes(res_);
    filter2_.SetRes(res_);
}

float voice::Process(float sample) 
{
    // TODO: Implement audio processing logic
    float ret, mod, noise_sample;
    
    env_value_ = env_.Process(gate_);

    // set filter cutoff frequency
    mod = fclamp(cutoff_ + 25.0f * env_value_ * env_amount_ * cutoff_, 0.0f, 20000.0f);
    filter_.SetFreq(mod);
    filter2_.SetFreq(mod);

    switch (noise_mode_)
    {
    case PINK:
        /* code */
        noise_sample = noise_.Process();
        noise_sample = pink_noise_filter(noise_sample);
        break;
    case WHITE:
        noise_sample = noise_.Process();
        break;
    case BLUE:
        noise_sample = noise_.Process();
        noise_sample = blue_noise_filter(noise_sample);
        break;
    default:
        noise_sample = noise_.Process();
        break;
    }


    filter_.Process(sample + noise_sample);
    
    if(passband_ == LOWPASS)
        ret = filter_.Low();
    else if(passband_ == BANDPASS)
        ret = filter_.Band();
    else if(passband_ == HIGHPASS)
        ret = filter_.High();
    else
        ret = filter_.Low();

    if(four_pole_enabled_)
    {
        filter2_.Process(ret);
        if(passband_ == LOWPASS)
            ret = filter2_.Low();
        else if(passband_ == BANDPASS)
            ret = filter2_.Band();
        else if(passband_ == HIGHPASS)
            ret = filter2_.High();
        else
            ret = filter2_.Low();
    }
    
    
    return ret;
}

void voice::set_passband(u_int8_t passband)
{
    passband_ = passband;
}

void voice::set_attack(float time)
{       
    float max = 10.0f;
    float min = 0.002f;    
    time = powf(time, 3.3)*(max - min);
    env_.SetAttackTime(fclamp(time, min, max));
}

void voice::set_decay(float time)
{
    float max = 10.0f;
    float min = 0.002f;    
    time = powf(time, 3.3)*(max - min);
    env_.SetDecayTime(fclamp(time, min, max));
}

void voice::set_sustain(float level)
{
    env_.SetSustainLevel(level);
}

void voice::set_release(float time)
{
    float max = 10.0f;
    float min = 0.002f;
    time = powf(time, 3.3)*(max - min);
    env_.SetReleaseTime(fclamp(time, min, max));
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

void voice::set_noise_level(float level)
{
    noise_level_ = fclamp(level * 1.1f - 0.05f, 0.0f, 1.0f);
    noise_.SetAmp(noise_level_);
}

void voice::enable_4pole(bool enable)
{
    four_pole_enabled_ = enable;
}

bool voice::get_pole()
{
    return four_pole_enabled_;
}

float voice::pink_noise_filter(float sample)
{
    // Convert white noise to pink noise using a simple filter
    // Based on Paul Kellet's filter (efficient, suitable for real-time audio)
    static float b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0;

    b0 = 0.99886f * b0 + sample * 0.0555179f;
    b1 = 0.99332f * b1 + sample * 0.0750759f;
    b2 = 0.96900f * b2 + sample * 0.1538520f;
    b3 = 0.86650f * b3 + sample * 0.3104856f;
    b4 = 0.55000f * b4 + sample * 0.5329522f;
    b5 = -0.7616f * b5 - sample * 0.0168980f;
    float pink = b0 + b1 + b2 + b3 + b4 + b5 + b6 + sample * 0.5362f;
    b6 = sample * 0.115926f;

    return pink * 0.2f; // scale to roughly match white noise amplitude
}

void voice::set_noise_mode(u_int8_t mode)
{
    noise_mode_ = mode;
}
float voice::blue_noise_filter(float sample)
{
    // Simple blue noise filter: high-pass filter on white noise
    // Blue noise has +6dB/octave slope (emphasizes high frequencies)
    // We'll use a simple first-order high-pass filter

    float ret = blue_noise_filter_.Process(sample);

    // Scale to match white noise amplitude roughly
    return ret * 1.5f;
}