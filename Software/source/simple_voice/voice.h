#include "daisysp.h"
using namespace daisysp;

class voice
{
    public:
    voice() {}
    ~voice() {}

    enum Waveshape
    {
        SQUARE,
        TRIANGLE,
        SAW
    };

    void Init(float fs);
    float Process();
    void set_osc_freq(float freq);
    void set_osc_spread(float spread);
    void set_osc_sideband(float sideband);
    void set_fx(float fx);
    void set_filter_freq(float freq);
    void set_filter_res(float res);
    void set_attack(float time);
    void set_decay(float time);
    void set_waveshape(int shape);
    void env_trigger();



    private:
    Oscillator osc1_, osc2_, osc3_, osc4_, osc5_;
    AdEnv env_;
    Svf filter_;
    float spread_, fx_, freq_;

};