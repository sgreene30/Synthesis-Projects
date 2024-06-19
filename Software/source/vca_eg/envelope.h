#include "daisysp.h"
using namespace daisysp;

class envelope
{
    public:
    envelope() {};
    ~envelope() {};
    void Init(float fs);
    float Process();
    void setAttack(float time);
    void setDecay(float time);
    void setSustain(float level);
    void setRelease(float time);

    Adsr env_gen;
    bool gate;
    int curve;
    float value;

    enum
    {
        exponential = 1, linear, logarithmic
    };
};