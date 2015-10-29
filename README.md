# GPTaskManager pour Arduino
librairie Multi-tache / Ordonnanceur pour Arduino

Inclusion de la librairie:
```c++
#include <GPTaskManager.h>

//Création Task Manager
GPTaskManager *myOS = NULL;
```

exemple utilisation:
```c++
void setup()
{

  //creation gestionnaire de tache
  myOS = new GPTaskManager();
  
  //exemple jout de trois tache normal ( 500 ms, 1000ms, 2000ms )
  myOS->add( myTask1, 500 );
  myOS->add( myTask2, 1000 );
  myOS->add( myTask3, 2000 );
  
  //exemple ajout tache Event
  myOS->event( digitalRead, PIN_BUTTON_1, myEvent, GPTM_BUTTON );
  myOS->event( digitalRead, PIN_BUTTON_2, myEvent );
}

void loop()
{
  myOS->update();
}
```

Exemple de fonction tache qui sera éxécuté:
```c++
void myTask1()
{
}

```
# Methode:
```c++
 /*--- STATUS DES TACHES ---*/
 GPTM_NORMAL  /*Tache normal*/
 GPTM_PAUSE   /*Mettre en pause une tache*/
 GPTM_DELETE  /*Supprimer une tache*/
 GPTM_BUTTON  /*tache type button*/
  
 /*--- AJOUTE UNE TACHE --- */
    // userTask    : Fonction Tache name
    // taskInterval: Temps de relance de la fonction
    // taskStatus  : Mode execution -> GPTM_NORMAL, GPTM_PAUSE
    void add( void (*userTask)(void), const ulong taskInterval, const uchar taskStatus = GPTM_NORMAL );

    /*--- AJOUTE UN EVENT OUTPUT --- */
    // modeRead : digitalRead, analogicRead
    // pin      : numéro pin carte Arguino
    // userTask : Fonction Tache name
    // mode     : Mode de d'étection de changement -> GPTM_NORMAL, GPTM_BUTTON
    //pression  : Temps avant detection nouvelle appuis
      void event( int (*modeRead)(uint8_t), const uint8_t pin, void (*userTask)(void), const int mode = GPTM_NORMAL, const ulong taskInterval = 100 );

    /*--- MODIFIER LE TEMPS DE RELANCE DE LA TACHE --- */
    // userTask    : Fonction Tache name
    // taskInterval: Temps de relance de la fonction
    void updateInterval( void (*userTask)(void), const ulong taskInterval );
    /*--- MODIFIER LE STATUS DE LA TACHE --- */
    // userTask  : Fonction Tache name
    // taskStatus: Mode execution -> GPTM_NORMAL, GPTM_PAUSE, GPTM_DELETE
    void updateStatus( void (*userTask)(void), const uchar taskStatus  );

    /*--- MODIFIER LA FONCTION CALLBACK DE LA TACHE --- */
    // userTask : Fonction Tache name
    // newTask  : Fonction Tache name
    void updateTask(void (*userTask)(void), void (*newTask)(void) );

    /*--- LANCE LA FONCTION CALLBACK D'UNE TACHE --- */
    // userTask : Fonction Tache name
    void exeTask(void (*userTask)(void) );
    
    /*--- RESET TOUTES LES TACHES --- */
    void resetTask();
    
    /*--- RESET LE TIMER DES TACHES --- */
    void resetTimer();
    
    /*--- VERIFIE SI LA TACHE EXISTE --- */
    bool existTask( void (*userTask)(void) );
    
    /*--- RENVOI LA RAM DISPONIBLE --- */
    static int freeRam();
```
