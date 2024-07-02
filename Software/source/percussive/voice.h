#include "daisysp.h"
using namespace daisysp;

class voice
{
    public:
    voice() {}
    ~voice() {}

    void Init(float fs);
    void set_freq(float freq); //0-1.0f input which is gets to V/Oct
    void set_wave_shape(float shape); //0-1.0f input which maps from saw/tri/square
    /*
        sets the sideband oscillator amplitudes. the outer oscillators will roll off with decreasing input
        //0-1.0f maps to center only to equal amplitude 
    */
    float Process();
    void set_amp_env_attack(float time);
    void set_amp_env_decay(float time);
    void env_trigger();
    void set_impact(float impact); //0-1.0f sets the frequency envelope max height

    private:
    //VariableShapeOscillator osc_center;
    Oscillator osc;
    AdEnv amp_env;
    AdEnv freq_env;
    bool gate = false;
    float osc_freq;
    void set_freq_env_attack(float time);
    void set_freq_env_decay(float time);

};