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

    filter_up_.SetStateBuffer(filter_state_up_, DSY_COUNTOF(filter_state_up_));
    filter_up_.Init(taps_up_, ntaps_, false);

    filter_down_.SetStateBuffer(filter_state_down_, DSY_COUNTOF(filter_state_down_));
    filter_down_.Init(taps_down_, ntaps_, false);   
}

void voice::set_freq(float freq)
{
    float osc_midi;
    if(freq_curve_ == voct)
    {
        osc_midi = freq*120.0f;
        freq = mtof(osc_midi);
        osc_freq_ = freq;
    }
    else if (freq_curve_ == linear)
    {
        osc_freq_ = fmap(freq, 30, 16000, daisysp::Mapping::LINEAR);
    }
    else if (freq_curve_ == lfo)
    {
        osc_freq_ = fmap(freq, 0.01f, 30.0f, daisysp::Mapping::EXP);
    }
    
    
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

void voice::set_morph(float morph)
{
    morph_ = morph;
    fold_gain_ = fmap(morph_, 0.4f, 7.0f);
}

float voice::Process()
{
    float sample[2];
    sample[0] = 0.0f;
    osc_square_.SetPw(morph_/2.0f + 0.5f);

    osc_sin_.SetFreq(osc_freq_);
    osc_saw_.SetFreq(osc_freq_);
    osc_tri_.SetFreq(osc_freq_);
    osc_square_.SetFreq(osc_freq_);
    
    osc_sin_.SetAmp(sin_amp_);
    osc_saw_.SetAmp(saw_amp_);
    osc_tri_.SetAmp(tri_amp_);
    osc_square_.SetAmp(square_amp_);

    sample[0] += osc_sin_.Process();
    sample[0] += osc_saw_.Process();
    sample[0] += osc_tri_.Process();
    //sample[0] = filter_up_.Process(sample[0]);

    //fold
    sample[0] = (tanhf(sample[0]*fold_gain_) / tanhf(fold_gain_) + 0.8*sinf(2*fold_gain_*sample[0]))/1.8f;

    /*sample[0] = filter_down_.Process(sample[0]);

    sample[1] = 0.0f;
    sample[1] = filter_up_.Process(sample[1]);

    //fold
    sample[1] = (tanhf(sample[1]*fold_gain_) / tanhf(fold_gain_) + 0.8*sinf(2*fold_gain_*sample[1]))/1.8f;

    sample[1] = filter_down_.Process(sample[1]);*/

    sample[0] += osc_square_.Process();

    return sample[0];
}

