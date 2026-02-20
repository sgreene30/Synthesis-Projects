#include "daisysp.h"

using namespace daisysp;

#define SEGMENT_MAX 10.0f
#define SEGMENT_MIN 0.002f
#define LPF_FREQ_MIN 20.0f
#define LPF_FREQ_MAX 20000.0f
#define MAX_DELAY static_cast<size_t>(48000 * 0.5f)

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
    void set_feedback(float feedback);
    void set_dampening_filter(float dampening);
    void set_delay_time(float time);
    void set_delay_mix(float mix);

    private:
    float env_value_, env_amount_, volume_, fs_, lpf_amount_, lpf_freq_, feedback_, delay_mix_, delay_time_;
    bool gate_;
    Adsr env_;
    ReverbSc reverb_;
    DelayLine<float, MAX_DELAY> delay_line_;
};