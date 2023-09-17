#include "MAX30100_Interceptor.h"
#include <Arduino.h>


Interceptor::Interceptor() : IrACValue1(0),
                             RedACValue1(0),
                             IrACValue2(0),
                             RedACValue2(0)
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

        // Serial.print(IrACValue1);
        // Serial.print(" - ");
        // Serial.print(RedACValue1);
        // Serial.print(" - "); 
        // Serial.print(IrACValue2);
        // Serial.print(" - ");
        // Serial.print(RedACValue2);
        // Serial.print(" - ");
        // Serial.println("internBeatDetected");
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
        
        // Serial.print(IrACValue1);
        // Serial.print(" ");
        // Serial.print(RedACValue1);
        // Serial.print(" ");
        // Serial.print(IrACValue2);
        // Serial.print(" ");
        // Serial.print(RedACValue2);
        // Serial.print(" ");
        // Serial.println(internBeatDetected);
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