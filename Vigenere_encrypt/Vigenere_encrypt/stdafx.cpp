
// stdafx.cpp: исходный файл, содержащий стандарт, включающий
// Vigenere_encrypt.pch будет использоваться в качестве предкомпилированного заголовка
// stdafx.obj будет содержать предварительно откомпилированные сведения о типе

#include "stdafx.h"


int my_strchr(char* str, char c, bool if_rus)
{
	if (c > 0 && if_rus)
	{
		c += 176;
	}

	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] == c)
			return i;
	}
	return -1;
}

void create_square(char** Vig_square, const char* language, int letters_count)
{
	char s , c;
	int start_pos;

	//char Vig_square[letters_count + 2][letters_count + 2];

	Vig_square[0][0] = '0';

	if (!strcmp(language, "Eng"))
	{
		start_pos = 97;
	}
	else
	{
		start_pos = 224;
		letters_count--;
	}

	s = start_pos;

	for (auto i = 1; i <= letters_count; i++, s++)
	{
		Vig_square[i][0] = s;
		Vig_square[0][i] = s;
	}

	Vig_square[letters_count + 1][0] = '\0';
	Vig_square[0][letters_count + 1] = '\0';


	for (auto i = 1; i <= letters_count; i++)
	{
		c = Vig_square[i][0];

		auto j = 1;
		for (; j <= letters_count; j++, c++)
		{
			Vig_square[i][j] = c;

			if (c == char(start_pos + letters_count - 1))
				c = char(start_pos) - 1;
		}

		Vig_square[i][j] = '\0';
	}
}

void encrypt(char* buffer, char* to_encrypt, int const& word_size, CString keyword, char** square, const char* language)
{
	int const letters_count = strlen(square[0]), key_size = keyword.GetLength();
	bool my_upper = false;

	int index1, index2;

	for (int i = 0, j = 0; i < word_size; i++, j++)
	{
		if (j == key_size)
			j = 0;

		if ((to_encrypt[i] < 97 && to_encrypt[i] >= 65) || to_encrypt[i] <= -33)
		{
			my_upper = true;
			to_encrypt[i] += 32;
		}

		if(to_encrypt[i] < 0 || (to_encrypt[i] >= 65 && to_encrypt[i] <= 90) || (to_encrypt[i] >= 97 && to_encrypt[i] <= 122))
		{
			if (!strcmp(language, "Eng"))
			{
				index1 = my_strchr(square[0], to_encrypt[i], false);
				index2 = my_strchr(square[0], keyword[j], false);
			}
			else
			{
				index1 = my_strchr(square[0], to_encrypt[i], true);
				index2 = my_strchr(square[0], keyword[j], true);
			}

			if (my_upper)
				buffer[i] = square[index1][index2] - 32;
			else
				buffer[i] = square[index1][index2];
			my_upper = false;
		}
		else
		{
			buffer[i] = to_encrypt[i];
			j--;
		}
	}
	buffer[word_size] = '\0';
}