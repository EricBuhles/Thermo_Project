//connect pin 9 -> 10k Ohm + (series with)100nF ceramic cap -> GND, tap the sinewave signal from the point at between the resistor and cap.

float wav1[3];                                //0 frequency, 1 unscaled amplitude, 2 is final amplitude
int average;
const int Pin = 9;
float time;
float percentage;
float templitude;
float offset = 2.5;                           //default value 2.5 volt as operating range voltage is 0~5V
float minOutputScale = 0.0;
float maxOutputScale = 5.0;
const int resolution = 1;                     //this determines the update speed. A lower number means a higher refresh rate.
const float pi = 3.14159;

#include <Wire.h>                             //Temp. library
#include "WaveShare_MLX90614.h"

WaveShare_MLX90614   MLX90614 = WaveShare_MLX90614();

void setup() 
{
//wav1[0] = 90;                               //frequency of the sine wave
wav1[1] = 2.5;                                //0V - 2.5V amplitude (Max amplitude + offset) value must not exceed the „maxOutputScale“
TCCR1B = TCCR1B & 0b11111000 | 1;             //set timer 1B (pin 9) to 31250khz
pinMode(Pin, OUTPUT);
//Serial.begin(115200);//this is for debugging

Serial.begin(115200);                         //Temp. Sensor Setup
MLX90614.begin(); 

}

long mapf(float x, float in_min, float in_max, long out_min, long out_max)
{
return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void loop() 
{
//Serial.print(" \tObject Temp: "); Serial.print(MLX90614.readObjectTemp()); Serial.println(" C");
//Serial.println();
//Serial.println( map(MLX90614.readObjectTemp(), 20, 35, 30, 90));
int temp_map = MLX90614.readObjectTemp();
Serial.println(map(temp_map, 19, 27, 30, 90));
if (temp_map<19){
  wav1[0] = 0;
  wav1[1] = -2.5; 
}
else {
  wav1[1] = 2.5; 
  wav1[0] = map(temp_map, 19, 27, 30, 90);
}
time = micros()% 1000000;
percentage = time / 1000000;
templitude = sin(((percentage) * wav1[0]) * 2 * pi);
wav1[2] = (templitude * wav1[1]) + offset;     //shift the origin of sinewave with offset.
average = mapf(wav1[2],minOutputScale,maxOutputScale,0,255); 
analogWrite(9, average);                       //set output „voltage“
delayMicroseconds(resolution);                 //this is to give the micro time to set the „voltage“
}






