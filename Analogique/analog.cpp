// Source lecture analogique
// IC: MCP3008
// ------------
// 16 VDD 3.3V  15 VRef:3.3V
// 14 AGND: GND 13 CLK
// 12 DOUT      11 DIN 
// 10 CS         9 DGND GND
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#define MOSI  12  //Physical 19 read
#define MISO  13  //         21 Send
#define Clk   28  //  22       23
#define CE    29 //          40

//#define LED1   29    // pin 40
//#define LED2   28    // pin 38
//#define LED3   27    // pin 36
void pulse()
{
digitalWrite(Clk,0);
delay(10);
digitalWrite(Clk,1);
delay(10);
}
int main(void)
{
int data,value;
	printf("Lecture MCP3008 Analogique\n");
	printf("---------------------------\n");

	// Calling wiringPi setup first.
	wiringPiSetup();
	pinMode(MOSI, INPUT); 
	pinMode(MISO, OUTPUT);
	pinMode(Clk, OUTPUT);
	pinMode(CE , OUTPUT);
	delay(200);

// data Start + Single + D2 + D1 + D0	  
//     1         1        0    0    0	  
	digitalWrite(MISO,0);
	digitalWrite(Clk,0);
	digitalWrite(CE,1);
        for(int j=0;j<10000;j++)
          {
	   digitalWrite(CE,0);
	   delay(1);
	      pulse();
	      digitalWrite(MISO,1); //Start
              pulse();
              digitalWrite(MISO,1);
              pulse();
              digitalWrite(MISO,0);
              pulse();
              digitalWrite(MISO,0);
              pulse();
	      digitalWrite(MISO,0);
	      pulse();
	   while(digitalRead(MOSI)) pulse(); //Attend le start bit  
           data=0;
	   value=0;
	   for(int i=0;i<21;i++) // lecture 1 bit vide, 10 data, 1 bit null
	     { // 00 0000000010 100000000  Data:1280   Value:2
	       // 00 1100110111 110110011  Data:421811   Value:823
	       //    9876543210 123456789  21 bits de tramsission 
	     pulse();
	     data <<=1;
	     if(i<12) value <<=1;	
	     if( digitalRead(MOSI)) 
	        { 
		 if(i<12) value |=1;
		 data |=1;
	         printf("1");
		}
	     else 
	        {
		 if(i<12) value |=0;
		 data |=0;
		 printf("0");
	        }
	     if(i==11) printf(" ");
 	     }
	   printf("  Data:%i   Value:%i\n",data,value);
	   digitalWrite(CE,1);
	   delay(1);
          }
	return 0;
}
