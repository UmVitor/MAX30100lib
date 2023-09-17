/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <math.h>
#include <Arduino.h>

#include "MAX30100_SpO2Calculator.h"

// SaO2 Look-up Table
// http://www.ti.com/lit/an/slaa274b/slaa274b.pdf
const uint8_t SpO2Calculator::spO2LUT[43] = {100, 100, 100, 100, 99, 99, 99, 99, 99, 99, 98, 98, 98, 98,
                                             98, 97, 97, 97, 97, 97, 97, 96, 96, 96, 96, 96, 96, 95, 95,
                                             95, 95, 95, 95, 94, 94, 94, 94, 94, 93, 93, 93, 93, 93};

SpO2Calculator::SpO2Calculator() : irACValueSqSum(0),
                                   redACValueSqSum(0),
                                   beatsDetectedNum(0),
                                   samplesRecorded(0),
                                   spO2(0),
                                   spO2Amp(0),
                                   mixedIrACValueSqSum(0),
                                   mixedRedACValueSqSum(0)

{
}

void SpO2Calculator::update(float irACValue, float redACValue, bool beatDetected)
{
    irACValueSqSum += irACValue * irACValue;
    redACValueSqSum += redACValue * redACValue;
    ++samplesRecorded;

    if (beatDetected)
    {
        ++beatsDetectedNum;
        if (beatsDetectedNum == CALCULATE_EVERY_N_BEATS)
        {
            float acSqRatio = 100.0 * log(redACValueSqSum / samplesRecorded) / log(irACValueSqSum / samplesRecorded);
            uint8_t index = 0;

            if (acSqRatio > 66)
            {
                index = (uint8_t)acSqRatio - 66;
            }
            else if (acSqRatio > 50)
            {
                index = (uint8_t)acSqRatio - 50;
            }
            reset();

            spO2 = spO2LUT[index];
            Serial.print("Inside Update: ");
            Serial.println(spO2);
        }
    }
}

void SpO2Calculator::updateAmp(float irACValue1,
                               float redACValue1,
                               float irACValue2,
                               float redACValue2,
                               bool beatDetected)
{

    // Serial.print(irACValue1);
    // Serial.print(" ");
    // Serial.print(redACValue1);
    // Serial.print(" ");
    // Serial.print(irACValue2);
    // Serial.print(" ");
    // Serial.print(redACValue2);
    // Serial.print(" ");
    // Serial.println(beatDetected);

    float differenceSignalIr = instrumentationAmplifier(irACValue1, irACValue2);
    float differenceSignalRed = instrumentationAmplifier(redACValue1, redACValue2);
    irACValueSqSumAmp += differenceSignalIr * differenceSignalIr;
    redACValueSqSumAmp += differenceSignalRed * differenceSignalRed;
    ++samplesRecordedAmp;
    if (beatDetected)
    {
        ++beatsDetectedNumAmp;
        if (beatsDetectedNumAmp == CALCULATE_EVERY_N_BEATS)
        {
            Serial.print(redACValueSqSumAmp);
            Serial.print(" ");
            Serial.print(irACValueSqSumAmp);
            Serial.print(" ");
            Serial.print(samplesRecordedAmp);
            Serial.print(" ");
            float acSqRatio = 100.0 * log(redACValueSqSumAmp / samplesRecordedAmp) / log(irACValueSqSumAmp / samplesRecordedAmp);
            Serial.print(acSqRatio);
            uint8_t index = 0;

            if (acSqRatio > 66)
            {
                index = (uint8_t)acSqRatio - 66;
            }
            else if (acSqRatio > 50)
            {
                index = (uint8_t)acSqRatio - 50;
            }
            reset();

            spO2Amp = spO2LUT[index];
            Serial.print(" ");
            Serial.print(index);
            Serial.print(" ");
            Serial.println(spO2Amp);
        }
    }
}

void SpO2Calculator::reset()
{
    samplesRecorded  = 0;
    redACValueSqSum  = 0;
    irACValueSqSum   = 0;
    beatsDetectedNum = 0;
    samplesRecordedAmp = 0;
    redACValueSqSumAmp = 0;
    irACValueSqSumAmp = 0;
    beatsDetectedNumAmp = 0;
    spO2 = 0;
    spO2Amp = 0;
    mixedIrACValueSqSum = 0;
    mixedRedACValueSqSum = 0;
}

uint8_t SpO2Calculator::getSpO2()
{
    return spO2;
}

uint8_t SpO2Calculator::getSpO2Amp()
{
    return spO2Amp;
}

float SpO2Calculator::instrumentationAmplifier(float signal1, float signal2)
{
    float R1 = 10e3;
    float R2 = 10e3;
    float R3 = 10e3;
    float R4 = 10e3;
    float Vout;
    float gain = (R4 / R3) * (1 + (R2 / R1));
    Vout = gain * (signal2 - signal1);
    return Vout;
}
