#include "GPTaskManager.h"

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//structure des taches
GPTask::GPTask()
{
  this->task      = NULL;
  this->status    = GPTM_DELETE;
  this->interval  = 0;
  this->lastStart = 0;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
void GPTask::execTash()
{
  (*this->task)();
}
