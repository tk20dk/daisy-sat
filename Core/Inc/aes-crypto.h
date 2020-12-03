#ifndef AES_CRYPTO_H__
#define AES_CRYPTO_H__

#include <crypto.h>


class TAesCrypto
{
public:
  static void TestCCM();
  static void TestCFB();

  static int32_t EncryptCCM(
    uint8_t*  HeaderMessage,
    uint32_t  HeaderMessageLength,
    uint8_t*  Plaintext,
    uint32_t  PlaintextLength,
    uint8_t  *AES128_Key,
    uint8_t  *InitializationVector,
    uint32_t  IvLength,
    uint8_t  *OutputMessage,
    int32_t  *OutputMessageLength,
    int32_t  *AuthenticationTAGLength );

  static int32_t DecryptCCM(
    uint8_t*  HeaderMessage,
    uint32_t  HeaderMessageLength,
    uint8_t*  Plaintext,
    uint32_t  PlaintextLength,
    uint8_t  *AES128_Key,
    uint8_t  *InitializationVector,
    uint32_t  IvLength,
    uint8_t  *OutputMessage,
    int32_t  *OutputMessageLength,
    int32_t   AuthenticationTAGLength );

  static int32_t EncryptCFB(
    uint8_t*  InputMessage,
    uint32_t  InputMessageLength,
    uint8_t  *AES128_Key,
    uint8_t  *InitializationVector,
    uint32_t  IvLength,
    uint8_t  *OutputMessage,
    uint32_t *OutputMessageLength );

  static int32_t DecryptCFB(
    uint8_t*  InputMessage,
    uint32_t  InputMessageLength,
    uint8_t  *AES128_Key,
    uint8_t  *InitializationVector,
    uint32_t  IvLength,
    uint8_t  *OutputMessage,
    uint32_t *OutputMessageLength );
};


#endif // AES_CRYPTO_H__
