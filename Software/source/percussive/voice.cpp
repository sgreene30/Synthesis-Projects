#include "voice.h"

void voice::Init(float fs)
{
    osc_.Init(fs);
    osc_.SetWaveform(osc_.WAVE_TRI);
    amp_env_.Init(fs);
    amp_env_.SetTime(ADENV_SEG_ATTACK, 0.002f);
    freq_env_.Init(fs);
    freq_env_.SetMin(1.0f);
    freq_env_.SetTime(ADENV_SEG_ATTACK, 0.002f);
    

    filter_up_.SetStateBuffer(filter_state_up_, DSY_COUNTOF(filter_state_up_));
    filter_up_.Init(taps_up_, ntaps_, false);

    filter_down_.SetStateBuffer(filter_state_down_, DSY_COUNTOF(filter_state_down_));
    filter_down_.Init(taps_down_, ntaps_, false);

    noise_.Init();
    noise_hpf_.Init();
    noise_hpf_.SetFrequency(0.005f);
    noise_hpf_.SetFilterMode(noise_hpf_.FILTER_MODE_HIGH_PASS);
    noise_lpf_.Init();
    noise_lpf_.SetFrequency(0.005f);

    fm_.Init(fs);
    fm_.SetRatio(0.5f);
    //filter_dist.SetStateBuffer(filter_state_dist, DSY_COUNTOF(filter_state_dist));
    //filter_dist.Init(taps_down, ntaps, false);    
}

void voice::set_noise_mix(float mix)
{
    fmap(mix, -0.05f, 1.05f);
    if(mix < 0.0f) mix = 0.0f;
    else if (mix > 1.0f) mix = 1.0f;
    
    noise_mix_ = mix;
}

void voice::set_freq(float freq)
{
    freq = mtof(freq*120);
    //osc_.SetFreq(freq);
    osc_freq_ = freq;
}

void voice::set_wave_shape(uint8_t shape)
{
    if(shape == TRI) osc_.SetWaveform(osc_.WAVE_POLYBLEP_TRI);
    else if(shape == SIN) osc_.SetWaveform(osc_.WAVE_SIN);
    else if(shape == SAW) osc_.SetWaveform(osc_.WAVE_POLYBLEP_SAW);
}

void voice::set_distortion(float gain)
{
    distortion_gain_ = pow10f(2*gain) - 0.99f;
}

void voice::set_fold(float gain)
{
    fold_gain_ = fmap(gain, 0.4f, 7.0f);
}

void voice::set_effect_value(float value)
{
    if(effect_ == FOLD) 
    {
        set_fold(value);
    }
    else if(effect_ == DISTORT)
    {
        set_distortion(value);
    } 
}

float voice::Process()
{
    float amplitude;
    amplitude = amp_env_.Process();

    return process_drum(amplitude) + noise_mix_*process_noise(amplitude);
}

float voice::process_drum(float amplitude)
{
    float sample[2];
    osc_.SetFreq(osc_freq_ * freq_env_.Process());
    osc_.SetAmp(amplitude*(1-noise_mix_));

    sample[0] = osc_.Process();
    sample[0] = filter_up_.Process(sample[0]);

    if(effect_ == FOLD) sample[0] = (tanhf(sample[0]*fold_gain_) / tanhf(fold_gain_) + 0.8*sinf(2*fold_gain_*sample[0]))/1.8f;
    else if(effect_ == DISTORT) sample[0] = tanhf(sample[0]*distortion_gain_) / tanhf(distortion_gain_);
    else sample[0] = sample[0];

    sample[0] = filter_down_.Process(sample[0]);


    sample[1] = 0.0f;
    sample[1] = filter_up_.Process(sample[1]);

    if(effect_ == FOLD) sample[1] = (tanhf(sample[1]*fold_gain_) / tanhf(fold_gain_) + 0.8*sinf(2*fold_gain_*sample[1]))/1.8f;
    else if(effect_ == DISTORT) sample[1] = tanhf(sample[1]*distortion_gain_) / tanhf(distortion_gain_);
    else sample[1] = sample[1];

    sample[1] = filter_down_.Process(sample[1]);
    
    return sample[0];
}

float voice::process_noise(float amplitude)
{
    float noise, noise_low, noise_high;

    noise = 0.0f;
    if(noise_mode_ == NOISE)
    {
        noise_.SetAmp(amplitude);
        noise = noise_.Process();
        noise_low = noise_lpf_.Process(noise);
        noise_high = noise_hpf_.Process(noise);

        noise = noise_low * noise_color_ + noise_high * (1.0f - noise_color_);
    }
    else if (noise_mode_ == GONG)
    {
        noise = 0.0f;
    }
    else if (noise_mode_ == FM)
    {
        fm_.SetIndex(mod_ind_*amplitude);
        noise = fm_.Process()*amplitude; 
    }
    
    return noise;
}

void voice::set_noise_mode(uint8_t mode)
{
    noise_mode_ = mode;
}

void voice::set_noise_effect_1(float value)
{
    noise_color_ = value;
    
    value = mtof(value*120);
    fm_.SetFrequency(value);
    fm_carrier_ = value;
    
}

void voice::set_noise_effect_2(float value)
{
    value = fmap(value, -0.05f, 2.0f);
    if(value < 0.0f)
    {
        value = 0.0f;
    }
    mod_ind_ = value;
}

void voice::set_amp_env_decay(float time)
{
    time = fmap(time, 0.002f, 2.0f, Mapping::LINEAR);
    amp_decay_time_ = time;
    amp_env_.SetTime(ADENV_SEG_DECAY, time);
}

void voice::env_trigger()
{
    amp_env_.Trigger();
    freq_env_.Trigger();
}

void voice::set_effect_mode(uint8_t mode)
{
    effect_ = mode;
}

void voice::set_freq_env_decay(float time)
{
    time = time * amp_decay_time_;
    freq_env_.SetTime(ADENV_SEG_DECAY, time);
}

void voice::set_impact(float impact)
{
    freq_env_.SetMax(1.0f + 8.0f*impact);
}