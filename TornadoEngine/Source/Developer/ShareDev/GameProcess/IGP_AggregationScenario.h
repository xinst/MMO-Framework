/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IGP_AggregationScenarioH
#define IGP_AggregationScenarioH

#include "GP_TypeScenario.h"
#include "IGP_Scenario_General.h"

class IGP_Scenario_General;
class TGP_Scenario;

class IGP_AggregationScenario : public virtual IGP_Scenario_General
{
protected:
  // для смены указателя только в потоке Логики
  IGP_Scenario_General* mPtrCurrentScenario_General;
  TGP_Scenario*         mPtrCurrentScenario_Scenario;
public:
  IGP_AggregationScenario();
  virtual ~IGP_AggregationScenario();

  virtual void Thread_Logic();
  virtual void Thread_Bullet();
  virtual void Thread_Ogre();
  virtual void Thread_OpenAL();

  nsGameProcess::GP_TypeScenario GetCurrentScenarioType();// L
  
  virtual bool Activate(nsGameProcess::GP_TypeScenario type);// L
protected:
  virtual void GetByType(nsGameProcess::GP_TypeScenario type, 
    IGP_Scenario_General*& pGeneral, TGP_Scenario*& pScenario) = 0;

  void Begin( IGP_Scenario_General* pNew_General, TGP_Scenario* pNew_Scenario );
  void End();

  IGP_Scenario_General* GetCurrentScGeneral();
  TGP_Scenario*         GetCurrentSc();

  void RegisterScenarioOnEvent(IGP_Scenario_General* pSc);
  
  bool IsActive(nsGameProcess::GP_TypeScenario type);
private:
  void EndEventScenario(nsGameProcess::GP_TypeScenario type);
  void ProgressEventScenario(nsGameProcess::GP_TypeScenario type, int progress);
};

#endif 
