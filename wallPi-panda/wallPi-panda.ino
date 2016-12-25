#include <Arduino.h>
#include <Chrono.h>

#define NUM_TARGETS 5
#define BUTTON_PIN 12
#define BUTTON_LED_PIN 13

#define TARGET1_PIN A0
#define TARGET2_PIN A1
#define TARGET3_PIN A2
#define TARGET4_PIN A3
#define TARGET5_PIN A4

#define TARGET1_LED_PIN 2
#define TARGET2_LED_PIN 3
#define TARGET3_LED_PIN 4
#define TARGET4_LED_PIN 5
#define TARGET5_LED_PIN 6

#define MAX_PANDA_GAME 10
#define MIN_PANDA_GAME 3

boolean enabled_game=false;
char startGameC[] = "STARTC";
char stopGameC[] = "STOPC:%d";

Chrono gameTimer(Chrono::SECONDS); //2Min
Chrono targetTimer(Chrono::MILLIS ); //1s
int result=0;
int panda_stage=0;
int target_index=0;
unsigned int hitpoints=0;
unsigned int targetAttempts=0;

void setup() {


pinMode(BUTTON_PIN, INPUT_PULLUP);
pinMode(TARGET1_PIN, INPUT);
pinMode(TARGET2_PIN, INPUT);
pinMode(TARGET3_PIN, INPUT);
pinMode(TARGET4_PIN, INPUT);
pinMode(TARGET5_PIN, INPUT);

pinMode(BUTTON_LED_PIN, OUTPUT);
pinMode(TARGET1_LED_PIN, OUTPUT);
pinMode(TARGET2_LED_PIN, OUTPUT);
pinMode(TARGET3_LED_PIN, OUTPUT);
pinMode(TARGET4_LED_PIN, OUTPUT);
pinMode(TARGET5_LED_PIN, OUTPUT);

pinMode(7, OUTPUT);
digitalWrite(BUTTON_LED_PIN, HIGH);
digitalWrite(TARGET1_LED_PIN, LOW);
digitalWrite(TARGET2_LED_PIN, LOW);
digitalWrite(TARGET3_LED_PIN, LOW);
digitalWrite(TARGET4_LED_PIN, LOW);
digitalWrite(TARGET5_LED_PIN, LOW);
digitalWrite(7, HIGH);
//Serial.println("init complete");
Serial.begin(9600);
while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
}

}

void loop()
{
//
//    while(1)
//  {
//Serial.print(digitalRead(TARGET1_PIN));
//Serial.print(digitalRead(TARGET2_PIN));
//Serial.print(digitalRead(TARGET3_PIN));
//Serial.print(digitalRead(TARGET4_PIN));
//Serial.print(digitalRead(TARGET5_PIN));
//Serial.println("");
//delay(200);
//  
//
//  
//  }

  
  if(enabled_game == false)
  {
    if(digitalRead(BUTTON_PIN) == LOW)
    {
        delay(100);
        if(digitalRead(BUTTON_PIN) == LOW)
        {
          digitalWrite(BUTTON_LED_PIN, LOW);
          enabled_game=true;
          Serial.print(startGameC);
          Serial.print("\n");
          panda_stage=0;
          delay(100);
         // Serial.println("init complete");
        }
      }
        else
  {
    digitalWrite(BUTTON_LED_PIN, HIGH);
  }

      
  }

  if(enabled_game == true)
  {
    if (pandaGame())
    {
      char command[100] = "";
      sprintf(command,
              stopGameC,
              result
             );
      Serial.print(command);
      Serial.print("\n");
      delay(100);
      enabled_game=false;
      endBlink();
    }
  }

}//end loop


void endBlink(){
 for(int i=0; i<5 ; i++)
 {
         digitalWrite(BUTTON_LED_PIN, HIGH);
      delay(100);
      digitalWrite(BUTTON_LED_PIN, LOW);
      delay(100);
 }
   digitalWrite(BUTTON_LED_PIN, HIGH);
  
}

int pandaGame()


{



  switch (panda_stage)
  {
    case 0:
      target_index = 0;
      hitpoints = 0;
      targetAttempts = 0;
      for(int i=0; i<NUM_TARGETS ; i++)
      {
        disTarget(i);
      }
      panda_stage = 1;
      break;

    case 1:
      gameTimer.restart();
      targetTimer.restart();
      panda_stage = 2;
      break;

    case 2:
      enTarget(target_index);
      targetTimer.restart();
      panda_stage = 3;
      break;

    case 3:
      if (gameTimer.hasPassed(30))
      {
        panda_stage = 4;
        break;
      }

      if (targetTimer.hasPassed(1000)) //change target on 1,4s?
      {
        disTarget(target_index);
       
       // targetTimer.restart();
        target_index++;
        enTarget(target_index);

        targetAttempts++;
        if (target_index > NUM_TARGETS)
          target_index = 0;

        panda_stage = 2;
      }
      else

        if (readTarget(target_index))
        {
              if (readTarget(target_index))
            {
              targetBlink(target_index);
              disTarget(target_index);
              //targetTimer.restart();
              //Serial.println("hit");
              target_index++;

              if (target_index > NUM_TARGETS)
                target_index = 0;
              hitpoints++;
            }
        }
      break;

    case 4: // end of GAME
      panda_stage = 0;
      if(hitpoints<=targetAttempts)
      {
        result = map(hitpoints, 0, targetAttempts,MIN_PANDA_GAME ,MAX_PANDA_GAME);
      }else
      result= MAX_PANDA_GAME-1;

//
//      Serial.print("hitpoints: ");
//      Serial.println(hitpoints);
//      Serial.print("Target Attempts: ");
//      Serial.println(targetAttempts);
      for (int i = 0; i < NUM_TARGETS; i++)
      {
        disTarget(i);
      }
      return result;
      break;
  }

  return 0;
}//end


void enTarget(int num){
switch(num)
{
case 0: digitalWrite(TARGET1_LED_PIN, HIGH); break;
case 1: digitalWrite(TARGET2_LED_PIN, HIGH); break;
case 2: digitalWrite(TARGET3_LED_PIN, HIGH); break;
case 3: digitalWrite(TARGET4_LED_PIN, HIGH); break;
case 4: digitalWrite(TARGET5_LED_PIN, HIGH); break;
}
}

void disTarget(int num){
switch(num)
{
case 0: digitalWrite(TARGET1_LED_PIN, LOW); break;
case 1: digitalWrite(TARGET2_LED_PIN, LOW); break;
case 2: digitalWrite(TARGET3_LED_PIN, LOW); break;
case 3: digitalWrite(TARGET4_LED_PIN, LOW); break;
case 4: digitalWrite(TARGET5_LED_PIN, LOW); break;
}
}

int readTarget(int num){
switch(num)
{
case 0: return (digitalRead(TARGET1_PIN) == LOW); break;
case 1: return (digitalRead(TARGET2_PIN) == LOW); break;
case 2: return (digitalRead(TARGET3_PIN) == LOW); break;
case 3: return (digitalRead(TARGET4_PIN) == LOW); break;
case 4: return (digitalRead(TARGET5_PIN) == LOW); break;
}
return 0;
}

void targetBlink(int num ){
  for(int i=0; i<2; i++)
  {
    enTarget(num);
    delay(100);
    //Serial.println("hit");
    disTarget(num);
    delay(50);
  }
}
