#include "daisysp.h"
using namespace daisysp;

class voice
{
    public:
    voice() {}
    ~voice() {}
    
    //noise source
    enum
    {
        SIX_OSCILLATOR, GONG_XOR, MULTI_XOR  
    };

    void Init(float fs);
    float Process();
    void set_global_tune(float tune);
    void set_bandpass_freq(float freq);
    void set_bandpass_res(float res);
    void set_highpass_freq(float freq);
    void set_decay(float time);
    void env_trigger();

    private:
        AdEnv amp_env_;
        Svf bandpass_1_, bandpass_2_, bandpass_3_, highpass_;
        uint8_t noise_mode_;
        Oscillator osc_1_, osc_2_, osc_3_, osc_4_, osc_5_, osc_6_;
        Overdrive distortion_;
        WhiteNoise noise_;0
        float osc_1_base_freq_ = 264.0f;
        float osc_2_base_freq_ = 318.0f;
        float osc_3_base_freq_ = 375.0f;
        float osc_4_base_freq_ = 455.0f;
        float osc_5_base_freq_ = 557.0f;
        float osc_6_base_freq_ = 677.0f;


};