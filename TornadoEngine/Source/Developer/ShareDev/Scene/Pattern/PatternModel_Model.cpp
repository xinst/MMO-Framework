/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "PatternModel_Model.h"
#include <boost/foreach.hpp>
#include <OgreRoot.h>
#include "ModuleLogic.h"
#include "ModuleGraphicEngine.h"
#include "FactoryGameItem.h"

#include "ShapeItem.h"

namespace nsPatternModel_Model
{
  const char* sNameGameItem         = "NameGameItem";
  const char* sVariantPatternConfig = "VariantPatternConfig";
  const char* sMobility             = "Mobility";
}

using namespace nsPatternModel_Model;

TPatternModel_Model::TPatternModel_Model()
{
  mDefaultParameterMap.insert(TMapItem::TMapStrStrVT(sNameGameItem,""));
  mDefaultParameterMap.insert(TMapItem::TMapStrStrVT(sVariantPatternConfig,""));
  mDefaultParameterMap.insert(TMapItem::TMapStrStrVT(sMobility,""));
}
//---------------------------------------------------------------------------
TPatternModel_Model::~TPatternModel_Model()
{

}
//---------------------------------------------------------------------------
TBehaviourPatternContext* TPatternModel_Model::MakeNewConext()
{ 
  return new TPatternContext_Model(this);
}
//---------------------------------------------------------------------------
void TPatternModel_Model::GetDefaultParameterMap(TMapItem::TMapStrStr& m)
{
  m = mDefaultParameterMap;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::SetParameterFromPattern(TContainer c)
{
  return true;
}
//---------------------------------------------------------------------------
TContainer TPatternModel_Model::GetParameterToPattern()
{
  TContainer c;
  return c;
}
//---------------------------------------------------------------------------
void TPatternModel_Model::SetPosition(nsMathTools::TVector3& v)
{
  // подготовить задание для физики
  nsMathTools::TVector3* pV = new nsMathTools::TVector3(v);
  mPipePositionLogic2Bullet.Add(pV);
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::GetPosition(nsMathTools::TVector3& v)
{
  return GetFromPipe(v, &mPipePositionLogic2Bullet);
}
//---------------------------------------------------------------------------
void TPatternModel_Model::SetOrientation(nsMathTools::TVector3& v)
{
  // подготовить задание для физики
  nsMathTools::TVector3* pV = new nsMathTools::TVector3(v);
  mPipeOrientationLogic2Bullet.Add(pV);
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::GetOrientation(nsMathTools::TVector3& v)
{
  return GetFromPipe(v, &mPipeOrientationLogic2Bullet);
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::GetFromPipe(nsMathTools::TVector3& v, TDataExchange2Thread<nsMathTools::TVector3>* pPipe)
{
  nsMathTools::TVector3** ppV = pPipe->GetFirst();
  if( ppV==NULL )
    return false;
  while(1)
  {
    v = *(*ppV);
    pPipe->RemoveFirst();
    ppV = pPipe->GetFirst();
    if( ppV==NULL )
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::LoadFromParameterMap()
{
  return true;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::UpdateFromGameItem(TBaseItem* pBI)
{
  return true;
}
//---------------------------------------------------------------------------
bool TPatternModel_Model::Unload()
{
  return true;
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadFromThread_Logic(TBehaviourPatternContext* pContext)
{
  TPatternContext_Model* pContextModel = (TPatternContext_Model*)pContext;
  // найти имя модели
  std::string nameGameItem = pContextModel->GetNameGameItem();
  if( nameGameItem.length()==0 )
  {
    TMapItem::TMapStrStrConstIt itNameGameItem = pContext->GetParameterMap()->find(sNameGameItem);
    if( itNameGameItem==pContext->GetParameterMap()->end() )
      return;
    nameGameItem = itNameGameItem->second;
    pContextModel->SetNameGameItem(nameGameItem);
  }
  // найти вариант настроек паттерна
  std::string variantPatternConfig = pContextModel->GetNameVariantPatternConfig();
  if( variantPatternConfig.length()==0 )
  {
    TMapItem::TMapStrStrConstIt itVariant = pContext->GetParameterMap()->find(sVariantPatternConfig);
    if( itVariant!=pContext->GetParameterMap()->end() )// если нет настройки, ничего не делать
    {
      variantPatternConfig = itVariant->second;
      pContextModel->SetNameVariantPatternConfig(variantPatternConfig);
    }
  }
  // найти будет ли модель мобильной
  TMapItem::TMapStrStrConstIt itMobility = pContext->GetParameterMap()->find(sMobility);
  if( itMobility!=pContext->GetParameterMap()->end() )// если нет, то ничего не делать
  {
    bool mobility = itMobility->second=="true" ? true : false;
    pContextModel->SetMobility(mobility);
  }
  TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();

  TModelItem* pModel = (TModelItem*)pFGI->Get(TFactoryGameItem::Model, nameGameItem);
  if( pModel==NULL )
    return;
  
  TPatternConfigItem* pPattern = (TPatternConfigItem*)pFGI->Get(TFactoryGameItem::PatternConfig, GetName());
  if( pPattern )
  {
    TPatternConfigItem::TMapStrMapIt itMapVariant = pPattern->mMapVariant.find(variantPatternConfig);
    if( itMapVariant!=pPattern->mMapVariant.end() )
      pContextModel->SetMapVariant(itMapVariant->second);// использовать в конце загрузки в каждом из потоков (графика,звук,физика)
  }

  // задача: создать модели по имени. При синхронизации менять положение и ориентацию форм или моделей
  pContextModel->SetTypeContent(pModel->mTypeCollection);
  if( pModel->mTypeCollection==TModelItem::eModel )
    LoadModelsFromThread_Logic(pContextModel, pModel->mMapNamePart);
  else
    LoadShapesFromThread_Logic(pContextModel, pModel->mMapNamePart);
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadFromThread_Ogre(TBehaviourPatternContext* pContext, bool fast)
{
  TPatternContext_Model* pContextModel = (TPatternContext_Model*)pContext;
  int cntPart = pContextModel->GetCountPart();
  for( int iPart = 0 ; iPart < cntPart ; iPart++ )
  {
    std::string namePart = pContextModel->GetNamePart(iPart);
    int cntVariant = pContextModel->GetCountVariant(namePart);
    for( int iVariant = 0 ; iVariant < cntVariant ; iVariant++ )
    {
      std::string nameVariant = pContextModel->GetNameVariant(namePart, iVariant);
      TPatternContext_Model::TBaseDesc* pDesc = pContextModel->GetDesc(namePart, nameVariant);
      if( pDesc==NULL )
        continue;
      if( pDesc->type==TModelItem::eModel )
      {
        TPatternContext_Model::TModelDesc* pModelDesc = (TPatternContext_Model::TModelDesc*)pDesc;
        pModelDesc->pCtxModel->GetModel()->LoadFromThread_Ogre(pModelDesc->pCtxModel);
      }
      else
      {
        TPatternContext_Model::TShapeDesc* pShapeDesc = (TPatternContext_Model::TShapeDesc*)pDesc;
        LoadShapeFromThread_Ogre(pContextModel, pShapeDesc);
      }
    }
  }
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadFromThread_Bullet( TBehaviourPatternContext* pContext, bool fast )
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadFromThread_OpenAL( TBehaviourPatternContext* pContext, bool fast )
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::UnloadFromThread_Logic(TBehaviourPatternContext* pContext)
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::UnloadFromThread_Ogre( TBehaviourPatternContext* pContext, bool fast )
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::UnloadFromThread_Bullet( TBehaviourPatternContext* pContext, bool fast )
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::UnloadFromThread_OpenAL( TBehaviourPatternContext* pContext, bool fast )
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::SynchroFromThread_Logic(TBehaviourPatternContext* pContext)
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::SynchroFromThread_Ogre(TBehaviourPatternContext* pContext)
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::SynchroFromThread_Bullet(TBehaviourPatternContext* pContext)
{
  // проверка на изменение позиции и ориентации
}
//---------------------------------------------------------------------------
void TPatternModel_Model::SynchroFromThread_OpenAL(TBehaviourPatternContext* pContext)
{

}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadModelsFromThread_Logic(TPatternContext_Model* pContextModel, TModelItem::TMapStrPart& mapNamePart)
{
  TFactoryBehaviourPatternModel* pFBPM = TModuleLogic::Get()->GetFBPM();
  TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();
  // создать другие модели и сохранить в контексте
  BOOST_FOREACH( TModelItem::TMapStrPartVT& vtPart, mapNamePart )
  {
    std::string namePart = vtPart.first;
    BOOST_FOREACH( TModelItem::TVariant& variant, vtPart.second.vecVariant )
    {
      TModelItem* pModel = (TModelItem*)pFGI->Get( TFactoryGameItem::Model, variant.nameItem);
      if( pModel==NULL )
        continue;
      TBehaviourPatternModel* pBPM = pFBPM->GetPatternByName(pModel->mNamePattern);
      if( pBPM==NULL )
        continue;

      TPatternContext_Model::TModelDesc* pModelDesc = new TPatternContext_Model::TModelDesc;
      pModelDesc->namePart    = namePart;
      pModelDesc->nameVariant = variant.name;
      pModelDesc->pCtxModel = (TPatternContext_Model*)pBPM->MakeNewConext();
      pModelDesc->pCtxModel->SetNameGameItem(variant.nameItem);
      pModelDesc->pCtxModel->SetMobility(pContextModel->GetMobility());// мобильность наследуется
      pContextModel->AddDesc(pModelDesc);

      pBPM->LoadFromThread_Logic(pModelDesc->pCtxModel);// дальше по итерации
    }
  }
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadShapesFromThread_Logic(TPatternContext_Model* pContextModel, TModelItem::TMapStrPart& mapNamePart)
{
  TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();
  
  BOOST_FOREACH( TModelItem::TMapStrPartVT& vtPart, mapNamePart )
  {
    std::string namePart = vtPart.first;
    BOOST_FOREACH( TModelItem::TVariant& variant, vtPart.second.vecVariant )
    {
      // передача данных от Логики к Физике и Графике для каждого потока ( создание формы )
      TShapeItem* pShapeItem = (TShapeItem*)pFGI->Get( TFactoryGameItem::Shape, variant.nameItem);
      if( pShapeItem==NULL )
        continue;

      TPatternContext_Model::TShapeDesc* pShape = new TPatternContext_Model::TShapeDesc;
      pShape->namePart      = namePart;
      pShape->nameVariant   = variant.name;
      pShape->nameShapeItem = variant.nameItem;
      if( variant.redefinitionMaterial.length() )
        pShape->nameMaterial = variant.redefinitionMaterial;
      else
        pShape->nameMaterial = pShapeItem->mNameMaterial;
      pContextModel->AddDesc(pShape);
    }
  }
}
//---------------------------------------------------------------------------
void TPatternModel_Model::LoadShapeFromThread_Ogre(TPatternContext_Model* pContextModel, 
                                                   TPatternContext_Model::TShapeDesc* pShapeDesc)
{
  TFactoryGameItem* pFGI = TModuleLogic::Get()->GetFGI();
  TShapeItem* pShapeItem = (TShapeItem*)pFGI->Get(TFactoryGameItem::Shape,pShapeDesc->nameShapeItem);
  if( pShapeItem==NULL )
    return;

  TBuilder_Ogre* pBOgre = GetBuilderOgre();

  Ogre::MeshPtr pMesh = pBOgre->GetShapeMaker()->Build( pShapeItem );
  //pMesh->
  // доступ
  //Ogre::MeshManager::getSingleton().getByName( const String& name ); 	
  Ogre::Root* pRoot = TModuleLogic::Get()->GetC()->pGraphicEngine->GetGE()->GetRoot();

}
//---------------------------------------------------------------------------
