#include "mbed.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define ON 1 
#define OFF 0
#define OPEN 1
#define CLOSE 0
#define HIGH 1
#define LOW 0
#define FREQUENCY 0.05
#define PRESSED 0
#define NONPRESSED 1
#define PROXON 0
DigitalIn testPB (p15);
DigitalIn StopPB (p16);
DigitalIn StartPB (p17);

DigitalIn proxSensor1 (p18);
DigitalIn proxSensor2 (p19);
DigitalOut fillValve (p20);

DigitalOut Co2ControlValve (p29);
DigitalOut linearActuatorF (p9);
DigitalOut linearActuatorR (p8);
DigitalOut Enable1 (p10);//ENB1

DigitalOut LedG (p12);//green LED
DigitalOut LedR (p13);//red LED

DigitalOut D_1(p22);//m1+
DigitalOut D_2(p23);//M1-
DigitalOut D_3(p24);//m2+
DigitalOut D_4(p25);//m2-
PwmOut enable1 (p21);
PwmOut enable2 (p26);

Serial pc(USBTX, USBRX);

void fillBottle();
void carbonateBottle();
void SlewDriveControl();
void bothProcess();
void test();
void carbonateBottleTest();
void fillBottleTest();
void stepperControlFullTurn();
void stepperControlQuarterTurn();
void stepperControlHalfTurn();

// start of main: ----------------------------------------------------------------------

int main() 
{
    enable1 =OFF;
    enable2 =OFF;

    while(1)
        {
    if(testPB == PRESSED)
        {   
        test();
        }
        
        while (StopPB == NONPRESSED)//checks status of stop push button 
            {
            LedR = ON;
            LedG = OFF;
            fillValve = CLOSE;
            Co2ControlValve = CLOSE;
            int count = 0; 
            if(StartPB == PRESSED)//looks for start push button low 
                {
                while(StopPB == NONPRESSED)//confirming stop push button isnt high
                    {

                    LedG = ON;
                    LedR = OFF;
                    if (count == 0)// turns platform 90 degrees on start
                        {
                        stepperControlQuarterTurn();
                        count += 1;
                        }
                    wait(.2);
                    if(proxSensor1 == PROXON && proxSensor2 == PROXON)//checking for activation of both proxsensors
                        {
                        wait(1);
                        if(proxSensor1 == PROXON && proxSensor2 == PROXON)//confirming proxensor still activated
                            {
                            bothProcess();
                            }

                        }
                    if(proxSensor2 == PROXON) 
                        {   
                        wait(2);
                        if(proxSensor2 == PROXON)
                            {
                            fillBottle();
                            }

                        }
                    if(proxSensor1 == PROXON)
                        {
                        wait(2);
                        if(proxSensor1 == PROXON)
                            {
                            carbonateBottle();
                            }
                        } 

                   
                    }
                }
            }
        }
}

//eo main:: ==================================================================


// fillBottle: ---------------------------------------------------------------
//description: function opens fill valve for 8 seconds and closes fill valve 
//then calls funcion to turn platform 90 degrees.

void fillBottle()
{   
    if(proxSensor2 == PROXON)
    {
        wait(.2);
        fillValve = OPEN;
        wait(8);
        fillValve =CLOSE; 
        stepperControlQuarterTurn();
    }
    
     
}

//eo fillBottleOpen:: ==================================================================

// fillBottleTest: ---------------------------------------------------------------
//Description: open and closes fill valve for 3 seconds to confirm working 

void fillBottleTest()
{

        if(proxSensor2 == PROXON)
        {
            wait(3);
            if(proxSensor2 == PROXON)
            {
                fillValve = OPEN;// turn fill valve on 
                wait(3);
                fillValve =CLOSE;// turn fil valve off
                
            }
        
        }
 
}

//eo fillBottleTest:: ==============================================================



// carbonateBottle: ---------------------------------------------------------------
//decription: if the proximty sensor 1 is activated the linear actuator is lowered.
//next the co2 valve is opened and stays on for 5 seconds. The linear actuator is
//retracts and the quarter turn fuction is called. 
void carbonateBottle()
{

        if(proxSensor1 == PROXON)//checks status of proximity sensor is on
        {
        wait(1);
        Enable1 = ON;//enable pin on motor controller set to high
        linearActuatorF = HIGH;// actuator is activated lowering for 5 seconds
        linearActuatorR = LOW;
        wait(5);
        Co2ControlValve = OPEN;//co2 valve is turned on 
        wait(5);
        Co2ControlValve = CLOSE;// co2 valve is turned off
        linearActuatorF = LOW;// actuator set to reverse
        linearActuatorR = HIGH;
        wait(5);
        Enable1 = OFF;
        stepperControlQuarterTurn(); //turns platform 90 degrees
            
        }


}

