//-------------------------------------------------------
/*
NAME: 
BY     : Teysseire Guillaume
TIME   : XX/XX/20XX
UPDA   : XX/XX/20XX
VERSION:
WEB    : studiogoupil.fr
*/
//-------------------------------------------------------

#include <GPTaskManager.h>
//Port Serial
#define PORT_SERIAL 115200
//Création Task Manager
GPTaskManager *myOS = NULL;

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Fonction inisialisation
void setup()
{
  Serial.begin( PORT_SERIAL );
  //creation gestionnaire de tache
  myOS = new GPTaskManager();
  
  //TODO
  
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
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

