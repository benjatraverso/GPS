#ifndef PROGRAM_H
      #define PROGRAM_H
      #include <Servo.h>

      const unsigned int SRV = 9;
      const unsigned int FWD = 10;
      const unsigned int BWD = 4;
      const unsigned int EN =  3;
      const unsigned int ECHO = 11;
      const unsigned int TRIG = 12;

      Servo myservo;
      unsigned int passedTime = 0;

      ///////////////GLOBAL VARIABLES//////////////////
      unsigned int gState = 0;
      bool gbWaypointReceived;
      bool gbValidWaypoint;

      enum states
      {
        eIdle = 0,
        eReadingCommands,
        eValidateWaypoint,
        eMoveToWaypoint,
        eError
      };

      /***************PROTOCOL*******************
      GPS $GPRMC

      ignore anything until you receive a valid "$GPRMC"
      then start reading data devided by ','

      Block n°  Example    Description
            0   $GPRMC     Always starts like this...
            1   220516     Time Stamp
            2   A          validity - A-ok, V-invalid
            3   5133.82    current Latitude
            4   N          North/South
            5   00042.24   current Longitude
            6   W          East/West
            7   173.8      Speed in knots
            8   231.8      True course
            9   130694     Date Stamp
            10  004.2      Variation
            11  W          East/West
            12  *70        checksum incuding this checksum

      full block Example:
      "$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70"
      ********************************************/

      //////////////////////////////ATMEGA 320 USED PINS///////////////////////////////////
      //
      //                               +-----___-----+
      //                               | °           |
      //                         RESET | 1        28 | AI5 (SCL)   Left for I2C
      //                               |             |
      //                           RXD | 2        27 | AI4 (SDA)   Left for I2C
      //                               |             |
      //                           TXD | 3        26 | AI3         
      //                               |             |
      // ForwardGear          INT0 PD2 | 4        25 | AI2         
      //                               |             |
      //                      INT1 PD3 | 5        24 | AI1         
      //                               |             |
      //                           PD4 | 6        23 | AI0         
      //                               |             |
      //                           VCC | 7        22 | GND
      //                               |             |
      //                           GND | 8        21 | AREF
      //                               |             |
      //                         XTAL1 | 9        20 | AVCC
      //                               |             |
      // BackwardsGear           XTAL2 | 10       19 | DP13        
      //                               |             |
      //                     (PWM) DP5 | 11       18 | DP12        ObjectSensorTRIG 
      //                               |             |
      //                     (PWM) DP6 | 12       17 | DP11 (PWM)  ObjectSensorECHO
      //                               |             |
      //                           DP7 | 13       16 | DP10 (PWM)  
      //                               |             |
      //                           DP8 | 14       15 | DP9 (PWM)   Servo
      //                               |             |
      //                               +-------------+

#endif
