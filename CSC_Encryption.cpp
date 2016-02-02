#include "CSC_Encryption.h"

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
