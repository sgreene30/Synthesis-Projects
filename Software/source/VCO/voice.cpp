#include "voice.h"

void voice::Init(float fs)
{
    osc_tri_.Init(fs);
    osc_tri_.SetAmp(1.0f);
    osc_tri_.SetWaveform(osc_tri_.WAVE_POLYBLEP_TRI);
    
    osc_saw_.Init(fs);
    osc_saw_.SetAmp(1.0f);
    osc_saw_.SetWaveform(osc_saw_.WAVE_POLYBLEP_SAW); 

    osc_sin_.Init(fs);
    osc_sin_.SetAmp(1.0f);
    osc_sin_.SetWaveform(osc_sin_.WAVE_SIN); 

    osc_square_.Init(fs);
    osc_square_.SetAmp(1.0f);
    osc_square_.SetWaveform(osc_square_.WAVE_POLYBLEP_SQUARE);   
}

void voice::set_freq(float freq)
{
    osc_midi_ = freq*120.0f;
    freq = mtof(osc_midi_);
    osc_freq_ = freq;
}

void voice::set_fine(float fine)
{
    fine = fmap(fine, -12.0f, 12.0f, Mapping::LINEAR);
    osc_freq_ += fine;
}

void voice::set_freq_curve(u_int8_t curve)
{
    freq_curve_ = curve;
}

void voice::set_shape(float shape)
{
    if(shape <= 0.333f)
    {
        sin_amp_ = (fmap(3.0f * shape, 0.0f, 1.0f, daisysp::Mapping::LINEAR) - 1.0f) * -1.0f;
        tri_amp_ = fmap(3.0f * shape, 0.0f, 1.0f, daisysp::Mapping::LINEAR);
        square_amp_ = 0.0f;
        saw_amp_ = 0.0f;
    }
    else if (shape > 0.333f && shape <=0.666f)
    {
        sin_amp_ = 0.0f;
        tri_amp_ = (fmap((shape-0.333f) * 3.0f, 0.0f, 1.0f, daisysp::Mapping::LINEAR) - 1.0f) * -1.0f;
        square_amp_ = fmap((shape-0.333f) * 3.0f, 0.0f, 1.0f, daisysp::Mapping::LINEAR);
        saw_amp_ = 0.0f;
    }
    else if (shape > 0.666f)
    {
        sin_amp_ = 0.0f;
        tri_amp_ = 0.0f;
        square_amp_ = (fmap((shape-0.666f) * 3.0f, 0.0f, 1.0f, daisysp::Mapping::LINEAR) - 1.0f) * -1.0f;
        saw_amp_ = fmap((shape-0.666f) * 3.0f, 0.0f, 1.0f, daisysp::Mapping::LINEAR);
    }
    else
    {
        sin_amp_ = 0.0f;
        tri_amp_ = 0.0f;
        square_amp_ = 0.0f;
        saw_amp_ = 1.0f;
    }
    
}

float voice::Process()
{
    float ret = 0.0f;
    osc_sin_.SetFreq(osc_freq_);
    osc_saw_.SetFreq(osc_freq_);
    osc_tri_.SetFreq(osc_freq_);
    osc_square_.SetFreq(osc_freq_);
    
    osc_sin_.SetAmp(sin_amp_);
    osc_saw_.SetAmp(saw_amp_);
    osc_tri_.SetAmp(tri_amp_);
    osc_square_.SetAmp(square_amp_);

    ret += osc_sin_.Process();
    ret += osc_saw_.Process();
    ret += osc_tri_.Process();
    ret += osc_square_.Process();

    return ret;
}

