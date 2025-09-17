#include "voice.h"
#include "daisy_common.h"

void voice::Init(float fs)
{
    // TODO: Initialize voice with sample rate fs
    osc1_.Init(fs);
    osc2_.Init(fs);
    osc3_.Init(fs);
    osc4_.Init(fs);
    osc5_.Init(fs);
    env_.Init(fs);
    filter_.Init(fs);

    env_.SetCurve(-50.0f); //set exp curve

}

float voice::Process()
{
    // TODO: Process and return next audio sample
    float ret = osc1_.Process();
    ret += osc2_.Process();
    ret += osc3_.Process();
    ret += osc4_.Process();
    ret += osc5_.Process();    

    env_.Process();
    filter_.Process(ret);
    ret = filter_.Low()*env_.GetValue();
    return ret;
}

void voice::set_osc_freq(float freq)
{
    // TODO: Set oscillator frequency
    //osc1_.SetFreq(430.0f);
    freq = mtof(freq*120.0f);
    osc3_.SetFreq(freq);

    float high = (1.0f+spread_)*freq;
    float mid_high = fmap(high, 1.0f, 1.5f, daisysp::Mapping::LINEAR);
    
    float low = (1.0f-spread_*0.5f)*freq;
    float mid_low = fmap(low, 0.75f, 1.0f, daisysp::Mapping::LINEAR);

    osc1_.SetFreq(low);
    osc2_.SetFreq(mid_low);
    osc4_.SetFreq(mid_high);
    osc5_.SetFreq(high);

}

void voice::set_osc_spread(float spread)
{
    // TODO: Set oscillator spread
    spread_ = spread;
}

void voice::set_osc_sideband(float sideband)
{
    float sideband_far = fmap(sideband, 0.0f, 1.0f, daisysp::Mapping::EXP);
    float sideband_near = fmap(sideband, 0.0f, 2.0f, daisysp::Mapping::EXP);
    float center = 0.33f;
    sideband_far = fclamp(sideband_far, 0.0f, 0.33f);
    sideband_near = fclamp(sideband_near, 0.0f, 0.33f);

    osc1_.SetAmp(sideband_far);
    //osc2_.SetAmp(sideband_near);
    osc3_.SetAmp(center); 
    //osc4_.SetAmp(sideband_near);
    osc5_.SetAmp(sideband_far);
}

void voice::set_fx(float fx)
{
    // TODO: Set FX parameter
}

void voice::set_filter_freq(float freq)
{
    // TODO: Set filter frequency
    filter_.SetFreq(mtof(freq * 132.0f));
}

void voice::set_filter_res(float res)
{
    // TODO: Set filter resonance
    filter_.SetRes(res);
}

void voice::set_attack(float time)
{
    // TODO: Set envelope attack time
    time = fmap(time, 0.02f, 5.0f, daisysp::Mapping::EXP);
    env_.SetTime(AdEnvSegment::ADENV_SEG_ATTACK, time);
}

void voice::set_decay(float time)
{
    // TODO: Set envelope decay time
    time = fmap(time, 0.02f, 5.0f, daisysp::Mapping::EXP);
    env_.SetTime(AdEnvSegment::ADENV_SEG_DECAY, time);
}

void voice::set_waveshape(int shape)
{
    switch(shape)
    {
        case Waveshape::TRIANGLE: shape = osc1_.WAVE_POLYBLEP_TRI; break;
        case Waveshape::SQUARE: shape = osc1_.WAVE_POLYBLEP_SQUARE; break;
        case Waveshape::SAW: shape = osc1_.WAVE_POLYBLEP_SAW; break;
        default: shape = osc1_.WAVE_POLYBLEP_TRI; break;
    }
    osc1_.SetWaveform(shape);
    osc2_.SetWaveform(shape);
    osc3_.SetWaveform(shape);
    osc4_.SetWaveform(shape);
    osc5_.SetWaveform(shape);   
}

void voice::env_trigger()
{
    env_.Trigger();
}