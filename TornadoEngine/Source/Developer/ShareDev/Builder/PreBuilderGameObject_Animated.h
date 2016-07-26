/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef PreBuilderGameObject_AnimatedH
#define PreBuilderGameObject_AnimatedH

#include "TypeDef.h"
#include "PreBuilderGameObject.h"

class DllExport TPreBuilderGameObject_Animated : public TPreBuilderGameObject
{
public:
  TPreBuilderGameObject_Animated();
  virtual ~TPreBuilderGameObject_Animated();

  virtual void SetObjectItem(TMapItem::TObject* pObjectItem);

  //virtual std::string GetStrDesc();
  //virtual TGameObject* GetGameObject();

  virtual bool GenerateTask(TVectorTypeTask& vecTypeTask);

  virtual void TakeTask_Ogre(TListTaskOgre& listOgre);
  virtual void TakeTask_Bullet(TListTaskBullet& listBullet);
  virtual void TakeTask_OpenAL(TListTaskOpenAL& listOpenAL);

};

#endif
