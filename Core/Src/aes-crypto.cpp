#include "aes-crypto.h"
#include <memory.h>


void TAesCrypto::TestCCM()
{
  uint32_t const TAG_LENGTH = 4;
  uint32_t const PLAINTEXT_LENGTH = 4;
  uint32_t const CIPHER_TEXT_LENGTH = PLAINTEXT_LENGTH + TAG_LENGTH;

  uint8_t const HeaderMessage[] =
  {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07
  };
  uint32_t const HeaderLength = sizeof( HeaderMessage );

  // Payload message, will be authenticated and encrypted
  uint8_t const Plaintext[] =
  {
    0x20,0x21,0x22,0x23
  };
  uint32_t const InputLength = sizeof( Plaintext );

  // Key to be used for AES encryption/decryption
  uint8_t Key[ CRL_AES128_KEY ] =
  {
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
    0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f
  };

  // Initialization Vector, used only in non-ECB modes
  uint8_t IV[] =
  {
    0x10,0x11,0x12,0x13,0x14,0x15,0x16
  };

  // NIST example 1 ciphertext vector: in encryption we expect this vector as result
  uint8_t const Expected_Ciphertext[ CIPHER_TEXT_LENGTH ] =
  {
    0x71,0x62,0x01,0x5b,0x4d,0xac,0x25,0x5d
  };

  uint8_t encrypt_OutputMessage[ 64 ] = { 0 };
  uint8_t decrypt_OutputMessage[ 64 ] = { 0 };
  int32_t encrypt_OutputMessageLength = 0;
  int32_t decrypt_OutputMessageLength = 0;
  int32_t AuthenticationTAGLength = 0;

  static volatile int32_t Status = EncryptCCM(
    (uint8_t*)HeaderMessage,
    HeaderLength ,
    (uint8_t*)Plaintext,
    InputLength,
    Key,
    IV,
    sizeof( IV ),
	encrypt_OutputMessage,
    &encrypt_OutputMessageLength,
    &AuthenticationTAGLength );

  if( Status == AES_SUCCESS )
  {
    if( memcmp( Expected_Ciphertext, encrypt_OutputMessage, CIPHER_TEXT_LENGTH ) == 0 )
    {
      Status = 0;
    }
  }

  Status = DecryptCCM(
    (uint8_t*) HeaderMessage,
    sizeof( HeaderMessage ),
    (uint8_t*) encrypt_OutputMessage,
    encrypt_OutputMessageLength,
    Key,
    IV,
    sizeof( IV ),
    decrypt_OutputMessage,
    &decrypt_OutputMessageLength,
    AuthenticationTAGLength );

  if( Status == AUTHENTICATION_SUCCESSFUL )
  {
    if( memcmp( Plaintext, decrypt_OutputMessage, PLAINTEXT_LENGTH ) == 0 )
    {
   	  Status = 0;
    }
  }
}

int32_t TAesCrypto::EncryptCCM(
  uint8_t*  HeaderMessage,
  uint32_t  HeaderMessageLength,
  uint8_t*  Plaintext,
  uint32_t  PlaintextLength,
  uint8_t  *AES128_Key,
  uint8_t  *InitializationVector,
  uint32_t  IvLength,
  uint8_t  *OutputMessage,
  int32_t  *OutputMessageLength,
  int32_t  *AuthenticationTAGLength )
{
  AESCCMctx_stt AESctx;

  // Set flag field to default value
  AESctx.mFlags = E_SK_DEFAULT;

  // Set key size to 16 (corresponding to AES-128)
  AESctx.mKeySize = 16;

  // Set nonce size field to IvLength, note that valid values are 7,8,9,10,11,12,13
  AESctx.mNonceSize = IvLength;

  // Size of returned authentication TAG
  AESctx.mTagSize = 4;

  // Set the size of the header
  AESctx.mAssDataSize = HeaderMessageLength;

  // Set the size of the payload
  AESctx.mPayloadSize = PlaintextLength;

  // Initialize the operation, by passing the key and IV
  int32_t Status = AES_CCM_Encrypt_Init(
    &AESctx, AES128_Key, InitializationVector );

  // check for initialization errors
  if( Status == AES_SUCCESS )
  {
    // Process Header
    Status = AES_CCM_Header_Append(
      &AESctx, HeaderMessage, HeaderMessageLength );

    if( Status == AES_SUCCESS )
    {
      // Encrypt Data
      Status = AES_CCM_Encrypt_Append(
        &AESctx, Plaintext, PlaintextLength, OutputMessage, OutputMessageLength );

      if( Status == AES_SUCCESS )
      {
        // Do the Finalization, write the TAG at the end of the encrypted message
        Status = AES_CCM_Encrypt_Finish(
          &AESctx, OutputMessage + *OutputMessageLength, AuthenticationTAGLength );
      }
    }
  }

  return Status;
}

