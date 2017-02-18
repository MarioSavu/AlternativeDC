
#define fpval(x) *(float*)(&x)
#define shortval(x) *(short*)(&x)

class Data
{
private:
    union
    {
        struct
        {
            short accX, accY, accZ;
        };
        struct
        {
            short gyroX, gyroY, gyroZ;
        };
    };

public:
    // reading data from sensors
    void read()
    {
        readAccel();
        readGyro();
    }

    void readAccel()
    {
    }

    void readGyro()
    {
    }

    int dateTypeSize()
    {
        return sizeof(accX);
    }

    void reset()
    {
        accX = accY = accZ = 0;
    }
};

class Control
{
private:
    static const int n = 31;
    Data datas[n], thresholdsAcc, thresholdsGyro, sumAcc, sumGyro;

public:
    Control()
    {
        sumAcc.reset();
        sumGyro.reset();
        thresholdsAcc.reset();
        thresholdsGyro.reset();
    }

    void init()
    {
        calibSensorThreshold();
    }

    void rotateLeftBuffer()
    {
        for (int i = 0; i < n - 1; i++)
            datas[i] = datas[i + 1];
    }

    void calibSensorThreshold()
    {
        // some initial data readings

        // accel, gyro
        int step;

        for (int i = 0; i < 6; i++)
        {
            step = i * sizeof(datas[0].dateTypeSize());
            float minim = *(((float*)datas + 0) + step);
            float maxim = minim;

            for (int j = 1; j < n; j++)
            {
                if (minim > *(((float*)datas + j) + step))
                    minim = *(((float*)datas + j) + step);
                if (maxim < *(((float*)datas + j) + step))
                    maxim = *(((float*)datas + j) + step);
            }

            if (i < 3)
                *((float*)&thresholdsAcc + step) = (maxim - minim) * 1.5;
            else
                *((float*)&thresholdsGyro + step) = (maxim - minim) * 1.5;
        }
    }

    void read()
    {
        datas[n].read();
    }
};

Control c;

void setup()
{
    // put your setup code here, to run once:
    c.init();
}

void move()
{
    c.read();
}

void loop()
{
    c.rotateLeftBuffer();
    move();
}
