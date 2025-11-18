#include "voice.h"

void voice::Init(float fs)
{
    osc_.Init(fs);
    osc_.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);
    
    filter_up_.SetStateBuffer(filter_state_up_, DSY_COUNTOF(filter_state_up_));
    filter_up_.Init(taps_up_, ntaps_, false);

    filter_down_.SetStateBuffer(filter_state_down_, DSY_COUNTOF(filter_state_down_));
    filter_down_.Init(taps_down_, ntaps_, false);

    filter_dist_.SetStateBuffer(filter_state_dist_, DSY_COUNTOF(filter_state_dist_));
    filter_dist_.Init(taps_down_, ntaps_, false); //using same lowpass

    dist_lpf_.Init();
    dist_lpf_.SetFrequency(0.5f); //halfband cutoff
}

void voice::set_freq(float freq)
{
    freq = mtof(freq*120);
    freq_ = freq;// + fine_;
}

void voice::set_fine(float fine)
{
    fine_ = fmap(fine_, -1.0f*freq_/2.0f, freq_*2.0f, Mapping::LINEAR);
}

void voice::set_fold(float fold)
{
    float slope = -1.0f/6000.0f;
    if(freq_ > 500.0f)
    {
        fold = fold * ((slope * (freq_-500.0f)) + 1.0f);
        fold = fclamp(fold, 0.0f, 1.0f);
    } 
    fold_factor_ = fmap(fold, 0.4f, 7.0f);
}

void voice::set_distort(float distort)
{
    float slope = -1.0f/6000.0f;
    if(freq_ > 500.0f)
    {
        distort = distort * ((slope * (freq_-500.0f)) + 1.0f);
        distort = fclamp(distort, 0.0f, 1.0f);
    } 
    distort_factor_ = pow10f(2*distort) - 0.99f;;
}

float voice::Process()
{
    float sample[upsample_factor_];

    osc_.SetFreq(freq_);
    
    sample[0] = osc_.Process(); //generate base sample
    for(int i = 0; i < upsample_factor_; i++) //apply upsampling halfband filter and effect
    {
        if(i > 0)
            sample[i] = 0.0f; //zero padding for upsampling
        
        sample[i] = filter_up_.Process(sample[i]); //apply upsampling filter

        //apply distortion and bandlimit
        sample[i] = tanhf(sample[i]*distort_factor_) / tanhf(distort_factor_);
        //sample[i] = filter_dist_.Process(sample[i]);
        sample[i] = dist_lpf_.Process(sample[i]);
        //apply wavefolding
        sample[i] = (tanhf(sample[i]*fold_factor_) / tanhf(fold_factor_) + 0.8*sinf(2*fold_factor_*sample[i]))/1.8f;
        
        //downsample
        sample[i] = filter_down_.Process(sample[i]);
    }

    return sample[0];
}