int32_t TAesCrypto::DecryptCCM(
  uint8_t*  HeaderMessage,
  uint32_t  HeaderMessageLength,
  uint8_t*  Plaintext,
  uint32_t  PlaintextLength,
  uint8_t  *AES128_Key,
  uint8_t  *InitializationVector,
  uint32_t  IvLength,
  uint8_t  *OutputMessage,
  int32_t  *OutputMessageLength,
  int32_t   AuthenticationTAGLength )
{
  AESCCMctx_stt AESctx;

  // Set flag field to default value
  AESctx.mFlags = E_SK_DEFAULT;

  // Set key size to 16 (corresponding to AES-128)
  AESctx.mKeySize = 16;

  // Set nonce size field to IvLength, note that valid values are 7,8,9,10,11,12,13
  AESctx.mNonceSize = IvLength;

  // Size of returned authentication TAG
  AESctx.mTagSize = 4;

  // Set the size of the header
  AESctx.mAssDataSize = HeaderMessageLength;

  // Set the size of thepayload
  AESctx.mPayloadSize = PlaintextLength;

  // Set the pointer to the TAG to be checked
  AESctx.pmTag = Plaintext + PlaintextLength;

  // Size of returned authentication TAG
  AESctx.mTagSize = AuthenticationTAGLength;

  // Initialize the operation, by passing the key and IV
  int32_t Status = AES_CCM_Decrypt_Init(
    &AESctx, AES128_Key, InitializationVector );

  // check for initialization errors
  if( Status == AES_SUCCESS )
  {
    // Process Header
    Status = AES_CCM_Header_Append(
      &AESctx, HeaderMessage, HeaderMessageLength );

    if( Status == AES_SUCCESS )
    {
      // Decrypt Data
      Status = AES_CCM_Decrypt_Append(
        &AESctx, Plaintext, PlaintextLength, OutputMessage, OutputMessageLength );

      if( Status == AES_SUCCESS )
      {
        // Do the Finalization, check the authentication TAG
        Status = AES_CCM_Decrypt_Finish(
          &AESctx, nullptr, &AuthenticationTAGLength );
      }
    }
  }

  return Status;
}

void TAesCrypto::TestCFB()
{
  uint32_t const PLAINTEXT_LENGTH = 16;
  uint8_t OutputMessage[ PLAINTEXT_LENGTH ] = { 0 };
  uint32_t OutputMessageLength = 0;

  const uint8_t Plaintext[ PLAINTEXT_LENGTH ] =
  {
    0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
    0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
  };

  uint8_t Key[CRL_AES128_KEY] =
  {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
  };

  uint8_t IV[ CRL_AES_BLOCK ] =
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };

  uint8_t const Expected_Ciphertext[ PLAINTEXT_LENGTH ] =
  {
    0x3b, 0x3f, 0xd9, 0x2e, 0xb7, 0x2d, 0xad, 0x20,
    0x33, 0x34, 0x49, 0xf8, 0xe8, 0x3c, 0xfb, 0x4a
  };

  int32_t Status = EncryptCFB(
    (uint8_t*)Plaintext,
    PLAINTEXT_LENGTH,
    Key,
    IV,
    sizeof( IV ),
    OutputMessage,
    &OutputMessageLength );

  if( Status == AES_SUCCESS )
  {
    if( memcmp( Expected_Ciphertext, OutputMessage, PLAINTEXT_LENGTH ) == 0 )
    {
      Status = 0;
    }
  }

  Status = DecryptCFB(
    (uint8_t*)Expected_Ciphertext,
    PLAINTEXT_LENGTH,
    Key,
    IV,
    sizeof(IV),
    OutputMessage,
    &OutputMessageLength);

  if( Status == AES_SUCCESS )
  {
    if( memcmp( Plaintext, OutputMessage, PLAINTEXT_LENGTH ) == 0 )
    {
      Status = 0;
    }
  }
}

