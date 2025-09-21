#include "daisysp.h"
using namespace daisysp;

class voice
{
    public:
    voice() {}
    ~voice() {}

    enum
    {
        lfo, linear, voct
    };
  
    void Init(float fs);
    void set_freq(float freq);
    void set_fine(float fine);
    void set_shape(float shape);
    void set_freq_curve(u_int8_t curve);
    void set_morph(float morph);
    float Process();

    private:
    Oscillator osc_tri_, osc_saw_, osc_sin_, osc_square_;
    float osc_freq_, tri_amp_, sin_amp_, square_amp_, saw_amp_, morph_, fold_gain_;
    u_int8_t freq_curve_;

    FIR<FIRFILTER_USER_MEMORY> filter_up_;
    FIR<FIRFILTER_USER_MEMORY> filter_down_;
    
    size_t ntaps_ = 29;
    float filter_state_up_[29 + 1]; //cutoff 0.25 with gain = 2
    float filter_state_down_[29 + 1]; //cutoff 0.25 with unity gain
    float filter_state_dist_[29 + 1]; //cutoff 0.25 with unity gain

    float taps_up_[29] = {-1.16187669e-04, -8.19955441e-04, -1.45988993e-03,
        4.19630776e-04,  6.69320352e-03,  1.38548692e-02,  1.10941131e-02,
       -1.21341323e-02, -5.02746685e-02, -7.14470765e-02, -2.95248822e-02,
        1.01259966e-01,  2.93690181e-01,  4.68856259e-01,  5.39817141e-01,
        4.68856259e-01,  2.93690181e-01,  1.01259966e-01, -2.95248822e-02,
       -7.14470765e-02, -5.02746685e-02, -1.21341323e-02,  1.10941131e-02,
        1.38548692e-02,  6.69320352e-03,  4.19630776e-04, -1.45988993e-03,
       -8.19955441e-04, -1.16187669e-04};

    float taps_down_[29] = {-5.80938346e-05, -4.09977720e-04, -7.29944966e-04,
        2.09815388e-04,  3.34660176e-03,  6.92743461e-03,  5.54705653e-03,
       -6.06706615e-03, -2.51373342e-02, -3.57235382e-02, -1.47624411e-02,
        5.06299828e-02,  1.46845091e-01,  2.34428129e-01,  2.69908570e-01,
        2.34428129e-01,  1.46845091e-01,  5.06299828e-02, -1.47624411e-02,
       -3.57235382e-02, -2.51373342e-02, -6.06706615e-03,  5.54705653e-03,
        6.92743461e-03,  3.34660176e-03,  2.09815388e-04, -7.29944966e-04,
       -4.09977720e-04, -5.80938346e-05};

};