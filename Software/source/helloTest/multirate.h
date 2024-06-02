class ring_buffer
{
    public:
        ring_buffer() {};
        ~ring_buffer() {};
        void Init()
        {
            ntaps = 23;
            for(int i = 0; i < ntaps; i++) buf[i] = 0.0f;
            pos = 0;
        }
        void add(float sample)
        {
            buf[pos] = sample;
            pos++;
            if(pos == ntaps) pos = 0;
        }

        float read(int index)
        {
            index = pos - index - 1;
            if(index < 0) index -= ntaps;

            return buf[index];
        }
    private:
        int pos;
        float buf[23];
        int ntaps;
};

class half_band
{
    public:
        half_band() {};
        ~half_band() {};
        float process(float sample) 
        {
            float ret;
            buffer.add(sample);
            
            /*ret = (buffer.read(0) + buffer.read(22)) * taps[0];
            ret = ret + (buffer.read(2) + buffer.read(20)) * taps[2];
            ret = ret + (buffer.read(4) + buffer.read(18)) * taps[4];
            ret = ret + (buffer.read(6) + buffer.read(16)) * taps[6];
            ret = ret + (buffer.read(8) + buffer.read(14)) * taps[8];
            ret = ret + (buffer.read(10) + buffer.read(12)) * taps[10];
            ret = ret + buffer.read(11) * taps[11];*/

            ret = buffer.read(0);
            return ret;
        }

        void Init(float gain = 1.0f)
        {
            buffer.Init();

            for(int i = 0; i < 23; i++) taps[i] = 0.0f;
            
            taps[0]  = -0.007193324475036675f;
            taps[2]  = 0.013616925865793945f;
            taps[4]  = -0.02628066406628572f;
            taps[6]  = 0.04860759433245811f;
            taps[8]  = -0.09646258355192097f;
            taps[10] = 0.31500529607952577f;
            taps[11] = 0.5000099882993633f;

            if(gain != 1.0f)
            {
                for(int i = 0; i < 23; i++) taps[i] = taps[i] * gain;
            }
        }
    private:

        ring_buffer buffer;
        float taps[23];
};