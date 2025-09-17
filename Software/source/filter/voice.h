#include "daisysp.h"
using namespace daisysp;
class voice
{
    public:
    voice() {}
    ~voice() {}

    enum passbands
    {
        LOWPASS,
        BANDPASS,
        HIGHPASS
    };

    enum NoiseModes
    {
        WHITE,
        PINK,
        BLUE
    };

    void Init(float fs);
    float Process(float sample);
    void set_cutoff(float cutoff);
    void set_res(float res);
    void set_passband(u_int8_t passband);
    void set_attack(float time);
    void set_decay(float time); 
    void set_sustain(float level);
    void set_release(float time);
    void set_gate(bool gate);
    void set_env_amount(float amount);
    void set_noise_level(float level);
    void enable_4pole(bool enable);
    bool get_pole();
    bool get_gate();
    float get_env_value();
    void set_noise_mode(u_int8_t mode);
    float pink_noise_filter(float sample);
    float blue_noise_filter(float sample);


    private:
    float cutoff_, res_, attack_, decay_, sustain_, release_, env_amount_, env_value_, noise_level_;
    uint8_t noise_mode_;
    bool gate_, four_pole_enabled_;
    u_int8_t passband_;
    Svf filter_, filter2_;
    Adsr env_;
    WhiteNoise noise_;
    OnePole blue_noise_filter_;
};