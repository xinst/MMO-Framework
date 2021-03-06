/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "CryptMITM.h"
#include "CryptoAES_Impl.h"
#include "BreakPacket.h"
#include "MD5.h"

bool TCryptMITM::Calc(void* rsa, int size_rsa, 
											void* pLogin, int sizeLogin,
                      void* pPassword, int sizePassword, 
											TContainer& c_result)
{
  TContainer cAES_RSA;
  // зашифровать публичный ключ RSA с помощью AES, используя в качестве ключа пароль
  TContainer cMD5_Password;
  TMD5 md5Password;
  md5Password.FastCalc(pPassword, sizePassword, cMD5_Password);
  TCryptoAES_Impl crypt_aes;
  crypt_aes.SetPublicKey(cMD5_Password.GetPtr(), cMD5_Password.GetSize());
  if(crypt_aes.Encrypt(rsa, size_rsa, cAES_RSA)==false)
    return false;

  TBreakPacket bp;
  // зашифрованный RSA
  bp.PushFront(cAES_RSA.GetPtr(), cAES_RSA.GetSize());
  // Login
  bp.PushFront((char*)pLogin, sizeLogin);
  // длина логина
  char lenLogin = sizeLogin;
  bp.PushFront(&lenLogin, sizeof(lenLogin));
  // собрать
  bp.Collect();
  // отдать собранный пакет
  c_result.Entrust((char*)bp.GetCollectPtr(), bp.GetSize());
  // отцепиться
  bp.UnlinkCollect();
	return true;
}
//-------------------------------------------------------------------------------------------------------