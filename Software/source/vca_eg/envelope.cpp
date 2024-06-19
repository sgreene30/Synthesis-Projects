#include "envelope.h"
#include <cmath >

void envelope::Init(float fs)
{
    env_gen.Init(fs);
}

float envelope::Process()
{
    float env_signal;
    float max = 1.0f;
    float min = 0.0f;
    float a = 1.0f/pow10f(max);
    env_signal = env_gen.Process(gate);

    if(curve == linear) return fmap(env_signal, min, max, Mapping::LINEAR);
    else if (curve == exponential) return fmap(env_signal, min, max, Mapping::EXP);
    else return fclamp(fastlog10f((env_signal+a)/0.11), min, max);
}

void envelope::setAttack(float time)
{
    float max = 10.0f;
    float min = 0.002f;    
    time = powf(time, 3.3)*(max - min);
    env_gen.SetAttackTime(fclamp(time, min, max));
}

void envelope::setDecay(float time)
{
    float max = 10.0f;
    float min = 0.002f;    
    time = powf(time, 3.3)*(max - min);
    env_gen.SetDecayTime(fclamp(time, min, max));
}

void envelope::setSustain(float level)
{
    env_gen.SetSustainLevel(level);
}

void envelope::setRelease(float time)
{
    float max = 10.0f;
    float min = 0.002f;
    time = powf(time, 3.3)*(max - min);
    env_gen.SetReleaseTime(fclamp(time, min, max));
}