//eo carbontateBottle:: ==================================================================
// bothProcess: ---------------------------------------------------------------
//Description: once both proximity sensors are activated runs fill and
//carbontaion process's at once. once both steps are done motor turns 90 degrees
void bothProcess()
{
    if(proxSensor1 == PROXON && proxSensor2 == PROXON) //checks status of both prox sensors
        {
        fillValve = OPEN;//turns on fill valve
        Enable1 = ON;
        linearActuatorF = HIGH;//extend linear actuator
        linearActuatorR = LOW;
        wait(5);
        Co2ControlValve = OPEN;//open co2 valve
        wait(3);
        fillValve = CLOSE;//close fill valve
        wait(2);
        Co2ControlValve = CLOSE; // close co2 valve
        Enable1 = ON;
        linearActuatorR = HIGH;//retract linear actuator
        linearActuatorF = LOW;
        wait(5);
        stepperControlQuarterTurn();//turn platform 90 degrees
        }
}

//eo bothProcess:: ==================================================================

// Test: ---------------------------------------------------------------
//Description: This fuction is designed to execute if the test push button 
//is activated.  The test fuctions that are called based on the proximity sensor 
//inputs are the fill and carbonate test stages.
void test()
{
        while (StopPB == NONPRESSED)
        {
            LedR = ON;//stop led to on 
            wait(.2);
            LedR = OFF;//stop led off
            wait(.2);
            LedG = OFF;// start led off
            if(StartPB == PRESSED)
            {
                while(StopPB == NONPRESSED)
                {
                    LedR = OFF;
                    LedG = ON;
                    wait(.2);
                    LedG = ON;
                    wait(.2);
                    
                    fillBottleTest();
                    carbonateBottleTest();
                }
            }
        }
       
}
//eo Test:: =================================================================

//carbonateBottleTest: ==================================================================
//decription: This fuction is desgined to test the co2 injection stage of the project.
// This is done by fully extending the actuator and opening the co2 valve for 1 second.
//The actuator is then fully retracted.
void carbonateBottleTest()
{
    if(proxSensor1 == PROXON )//checks status of proximity sensor 1
    {   
        wait(3);
        if(proxSensor1 == PROXON)//confirming status of prox senor 1
        {
            wait(.2);
            Enable1 = ON;// enable pin on motor controller to high
            linearActuatorF = HIGH;//starting actuator to forward direction
            linearActuatorR = LOW;
            wait(5);
            Co2ControlValve = OPEN;//co2 valve on 
            wait(1);
            Co2ControlValve = CLOSE;//co2 valve off
            linearActuatorF = LOW;//linear actuator reverse
            linearActuatorR = HIGH;
            wait(5);
            Enable1 = OFF;//enable pin on motor controller off
        } 
    }
}
//eo carbonateBottelTest:: =================================================================

//stepperControlQuarterTurn: ==================================================================
//Description: sets PWM pins and turns plateform 90 degrees
void stepperControlQuarterTurn()
{
    wait(0.5);
    enable1.period(0.05);//setting period to 5 ms
    enable1 = 0.2;//setting Duty cycle to 20%
    enable2.period(0.05);//setting period to 5 ms
    enable2 = 0.2;//setting duty cycle to 20%    
    D_1 = LOW; D_2 = LOW; D_3 = LOW; D_4 = LOW;
    for(int i = 0; i<25; i++)
        {
        D_1 = HIGH; D_2 = LOW; D_3 = LOW; D_4 = HIGH;//HIGH LOW
        wait(FREQUENCY);
        D_1 = HIGH; D_2 = LOW;D_3 = HIGH; D_4 = LOW;//HIGH HIGH
        wait(FREQUENCY);
        D_1 = LOW; D_2 = HIGH;D_3 = HIGH; D_4 = LOW; //LOW HIGH
        wait(FREQUENCY);
        D_1 = LOW; D_2 = HIGH;D_3 = LOW; D_4 = HIGH;//LOW LOW 
        wait(FREQUENCY);
        if(i == 24) //EXTRA 2 steps for consistanat 90 degree turn
            {
            D_1 = HIGH; D_2 = LOW; D_3 = LOW; D_4 = HIGH;
            wait(FREQUENCY);
            D_1 = HIGH; D_2 = LOW;D_3 = HIGH; D_4 = LOW;
            wait(FREQUENCY);   
            }
        }
    D_1 = LOW; D_2 = LOW; D_3 = LOW; D_4 = LOW;
    enable1 = OFF;
    enable2 = OFF;       
}
//eo stepperControlQuarterTurn:: =================================================================

