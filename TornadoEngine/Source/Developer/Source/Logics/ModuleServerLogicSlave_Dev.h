/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ModuleServerLogicSlave_DevH
#define ModuleServerLogicSlave_DevH

#include "ModuleServerLogic.h"
#include "BaseEvent.h"
#include "Events.h"
#include "SlaveForm.h"

class TModuleServerLogicSlave_Dev : public TModuleServerLogic
{
  TSlaveForm* mSlaveForm;

  enum
  {
    eCycleTime = 100,
  };

  TListMultiThread<unsigned int> mListSessionAdd;

  TListMultiThread<TSlaveForm::TDesc> mListID_SessionAdd;
  TListMultiThread<TSlaveForm::TDesc> mListID_SessionDelete;

  TListMultiThread<unsigned int> mListSessionRestore;
  TListMultiThread<unsigned int> mListSessionSave;
public:
  TModuleServerLogicSlave_Dev();
  virtual void InitLog();

  virtual void StartEvent();
  virtual void StopEvent();
protected:
  virtual bool WorkServer();
  virtual void EndWork();

  virtual void Input(int id, void* p, int size);
private:
  void HandleFromMMOEngine(nsMMOEngine::TBaseEvent* pBE);
private:
  void StartTimer();
  void InitForms();
  void ConnectToMaster();

  void ConnectUpQt();
  void DisconnectUpQt();
  void AddClientQt();
  void DeleteClientQt();

  void ConnectDownMMOEngine();
  void SaveContextClientMMOEngine();

  void ConnectUp(nsMMOEngine::TEventConnectUp* pBE);
  void DisconnectUp(nsMMOEngine::TEventDisconnectUp* pBE);

  void ConnectDown(nsMMOEngine::TEventConnectDown* pEvent);
  void DisconnectDown(nsMMOEngine::TEventDisconnectDown* pEvent);

  void SaveContextClient(nsMMOEngine::TEventSaveContext* pEvent);
  void RestoreContextClient(nsMMOEngine::TEventRestoreContext* pEvent);
};

#endif
