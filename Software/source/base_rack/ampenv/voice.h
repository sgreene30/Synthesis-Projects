#include "daisysp.h"
using namespace daisysp;

#define SEGMENT_MAX 10.0f
#define SEGMENT_MIN 0.002f
#define LPF_FREQ_MIN 20.0f
#define LPF_FREQ_MAX 20000.0f

class voice
{
    public:
    voice() {}
    ~voice() {}
    void Init(float fs);
    float Process(float sample);
    void set_volume(float volume);
    void set_attack(float time);
    void set_decay(float time); 
    void set_sustain(float level);
    void set_release(float time);
    void set_gate(bool gate);
    bool get_gate();
    void set_env_amount(float amount);
    float get_env_value();
    void set_lpf_freq(float freq);
    void set_lpf_amount(float amount);

    private:
    float env_value_, env_amount_, volume_, fs_, lpf_amount_, lpf_freq_;
    bool gate_;
    OnePole lpf_;
    Adsr env_;
};