#include "MAX30100_Interceptor.h"
#include <Arduino.h>

Interceptor::Interceptor() : IrACValue1(0),
                             RedACValue1(0),
                             IrACValue2(0),
                             RedACValue2(0),
                             RedACValueMixed(0), 
                             IrACValueMixed(0)
{
}

void Interceptor::SetSignal(float irACValue, float redACValue, uint16_t flagInterceptor, bool internBeatDetected)
{
    if(flagInterceptor == 1){
        IrACValue1 = irACValue;
        RedACValue1 = redACValue;
    }
    else{
        IrACValue2 = irACValue;
        RedACValue2 = redACValue;
        spO2calculator.update(IrACValue1,
                              RedACValue1,
                              IrACValue2,
                              RedACValue2,
                              internBeatDetected);

        RedACValueMixed = spO2calculator.instrumentationAmplifier(RedACValue1, RedACValue2);
        IrACValueMixed = spO2calculator.instrumentationAmplifier(IrACValue1, IrACValue2);
        // Serial.print(IrACValue1);
        // Serial.print(",");
        // Serial.print(RedACValue1);
        // Serial.print(","); 
        // Serial.print(IrACValue2);
        // Serial.print(",");
        // Serial.print(RedACValue2);
        // Serial.print(",");
        // Serial.print(RedACValueMixed);
        // Serial.print(",");
        // Serial.println(IrACValueMixed);
    }
    
}

void Interceptor::SetSignalAmp(float irACValue, float redACValue, uint16_t flagInterceptor, bool internBeatDetected)
{
    if (flagInterceptor == 1)
    {
        IrACValue1 = irACValue;
        RedACValue1 = redACValue;
    }
    else
    {
        IrACValue2 = irACValue;
        RedACValue2 = redACValue;
        spO2calculator.updateAmp(IrACValue1,
                              RedACValue1,
                              IrACValue2,
                              RedACValue2,
                              internBeatDetected);

        RedACValueMixed = spO2calculator.instrumentationAmplifier(RedACValue1, RedACValue2);
        IrACValueMixed = spO2calculator.instrumentationAmplifier(IrACValue1, IrACValue2);

        Serial.print((-1)*IrACValue1);
        Serial.print(",");
        Serial.print((-1)*RedACValue1);
        Serial.print(",");
        Serial.print(IrACValue2);
        Serial.print(",");
        Serial.print(RedACValue2);
        Serial.print(",");
        Serial.print(IrACValueMixed);
        Serial.print(",");
        Serial.println(RedACValueMixed);
    }
}

void Interceptor::Reset(){
    spO2calculator.reset();
}

uint8_t Interceptor::GetSpO2()
{

    return spO2calculator.getSpO2Amp();
}

void Interceptor::GetAllValues(){
    Serial.print(IrACValue1);
    Serial.print(" ");
    Serial.print(RedACValue1);
    Serial.print(" ");
    Serial.print(IrACValue2);
    Serial.print(" ");
    Serial.println(RedACValue2);
}

float Interceptor::GlimpseSignal(){
    float irMixed = spO2calculator.instrumentationAmplifier(IrACValue1, IrACValue2);
    float redMixed = spO2calculator.instrumentationAmplifier(RedACValue1, RedACValue2);

    return irMixed;
}