/** @brief  AES CFB Encryption example.
  * @param  InputMessage: pointer to input message to be encrypted.
  * @param  InputMessageLength: input data message length in byte.
  * @param  AES128_Key: pointer to the AES key to be used in the operation
  * @param  InitializationVector: pointer to the Initialization Vector (IV)
  * @param  IvLength: IV length in bytes.
  * @param  OutputMessage: pointer to output parameter that will handle the encrypted message
  * @param  OutputMessageLength: pointer to encrypted message length.
  * @retval error status: can be AES_SUCCESS if success or one of
  *         AES_ERR_BAD_CONTEXT, AES_ERR_BAD_PARAMETER, AES_ERR_BAD_OPERATION
  *         if error occured.
  */
int32_t TAesCrypto::EncryptCFB(
  uint8_t*  InputMessage,
  uint32_t  InputMessageLength,
  uint8_t  *AES128_Key,
  uint8_t  *InitializationVector,
  uint32_t  IvLength,
  uint8_t  *OutputMessage,
  uint32_t *OutputMessageLength )
{
  AESECBctx_stt AESctx;

  // Set flag field to default value
  AESctx.mFlags = E_SK_DEFAULT;

  // Set key size to 16 (corresponding to AES-128)
  AESctx.mKeySize = 16;

  // Set iv size field to IvLength
  AESctx.mIvSize = IvLength;

  // Initialize the operation, by passing the key.
  int32_t Status = AES_CFB_Encrypt_Init( &AESctx, AES128_Key, InitializationVector );

  if( Status == AES_SUCCESS )
  {
    int32_t outputLength = 0;
    Status = AES_CFB_Encrypt_Append( &AESctx,
                                     InputMessage,
                                     InputMessageLength,
                                     OutputMessage,
                                     &outputLength);

    if( Status == AES_SUCCESS )
    {
      *OutputMessageLength = outputLength;
      Status = AES_CFB_Encrypt_Finish( &AESctx, OutputMessage + *OutputMessageLength, &outputLength );
      *OutputMessageLength += outputLength;
    }
  }

  return Status;
}

/** @brief  AES CFB Decryption example.
  * @param  InputMessage: pointer to input message to be decrypted.
  * @param  InputMessageLength: input data message length in byte.
  * @param  AES128_Key: pointer to the AES key to be used in the operation
  * @param  InitializationVector: pointer to the Initialization Vector (IV)
  * @param  IvLength: IV length in bytes.
  * @param  OutputMessage: pointer to output parameter that will handle the decrypted message
  * @param  OutputMessageLength: pointer to decrypted message length.
  * @retval error status: can be AES_SUCCESS if success or one of
  *         AES_ERR_BAD_CONTEXT, AES_ERR_BAD_PARAMETER, AES_ERR_BAD_OPERATION
  *         if error occured.
  */
int32_t TAesCrypto::DecryptCFB(
  uint8_t*  InputMessage,
  uint32_t  InputMessageLength,
  uint8_t  *AES128_Key,
  uint8_t  *InitializationVector,
  uint32_t  IvLength,
  uint8_t  *OutputMessage,
  uint32_t *OutputMessageLength )
{
  AESECBctx_stt AESctx;

  // Set flag field to default value
  AESctx.mFlags = E_SK_DEFAULT;

  // Set key size to 16 (corresponding to AES-128)
  AESctx.mKeySize = 16;

  // Set iv size field to IvLength
  AESctx.mIvSize = IvLength;

  // Initialize the operation, by passing the key.
  int32_t Status = AES_CFB_Decrypt_Init( &AESctx, AES128_Key, InitializationVector );

  if( Status == AES_SUCCESS )
  {
    int32_t outputLength = 0;
    Status = AES_CFB_Decrypt_Append(&AESctx,
                                    InputMessage,
                                    InputMessageLength,
                                    OutputMessage,
                                    &outputLength);

    if( Status == AES_SUCCESS )
    {
      *OutputMessageLength = outputLength;
      Status = AES_CFB_Decrypt_Finish( &AESctx, OutputMessage + *OutputMessageLength, &outputLength );
      *OutputMessageLength += outputLength;
    }
  }

  return Status;
}
