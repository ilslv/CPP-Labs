#pragma once
#include <atltime.h>

void encrypt(char* buffer, char* to_encrypt, int const& word_size, CString keyword, char** square, bool with_punct);
void create_square(char** Vig_square, int letters_count);
int my_strchr(char* str, char c, int differ = -65);