/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Scene.h"
#include "GameObject.h"

TScene::TScene()
{

}
//-----------------------------------------------------------------------------
TScene::~TScene()
{

}
//-----------------------------------------------------------------------------
void TScene::UpdateFromPhysic()
{

}
//-----------------------------------------------------------------------------
void TScene::UpdateToGraphic() 
{

}
//-----------------------------------------------------------------------------
void TScene::UpdateToSound()
{

}
//-----------------------------------------------------------------------------
int TScene::GetCountUsing()
{
  return GetCount(mMapID_UsingGO);
}
//-----------------------------------------------------------------------------
TGameObject* TScene::GetUsingByIndex(int index)
{
  return GetByIndex(mMapID_UsingGO, index);
}
//-----------------------------------------------------------------------------
TGameObject* TScene::GetUsingByID(int id)
{
  return GetByID(mMapID_UsingGO, id);
}
//-----------------------------------------------------------------------------
int TScene::GetCountNotUsing()
{
  return GetCount(mMapID_NotUsingGO);
}
//-----------------------------------------------------------------------------
TGameObject* TScene::GetNotUsingByIndex(int index)
{
  return GetByIndex(mMapID_NotUsingGO, index);
}
//-----------------------------------------------------------------------------
TGameObject* TScene::GetNotUsingByID(int id)
{
  return GetByID(mMapID_NotUsingGO, id);
}
//-----------------------------------------------------------------------------
void TScene::Include(TGameObject* pGO)
{
  mMapID_UsingGO.insert(TMapIntPtrVT(pGO->GetID(), pGO));
  // ��������� ������� �������� � ��������� ������ �������
}
//-----------------------------------------------------------------------------
void TScene::Exclude(int id)
{
  mMapID_UsingGO.erase(id);// ����� �� �����
  mMapID_NotUsingGO.erase(id);// ��� �����
  // ��������� ������� �������� �� ���������� �������
}
//-----------------------------------------------------------------------------
void TScene::SetUsing(int id, bool v)
{
  bool res = false;
  if( v )
    res = Move(mMapID_NotUsingGO, mMapID_UsingGO,   id);
  else
    res = Move(mMapID_UsingGO,   mMapID_NotUsingGO, id);
  if( res )
  {
    // ��������� ������� �������� (???)
  }
}
//-----------------------------------------------------------------------------
bool TScene::GetUsing(int id)
{
  if( GetUsingByID(id) )
    return true;

  return false;
}
//-----------------------------------------------------------------------------
void TScene::Thread_Logic()
{
  // ������� �������������
}
//-----------------------------------------------------------------------------
void TScene::Thread_Ogre()
{

}
//-----------------------------------------------------------------------------
void TScene::Thread_Bullet()
{
  // ���������� �������������

  // �������� �� ��������� �������� � �������� �� ���������� ������� ����� ���������� "�����" ��������
}
//-----------------------------------------------------------------------------
void TScene::Thread_OpenAL()
{

}
//-----------------------------------------------------------------------------
bool TScene::Move( TMapIntPtr& fromMap, TMapIntPtr& toMap, int id)
{
  TMapIntPtrIt fitFrom = fromMap.find(id);
  if( fitFrom==fromMap.end() )
    return false;// ������ �� ����������
  TGameObject* pGO = fitFrom->second;
  fromMap.erase(id);
  toMap.insert(TMapIntPtrVT(id, pGO));
  return true;
}
//-----------------------------------------------------------------------------
int TScene::GetCount(TMapIntPtr& m)
{
  return m.size();
}
//-----------------------------------------------------------------------------
TGameObject* TScene::GetByIndex(TMapIntPtr& m, int index)
{
  if( index < 0 || index >= GetCountNotUsing() )
    return NULL;
  TMapIntPtrIt bit = m.begin();
  for( int i = 0 ; i < index ; i++ )
    bit++;
  return bit->second;
}
//-----------------------------------------------------------------------------
TGameObject* TScene::GetByID(TMapIntPtr& m, int id)
{
  TMapIntPtrIt fit = m.find(id);
  if( fit==m.end() )
    return NULL;
  return fit->second;
}
//-----------------------------------------------------------------------------
void TScene::RefreshCollision()
{

}
//-----------------------------------------------------------------------------