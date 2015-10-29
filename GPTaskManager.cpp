#include "GPTaskManager.h"
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------

//Constructeur
GPTaskManager::GPTaskManager()
{
  //nombre tache
  this->nb_task = 0;

  #ifndef GPTM_TIME_REEL
    //temp de pause
    this->time_pause = -1;
    this->time_start = millis();
  #endif

  #ifdef DEBUG
    Serial.print("Task Manager Start\n");
  #endif
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Destructeur
GPTaskManager::~GPTaskManager()
{
  this->removeAllTask();
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//ajouter une tache
void GPTaskManager::add( void (*userTask)(void), const ulong taskInterval, const uchar taskStatus )
{
  GPTask *buffer = new GPTask();
  buffer->task     = userTask;
  buffer->interval = taskInterval;
  buffer->status   = taskStatus;

  //ajout du GPTask
  this->addTask( buffer );
}


//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Ajout evenenement OUTPUT
#ifdef GPTM_EVENT
  void GPTaskManager::event( int (*modeRead)(uint8_t), const uint8_t pin, void (*userTask)(void), const int mode, const ulong taskInterval )
  {
     if( modeRead == digitalRead )
        pinMode( pin, OUTPUT );

      GPTaskEvent *buffer = new GPTaskEvent();
      buffer->task     = userTask;
      buffer->status   = GPTM_NORMAL;
      buffer->interval = taskInterval;

      buffer->pin      = pin;
      buffer->capt     = modeRead;
      buffer->event_default = (*modeRead)(pin);
      buffer->mode     = mode;

      #ifdef DEBUG
        Serial.print("New Event\n");
      #endif
      //ajout du GPTask
      this->addTask( buffer );
  }
#endif


//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//mettre a jour la tache
void GPTaskManager::updateInterval( void (*userTask)(void), const ulong taskInterval )
{
  struct GPTask* buffer = this->getTask( userTask );
  buffer->interval  = taskInterval;
  buffer->lastStart = millis();

  #ifndef GPTM_TIME_REEL
    //temps mini d'un update
    if( this->time_pause > taskInterval )
      this->time_pause = taskInterval;
  #endif
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//mettre a jour le statue du pressesus
void GPTaskManager::updateStatus( void (*userTask)(void)  , const uchar taskStatus )
{
  if( taskStatus == GPTM_DELETE )
  {
    this->removeTask( userTask );
    return;
  }
  struct GPTask* buffer = this->getTask( userTask );
  buffer->status    = taskStatus;
  buffer->lastStart = millis();
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//modifier la fonction de callback de la tache
void GPTaskManager::updateTask(void (*userTask)(void), void (*newTask)(void) )
{
  struct GPTask* buffer = this->getTask( userTask );
  buffer->task = newTask;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//lance une tache
void GPTaskManager::exeTask(void (*userTask)(void) )
{
  struct GPTask* buffer = this->getTask( userTask );
  buffer->execTash();
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//mise a jour du manager
void GPTaskManager::update()
{
  ulong exec_time  = 0;
  //ulong time_start = millis();
  ulong i          = 0;
  struct GPTask* buffer    = NULL;

  //executé les taches
  while( i < this->nb_task )
  {
    buffer    = this->task[ i ];
    exec_time = buffer->interval + buffer->lastStart;

    if( buffer->status == GPTM_NORMAL )
    if( exec_time <= this->time_start )
    {
       //time exec fonction
       buffer->lastStart = exec_time;

       //lancer la tache
       buffer->execTash();
    }
    i++;
  }

  //pause du programme
  #ifndef GPTM_TIME_REEL
    i = this->time_pause-(millis()-this->time_start);
    if( i > 10 )
        delay( i );
    this->time_start = millis();
  #endif
}


//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
/*Supprimer toute les taches*/
void GPTaskManager::resetTask()
{
  this->removeAllTask();
}


//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
/* reset du timer d'execution */
void GPTaskManager::resetTimer()
{
  uchar i=0;
  ulong time = millis();
  while( i < this->nb_task )
  {
    this->task[ i ]->lastStart = time;
    i++;
  }
  #ifdef DEBUG
    Serial.print("Timer reset !\n");
  #endif
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
void GPTaskManager::addTask( struct GPTask *task)
{
  uchar i=0;

  struct GPTask **copie = this->task;
  this->task = new struct GPTask*[ this->nb_task+1 ];

  while( i < this->nb_task )
  {
    this->task[ i ] = copie[ i ];
    i++;
  }
  this->task[ i ] = task;
  this->nb_task++;

  #ifndef GPTM_TIME_REEL
    //temps mini d'un update
    if( this->time_pause > task->interval )
      this->time_pause = task->interval;
  #endif

  #ifdef DEBUG
    Serial.print("Add Task\n");
  #endif
}


//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
struct GPTask* GPTaskManager::getTask( void (*userTask)(void) )
{
  uchar i=0;

  while( i < this->nb_task )
  {
    if( userTask == (this->task[ i ]->task) )
      return this->task[ i ];
    i++;
  }
  return NULL;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
void GPTaskManager::removeTask( void (*userTask)(void) )
{
  //verifie si la tache existe
  if( this->existTask(userTask) )
    return;

  //init variable
  struct GPTask **copie = this->task;
  uchar i=0;
  uchar p=0;

  //suppression tache
  this->nb_task--;
  this->task = new struct GPTask*[ this->nb_task ];
  while( i < this->nb_task )
  {
    if( userTask == copie[ i ]->task )
    {
      delete copie[ i ];
      p++;
    }
    this->task[ i ] = copie[ p ];
    i++;
    p++;
  }

  #ifdef DEBUG
    Serial.print("Remove Task\n");
  #endif
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
void GPTaskManager::removeAllTask()
{
  uchar i=0;
  while( i < this->nb_task )
  {
    delete this->task[ i ];
    i++;
  }
  this->nb_task = 0;
  delete this->task;

  #ifdef DEBUG
    Serial.print("Remove All Task\n");
  #endif
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
/* vérifie si une tache existe */
bool GPTaskManager::existTask( void (*userTask)(void) )
{
  uchar i=0;
  while( i < this->nb_task )
  {
    if( userTask == this->task[ i ]->task )
      return false;
     i++;
  }
  return true;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
int GPTaskManager::freeRam()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
