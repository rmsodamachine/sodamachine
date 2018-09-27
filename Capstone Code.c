#include "mbed.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define HIGH 1
#define LOW 0

DigitalIn StartPB (p17);
DigitalIn proxSensor1 (p18);
DigitalIn proxSensor2 (p19);
DigitalIn proxSensor3 (p20);
DigitalOut fillValve (p21);
DigitalOut Co2ControlValve (p22);
DigitalOut linearActuatorF (p23);
DigitalOut linearActuatorR (p24);
DigitalOut LedG (p25);
DigitalOut LedR (p26);

void fillBottle();
void carbonateBottle();
void SlewDriveControl();


// main: ----------------------------------------------------------------------


int main() 
{

    while (1)//(StopPB == 0)
    {
        //if(startPB == 0)
       // {
        fillBottle();
        //carbonateBottle();
       // }
    }
}

//eo main:: ==================================================================


// fillBottle: ---------------------------------------------------------------


void fillBottle()
{
    while(1)
    {
        if(proxSensor1 == 0)
        {
            fillValve = 1;
            wait(8);
            fillValve =0;
        
        }
        break;
    } 
}

//eo fillBottle:: ==================================================================



// carbonateBottle: ---------------------------------------------------------------

void carbonateBottle()
{
    while(1)
    {
        if(proxSensor2 == 0)
        {
            linearActuatorF = HIGH;
            linearActuatorR = LOW;
            wait(5);
            
            
            Co2ControlValve = 1;
            wait(5);
            Co2ControlValve = 0; 
            
            linearActuatorR = HIGH;
            linearActuatorF = LOW;
            wait(5);
            
        }

        break;   
    } 
}

//eo carbontateBottle:: ==================================================================

// SlewDriveControl: ---------------------------------------------------------------

void slewDriveControl()
{
    
}

//eo SlewDriveControl:: ==================================================================

