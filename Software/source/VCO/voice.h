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
    float Process();

    private:
    Oscillator osc_tri_, osc_saw_, osc_sin_, osc_square_;
    float osc_freq_, tri_amp_, sin_amp_, square_amp_, saw_amp_, osc_midi_;
    u_int8_t freq_curve_;


};