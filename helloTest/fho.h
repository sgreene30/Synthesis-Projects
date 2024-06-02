#include "daisysp.h"
#include "multirate.h"
using namespace daisysp;

class fho
{
    public:
    fho() {}
    ~fho() {}

    void Init(float sample_rate);
    float Process();
    void set_freq_voct(float in);
    void set_freq_lin(float in);
    void set_freq_lfo(float in);
    void set_freq(float in);
    void set_tanh_gain(float gain); //takes value of 0-1
    void set_fold_gain(float gain); //takes value of 0-1
    void set_fold_off(float offset); //takes value of 0-1

    private:
    float tanh_gain;
    float frequency;
    Wavefolder folder;
    Oscillator osc;
    //half_band filter_unity;
    //half_band filter_2;
    //ring_buffer buffer;
    FIR<FIRFILTER_USER_MEMORY> filter;
    FIR<FIRFILTER_USER_MEMORY> filter_2;
    size_t filter_size = 23;
    float filter_state[23 + 1];
    float filter_2_state[23 + 1];
    float taps[23];
    float taps_2[14];



    //FIR<FIRFILTER_USER_MEMORY> filt;
    //static constexpr size_t filt_len = 3;
    //static float filt_tap[filt_len];
    //static float filt_state[filt_len + 1];       /*< Impl-specific storage */

};