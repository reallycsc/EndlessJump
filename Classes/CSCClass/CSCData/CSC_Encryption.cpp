#include "CSC_Encryption.h"
#include "openssl/aes.h"
#include <CSCClass/CommonFunctions.h>

char* csc_encode_base64(const string& str_to_encode)
{
	char* buffer;
	base64Encode((unsigned char*)str_to_encode.c_str(), str_to_encode.length(), &buffer);
	return buffer;
}

char* csc_decode_base64(const string& str_to_decode)
{
	unsigned char* buffer;
	base64Decode((unsigned char*)str_to_decode.c_str(), str_to_decode.length(), &buffer);
	return reinterpret_cast<char*>(buffer);
}

string csc_encode_aes(const string& str_to_encode)
{
	unsigned char user_key[AES_BLOCK_SIZE] = "reallycsc key";
	AES_KEY	aes_key;
	if (AES_set_encrypt_key(user_key, sizeof(user_key) * 8, &aes_key) < 0)
		return nullptr;

	// refill to scale of AES_BLOCK_SIZE
	string str_tmp = str_to_encode;
	int padding = 0;
	int str_length = str_tmp.length();
	if (str_length % AES_BLOCK_SIZE != 0)
		padding = AES_BLOCK_SIZE - str_length % AES_BLOCK_SIZE;
	str_length += padding;
	while(padding > 0)
	{
		str_tmp += '\0';
		padding--;
	}

	string str_encoded;
	for (size_t i = 0, length = str_length / AES_BLOCK_SIZE; i < length; i++)
	{
		string str16 = str_tmp.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		memset(out, 0, AES_BLOCK_SIZE);
		AES_encrypt(reinterpret_cast<const unsigned char*>(str16.c_str()), out, &aes_key);
		str_encoded += string(reinterpret_cast<const char*>(out), AES_BLOCK_SIZE);
	}
	return str_encoded;
}

string csc_decode_aes(const unsigned char* str_to_decode, const int &str_length)
{
	unsigned char user_key[AES_BLOCK_SIZE] = "reallycsc key";
	AES_KEY	aes_key;
	if (AES_set_decrypt_key(user_key, sizeof(user_key) * 8, &aes_key) < 0)
		return nullptr;

	string str_decoded;
	for (size_t i = 0, length = str_length / AES_BLOCK_SIZE; i < length; i++)
	{
		unsigned char in_tmp[AES_BLOCK_SIZE];
		memcpy(in_tmp, (char*)(str_to_decode + i*AES_BLOCK_SIZE), AES_BLOCK_SIZE);
		
		unsigned char out[AES_BLOCK_SIZE];
		memset(out, 0, AES_BLOCK_SIZE);
		AES_decrypt(in_tmp, out, &aes_key);
		str_decoded += string(reinterpret_cast<const char*>(out), AES_BLOCK_SIZE);
	}
	return str_decoded;
}