#include "daisysp.h"
using namespace daisysp;

class voice
{
    public:
    voice() {}
    ~voice() {}
    enum
    {
        DISTORT, FOLD, OTHER  
    };
    enum
    {
        TRI, SIN, SAW
    };
    enum
    {
        NOISE, GONG, FM
    };

    void Init(float fs);
    void set_freq(float freq); //0-1.0f input which is gets to V/Oct
    void set_wave_shape(uint8_t shape); //0-1.0f input which maps from saw/tri/square
    float Process();
    void set_freq_env_decay(float time);
    void set_amp_env_decay(float time);
    void env_trigger();
    void set_impact(float impact); //0-1.0f sets the frequency envelope max height
    void set_effect_mode(uint8_t mode);
    void set_effect_value(float value);
    void set_noise_mix(float mix);
    void set_noise_effect_1(float value);
    void set_noise_effect_2(float value);
    void set_noise_mode(uint8_t mode);

    private:
    Fm2 fm_;
    Oscillator osc_;
    AdEnv amp_env_, freq_env_;
    WhiteNoise noise_;
    OnePole noise_lpf_, noise_hpf_;
    bool gate_ = false;
    float osc_freq_, distortion_gain_, fold_gain_, noise_mix_, noise_color_, fm_carrier_, mod_ind_, amp_decay_time_;
    uint8_t effect_, noise_mode_;
    void set_carrier(float freq);
    void set_distortion(float gain);
    void set_fold(float gain);
    float process_drum(float amplitude);
    float process_noise(float amplitude);

    FIR<FIRFILTER_USER_MEMORY> filter_up_;
    FIR<FIRFILTER_USER_MEMORY> filter_down_;
    //FIR<FIRFILTER_USER_MEMORY> filter_dist;
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