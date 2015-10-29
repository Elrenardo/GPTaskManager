//-------------------------------------------------------
/*
NAME: Task Manager
BY  : Teysseire Guillaume
TIME: 08/11/2014
UPDA: 11/11/2014
*/
//-------------------------------------------------------
#include <GPTaskManager.h>

//Création Task Manager
GPTaskManager *myOS = NULL;

#define PORT_SERIAL 115200
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
#define PIN_LED_RED 2
#define PIN_LED_YELLOW 3
#define PIN_LED_GREEN 4

#define PIN_BUTTON_1 5
#define PIN_BUTTON_2 6

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
bool switchLed( bool *status )
{
  if( *status == true )
  {
    *status = false;
    return LOW;
  }
  *status = true;
  return HIGH;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
bool led_red_status = false;
void myTask1()
{
  //Serial.print("Ma Fonction 1\n");
  digitalWrite(PIN_LED_RED, switchLed( &led_red_status ));
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
bool led_yellow_status = false;
void myTask2()
{
  //Serial.print("Ma Fonction 2\n");
  digitalWrite(PIN_LED_YELLOW, switchLed( &led_yellow_status ));
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
bool led_green_status = false;
void myTask3()
{
  //Serial.print("Ma Fonction 3\n");
  digitalWrite(PIN_LED_GREEN, switchLed( &led_green_status ));
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
bool event_led_green_sprint = true;
void myEvent()
{
  if( event_led_green_sprint == true)
  {
    event_led_green_sprint = false;
    myOS->updateInterval( myTask3, 100 );
  }
  else
  {
    event_led_green_sprint = true;
    myOS->updateInterval( myTask3, 2000 );
  }
  //Serial.print("Ma fonction event 0\n");
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Fonction inisialisation
void setup()
{
  Serial.begin( PORT_SERIAL );
  Serial.print("Debut ON\n");
  
  pinMode( PIN_LED_RED, OUTPUT );
  pinMode( PIN_LED_YELLOW, OUTPUT );
  pinMode( PIN_LED_GREEN, OUTPUT );
  
  //creation gestionnaire de tache
  myOS = new GPTaskManager();
  //Ajout de trois tache normal
  myOS->add( myTask1, 500 );
  myOS->add( myTask2, 1000 );
  myOS->add( myTask3, 2000 );
  
  //ajout tache Event
  myOS->event( digitalRead, PIN_BUTTON_1, myEvent, GPTM_BUTTON );
  myOS->event( digitalRead, PIN_BUTTON_2, myEvent );
}

//Fonction update
void loop()
{
  myOS->update();
  //Serial.print( GPTaskManager::freeRam() );
  //Serial.print("\n");
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------


