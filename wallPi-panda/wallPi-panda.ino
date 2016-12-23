#include <Arduino.h>
#include <Chrono.h>

#define BUTTON_PIN 10//TODO
#define BUTTON_LED_PIN 11//TODO

#define TARGET1_PIN 10//TODO
#define TARGET2_PIN 11//TODO
#define TARGET3_PIN 11//TODO
#define TARGET4_PIN 11//TODO
#define TARGET5_PIN 11//TODO

#define TARGET1_LED_PIN 10//TODO
#define TARGET2_LED_PIN 11//TODO
#define TARGET3_LED_PIN 11//TODO
#define TARGET4_LED_PIN 11//TODO
#define TARGET5_LED_PIN 11//TODO

#define MAX_PANDA_GAME 10
#define MIN_PANDA_GAME 3
bool enabled_game=false;
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

Serial.begin(9600);
while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
}

pinMode(BUTTON_PIN, INPUT_PULLUP);
pinMode(TARGET1_PIN, INPUT_PULLUP);
pinMode(TARGET2_PIN, INPUT_PULLUP);
pinMode(TARGET3_PIN, INPUT_PULLUP);
pinMode(TARGET4_PIN, INPUT_PULLUP);
pinMode(TARGET5_PIN, INPUT_PULLUP);

pinMode(BUTTON_LED_PIN, OUTPUT);
pinMode(TARGET1_LED_PIN, OUTPUT);
pinMode(TARGET2_LED_PIN, OUTPUT);
pinMode(TARGET3_LED_PIN, OUTPUT);
pinMode(TARGET4_LED_PIN, OUTPUT);
pinMode(TARGET5_LED_PIN, OUTPUT);

digitalWrite(BUTTON_LED_PIN, HIGH);
digitalWrite(TARGET1_LED_PIN, LOW);
digitalWrite(TARGET2_LED_PIN, LOW);
digitalWrite(TARGET3_LED_PIN, LOW);
digitalWrite(TARGET4_LED_PIN, LOW);
digitalWrite(TARGET5_LED_PIN, LOW);


}

void loop()
{
  if(enabled_game = false)
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
        }
      }
  }
  else
  {
    digitalWrite(BUTTON_LED_PIN, HIGH);
  }

  if(enabled_game = true)
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
  for(int i=0; i<5; i++)
  {
    digitalWrite(BUTTON_LED_PIN, HIGH);
    delay(100);
    digitalWrite(BUTTON_LED_PIN, LOW);
    delay(100);
  }
}

int pandaGame()
{
  switch (panda_stage)
  {
    case 0:
      target_index = 0;
      hitpoints = 0;
      targetAttempts = 0;
      for(int i=0; i<5 ; i++)
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
      if (gameTimer.hasPassed(120))
      {
        panda_stage = 4;
        break;
      }

      if (targetTimer.hasPassed(1000)) //change target on 1,4s?
      {
        disTarget(target_index);
        targetAttempts++;
        targetTimer.restart();
        target_index++;
        enTarget(target_index);

        targetAttempts++;
        if (target_index > 7)
          target_index = 0;

        panda_stage = 2;
      }
      else

        if (readTarget(target_index))
        {
          delay(200);
              if (readTarget(target_index))
            {
              disTarget(target_index);
              targetTimer.restart();
              target_index++;

              if (target_index > 7)
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
      result= MAX_PANDA_GAME;


      Serial.print("hitpoints: ");
      Serial.println(hitpoints);
      Serial.print("Target Attempts: ");
      Serial.println(targetAttempts);
      for (int i = 0; i < 10; i++)
      {
        disTarget(i);
      }


      return 1;
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
case 0: return digitalRead(TARGET1_LED_PIN); break;
case 1: return digitalRead(TARGET2_LED_PIN); break;
case 2: return digitalRead(TARGET3_LED_PIN); break;
case 3: return digitalRead(TARGET4_LED_PIN); break;
case 4: return digitalRead(TARGET5_LED_PIN); break;
}
return 0;
}
