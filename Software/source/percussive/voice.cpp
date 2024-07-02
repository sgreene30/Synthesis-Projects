#include "voice.h"

void voice::Init(float fs)
{
    osc.Init(fs);
    osc.SetWaveform(osc.WAVE_SIN);
    amp_env.Init(fs);
    freq_env.Init(fs);
    freq_env.SetMin(1.0f);
}

void voice::set_freq(float freq)
{
    freq = mtof(freq*120);
    osc.SetFreq(freq);
    osc_freq = freq;
}

void voice::set_wave_shape(float shape)
{
    //osc_center.SetWaveform();
    //osc_center.SetWaveshape(fmap(shape, 0.25f, 1.0f, Mapping::LINEAR));
}

float voice::Process()
{
    osc.SetFreq(osc_freq * freq_env.Process());
    osc.SetAmp(amp_env.Process());
    
    return osc.Process();
}

void voice::set_amp_env_attack(float time)
{
    time = fmap(time, 0.002f, 0.5f, Mapping::LINEAR);
    set_freq_env_attack(time);
    amp_env.SetTime(ADENV_SEG_ATTACK, time);
}
void voice::set_amp_env_decay(float time)
{
    time = fmap(time, 0.002f, 0.5f, Mapping::LINEAR);
    set_freq_env_decay(time);
    amp_env.SetTime(ADENV_SEG_DECAY, time);
}

void voice::env_trigger()
{
    amp_env.Trigger();
    freq_env.Trigger();
}

void voice::set_freq_env_attack(float time)
{
    freq_env.SetTime(ADENV_SEG_ATTACK, time);
}

void voice::set_freq_env_decay(float time)
{
    time = time / 3.0f;
    freq_env.SetTime(ADENV_SEG_DECAY, time);
}

void voice::set_impact(float impact)
{
    freq_env.SetMax(1.0f + 8.0f*impact);
}