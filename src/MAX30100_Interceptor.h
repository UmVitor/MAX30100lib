#ifndef MAX30100_INTERCEPTOR_H
#define MAX30100_INTERCEPTOR_H

#include <stdint.h>
#include "MAX30100_SpO2Calculator.h"

class Interceptor
{
public:
    Interceptor();
    void SetSignal(float irACValue, float redACValue, uint16_t flagInterceptor, bool internBeatDetected);
    void SetSignalAmp(float irACValue, float redACValue, uint16_t flagInterceptor, bool internBeatDetected);
    void Reset();
    uint8_t GetSpO2();
    float IrACValue1;
    float RedACValue1;
    float IrACValue2;
    float RedACValue2;
    float RedACValueMixed;
    float IrACValueMixed;
    void GetAllValues();
    float GlimpseSignal();

private:
    SpO2Calculator spO2calculator;
};


#endif