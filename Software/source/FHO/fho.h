#include "daisysp.h"
using namespace daisysp;

/*
    Folding Hyperbolic Oscillator (FHO)
    Implements  Oscillator with wavefolding and distortion
*/

class fho
{
    public:
    fho() {}
    ~fho() {}

    void Init(float sample_rate);
    
    /*
    *   Process() generates the next sample of the oscillator
    *   Signal is upsampled from 96kHz by factor of two and then downsampled back to 96kHz
    *   Two half band filters filter and filter_2 are applied during sample rate conversion.
    *   Nonlinear functions are applied to upsampled signal to reduce aliasing
    */
    float Process();
    void set_freq_voct(float in); //set oscillator frequency with volt per octave input scaled 0-1
    void set_freq_lin(float in);  //set oscillator fequency on linear scaled 0-1
    void set_freq_lfo(float in);  //set oscillator frequency linearly in LFO mode scaled 0-1
    void set_freq(float in);      //set frequency unscaled
    void set_tanh_gain(float gain); //sets distorition gain takes value of 0-1
    void set_fold_gain(float gain); //sets folder gain takes value of 0-1
    void set_fold_off(float offset); //sets folder offset takes value of 0-1

    private:
    float tanh_gain;
    float frequency;
    Wavefolder folder;
    Oscillator osc;
    FIR<FIRFILTER_USER_MEMORY> filter; //unity gain half band filter
    FIR<FIRFILTER_USER_MEMORY> filter_2; //gain = 2 half band filter
    size_t filter_size = 23;
    float filter_state[23 + 1];
    float filter_2_state[23 + 1];
    float taps[23];
    float taps_2[14];
};