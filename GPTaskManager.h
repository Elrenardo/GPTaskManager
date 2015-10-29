//-------------------------------------------------------
//-------------------------------------------------------
/*
NAME    : Task Manager Class for Arduino
BY      : Teysseire Guillaume
TIME    : 08/11/2014
UPDA    : 18/11/2014
WEB     : studiogoupil.fr
VERSION : 2.50
*/
//-------------------------------------------------------
//-------------------------------------------------------
#ifndef GPTM_TASK_MANAGER
#define GPTM_TASK_MANAGER

#include <Arduino.h>
/*----------------------------------------------*/
/*----------------------------------------------*/
/*--- Activation mode TEMPS REEL ---*/
//#define GPTM_TIME_REEL

/*--- Mode Event ---*/
#define GPTM_EVENT

/*--- Mode DEBUG ---*/
//#define DEBUG

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
/*--- Definition des statues du task manager ---*/
#define GPTM_NORMAL 1
#define GPTM_PAUSE 2
#define GPTM_DELETE 0

#ifdef GPTM_EVENT
  #define GPTM_BUTTON 4
#endif

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
class GPTask;
#ifdef GPTM_EVENT
  class GPTaskEvent;
#endif


//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------

/*Fonction de gestion du multi-tache sur carte Arduino*/
class GPTaskManager
{
  public:
    GPTaskManager();
    ~GPTaskManager();

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
    #ifdef GPTM_EVENT
      void event( int (*modeRead)(uint8_t), const uint8_t pin, void (*userTask)(void), const int mode = GPTM_NORMAL, const ulong taskInterval = 100 );
    #endif


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


    /*--- ACTUALISE LE TACHE MANAGER --- */
    void update();



    /*--- AUTRE --- */
    void resetTask();
    void resetTimer();
    bool existTask( void (*userTask)(void) );
    static int freeRam();



  private:
    #ifndef GPTM_TIME_REEL
      unsigned long time_pause;//temp mini de pause entre chaque tache
      ulong time_start;
    #endif

    unsigned int nb_task;
    struct GPTask **task;   //tableau des tache en cours

  protected:
    //fonction private
    void           addTask( struct GPTask *task);
    struct GPTask* getTask( void (*userTask)(void) );
    void           removeTask( void (*userTask)(void) );
    void           removeAllTask();
};

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
class GPTask
{
  public:
    GPTask();
    virtual void execTash();

    void (*task)(void);
    uchar status;
    ulong interval;
    ulong lastStart;
};
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
#ifdef GPTM_EVENT
class GPTaskEvent: public GPTask
{
  public:
    GPTaskEvent();
    void execTash();

    int (*capt)(uint8_t);
    uint8_t pin;
    uint event_default;
    uchar mode;
};
#endif

#endif
