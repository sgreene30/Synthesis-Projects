#include "voice.h"

void voice::Init(float fs)
{
    osc_1_.Init(fs);
    osc_2_.Init(fs);   
    osc_3_.Init(fs);
    osc_4_.Init(fs);
    osc_5_.Init(fs);
    osc_6_.Init(fs);
    bandpass_1_.Init(fs);
    bandpass_2_.Init(fs);
    bandpass_3_.Init(fs);
    highpass_.Init(fs);
    amp_env_.Init(fs);
    noise_.Init();

    amp_env_.SetTime(ADENV_SEG_ATTACK, 0.001f);
    amp_env_.SetCurve(-100.0f);

    osc_1_.SetWaveform(osc_1_.WAVE_POLYBLEP_SQUARE);
    osc_2_.SetWaveform(osc_2_.WAVE_POLYBLEP_SQUARE);
    osc_3_.SetWaveform(osc_3_.WAVE_POLYBLEP_SQUARE);
    osc_4_.SetWaveform(osc_4_.WAVE_POLYBLEP_SQUARE);
    osc_5_.SetWaveform(osc_5_.WAVE_POLYBLEP_SQUARE);
    osc_6_.SetWaveform(osc_6_.WAVE_POLYBLEP_SQUARE);

    osc_1_.SetFreq(osc_1_base_freq_);
    osc_2_.SetFreq(osc_2_base_freq_);
    osc_3_.SetFreq(osc_3_base_freq_);
    osc_4_.SetFreq(osc_4_base_freq_);
    osc_5_.SetFreq(osc_5_base_freq_);
    osc_6_.SetFreq(osc_6_base_freq_);    

    bandpass_1_.SetFreq(5600.0f);
    bandpass_1_.SetRes(0.5f);
    bandpass_2_.SetFreq(5600.0f);
    bandpass_2_.SetRes(0.5f);
    bandpass_3_.SetFreq(5600.0f);
    bandpass_3_.SetRes(0.5f);
    highpass_.SetFreq(3000.0f);

    distortion_.Init();
    distortion_.SetDrive(1.0f);
}

void voice::set_global_tune(float tune)
{
    osc_1_.SetFreq(osc_1_base_freq_ * (powf(2.0f, tune)-0.41f));
    osc_2_.SetFreq(osc_2_base_freq_ * (powf(2.0f, tune)-0.41f));
    osc_3_.SetFreq(osc_3_base_freq_ * (powf(2.0f, tune)-0.41f));
    osc_4_.SetFreq(osc_4_base_freq_ * (powf(2.0f, tune)-0.41f));
    osc_5_.SetFreq(osc_5_base_freq_ * (powf(2.0f, tune)-0.41f));
    osc_6_.SetFreq(osc_6_base_freq_ * (powf(2.0f, tune)-0.41f));
}

void voice::set_bandpass_freq(float freq)
{
    freq = fmap(freq, 3000.0f, 8000.0f, Mapping::LINEAR);
    bandpass_1_.SetFreq(freq);
    bandpass_2_.SetFreq(freq);
    bandpass_3_.SetFreq(freq);
}

void voice::set_bandpass_res(float res)
{
    res = fmap(res, 0.1f, 1.0f, Mapping::LINEAR);
    bandpass_1_.SetRes(res);
    bandpass_2_.SetRes(res);
    bandpass_3_.SetRes(res);
}

void voice::set_highpass_freq(float freq)
{
    freq = fmap(freq, 1000.0f, 5000.0f, Mapping::LINEAR);
    highpass_.SetFreq(freq);
}

void voice::set_decay(float time)
{
    time = fmap(time, 0.01f, 2.0f, Mapping::LINEAR);
    amp_env_.SetTime(ADENV_SEG_DECAY, time);
}

void voice::env_trigger()
{
    amp_env_.Trigger();
}

float voice::Process()
{
    float ret = osc_1_.Process() + osc_2_.Process() + osc_3_.Process() + osc_4_.Process() + osc_5_.Process() + osc_6_.Process();
    ret *= 0.16666667f; //divide by number of oscillators to prevent clipping

    ret = noise_.Process() * 0.25f + ret; //add in noise at lower level
    float env_amp = amp_env_.Process();
    bandpass_1_.Process(ret);
    ret = bandpass_1_.Band();
    bandpass_2_.Process(ret);
    ret = bandpass_2_.Band();

    ret = distortion_.Process(ret * env_amp);

    highpass_.Process(ret);
    ret = highpass_.High();
    
    return ret;
}