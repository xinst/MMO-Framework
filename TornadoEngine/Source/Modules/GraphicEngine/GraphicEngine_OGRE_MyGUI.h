/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef GraphicEngine_OGRE_MyGUIH
#define GraphicEngine_OGRE_MyGUIH

#include "TypeDef.h"
#include "SrcEvent.h"

#include <boost/smart_ptr/scoped_ptr.hpp>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <MyGUI_Exception.h>

class TGE_Impl;

#define STR_NAME_GRAPHIC_ENGINE "GE"

/*
  Задачи класса: организация использования кванта времени (через Work()),
  выдача событий клавиатуры и мыши (те что не были поглощены GUI), 
  отрисовка окна GUI, 
  давать возможность управления камерой, 
  добавление и изменение объектов.
*/

class DllExport TGraphicEngine_OGRE_MyGUI : 
  public TSrcEvent /*for generation keyboard and mouse events(unused by GUI)*/
{
public:
  TGraphicEngine_OGRE_MyGUI();
  virtual ~TGraphicEngine_OGRE_MyGUI();
/* Order of calls:
   1. InitOGRE, 2. AddResource, 3. InitMyGUI */
	bool InitOGRE(const std::string& pathPluginCfg);
  void AddResource(const std::string& name, const std::string& type);
  bool InitMyGUI(const std::string& nameFileCore, const std::string& nameFileSkin);
  // return false - need exit
	bool Work();

  void SetWindowCaption(const std::wstring& _text);
  size_t GetWindowHandle();

  Ogre::Root*         GetRoot();
  Ogre::SceneManager* GetSceneManager();
  Ogre::Camera*       GetCamera();
  Ogre::RenderWindow* GetWindow();
private:
	boost::scoped_ptr<TGE_Impl> mGE;
  void MsgException(MyGUI::Exception& _e);
};

#endif