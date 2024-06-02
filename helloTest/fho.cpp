#include "fho.h"
#include <cmath>

void fho::Init(float sample_rate)
{
    tanh_gain = 0.0f;
    folder.Init();
    osc.Init(sample_rate);
	osc.SetWaveform(osc.WAVE_POLYBLEP_TRI);
	osc.SetAmp(1.0f);
	osc.SetFreq(100.0f);
    /*for(size_t i = 0; i < filter_size; i++)
    {
        taps[i] = 0.0f;
    }*/

    taps[1]  = 0.0f;
    taps[3]  = 0.0f;
    taps[5]  = 0.0f;
    taps[7]  = 0.0f;
    taps[9]  = 0.0f;
    taps[13] = 0.0f;
    taps[15] = 0.0f;
    taps[17] = 0.0f;
    taps[19] = 0.0f;
    taps[21] = 0.0f;

    taps[0]  = -0.007193324475036675f;
    taps[22] = -0.007193324475036675f;
    taps[2]  = 0.013616925865793945f;
    taps[20] = 0.013616925865793945f;
    taps[4]  = -0.02628066406628572f;
    taps[18] = -0.02628066406628572f;
    taps[6]  = 0.04860759433245811f;
    taps[16] = 0.04860759433245811f;
    taps[8]  = -0.09646258355192097f;
    taps[14] = -0.09646258355192097f;
    taps[10] = 0.31500529607952577f;
    taps[12] = 0.31500529607952577f;
    taps[11] = 0.5000099882993633f;

    taps_2[0] = -0.029433258658846596f;
    
    filter.SetStateBuffer(filter_state, DSY_COUNTOF(filter_state));
    filter.Init(taps, filter_size, false);

    filter_2.SetStateBuffer(filter_2_state, DSY_COUNTOF(filter_state));
    filter_2.Init(taps, filter_size, false);   
}

void fho::set_freq(float freq)
{
    osc.SetFreq(freq);
    frequency = freq;
}

void fho::set_freq_voct(float freq)
{
    freq = mtof(freq*120);
    osc.SetFreq(freq);
    frequency = freq;
}

void fho::set_freq_lin(float freq)
{
    freq = fmap(freq, 50.0, 5000.0f);
    osc.SetFreq(freq);
    frequency = freq;
}

void fho::set_freq_lfo(float freq)
{
    freq = fmap(freq, 0.05f, 30.0f);
    osc.SetFreq(freq);
    frequency = freq;
}

void fho::set_fold_gain(float gain)
{
    folder.SetGain(fmap(gain, 1.0f, 11.0f));
    
}

void fho::set_fold_off(float offset)
{
    folder.SetOffset(fmap(offset, -1.0f, 1.0f));
}

void fho::set_tanh_gain(float gain)
{
    //gain = gain*(-0.00015*frequency)+1;
    tanh_gain = pow10f(2*gain) - 1.0f;
}

float fho::Process()
{
    float upsample[2];
    upsample[0] = osc.Process();
    upsample[0] = filter.Process(upsample[0]);
    upsample[0] = folder.Process(upsample[0]);
    upsample[0] = tanhf(upsample[0]*tanh_gain) / tanhf(tanh_gain);
    upsample[0] = filter_2.Process(upsample[0]);

    upsample[1] = 0.0f;
    upsample[1] = filter.Process(upsample[1]);
    upsample[1] = folder.Process(upsample[1]);
    upsample[1] = tanhf(upsample[1]*tanh_gain) / tanhf(tanh_gain);
    upsample[1] = filter_2.Process(upsample[1]);

    return upsample[0];
}