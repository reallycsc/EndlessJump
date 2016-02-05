#pragma once
#include "../CommonDefinition.h"

char* csc_encode_base64(const string &str_to_encode);
char* csc_decode_base64(const string &str_to_decode);
string csc_encode_aes(const string& str_to_encode);
string csc_decode_aes(const unsigned char* str_to_decode, const int &length);