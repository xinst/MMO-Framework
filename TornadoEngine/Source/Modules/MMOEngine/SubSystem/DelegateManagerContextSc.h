/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef DELEGATE_MANAGER_CONTEXT_SC_H
#define DELEGATE_MANAGER_CONTEXT_SC_H

namespace nsMMOEngine
{
	class TContainerContextSc;
	class TBase;
  class TDelegateManagerContextSc
  {
    TBase* mBase;
  public:
    TDelegateManagerContextSc(TBase* pBase);
    virtual ~TDelegateManagerContextSc();

	protected:
		TContainerContextSc* AddContainer();
		void DeleteContainer(TContainerContextSc* pC);
  };
}

#endif

