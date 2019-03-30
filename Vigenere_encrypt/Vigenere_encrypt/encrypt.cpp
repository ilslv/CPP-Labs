#include <stdafx.h>
#include "encrypt.h"

int my_strchr(char* str, char c, int differ = 0)
{
	if (c < 65 && c > 0)
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
	char s, c;
	int start_position;

	//char Vig_square[letters_count + 2][letters_count + 2];
	
	Vig_square[0][0] = '0';

	if (!strcmp(language, "Rus"))
	{
		start_position = 224;
		letters_count--;
	}
	else
	{
		start_position = 97;
	}

	s = char(start_position);

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

			if (c == char(start_position + letters_count - 1))
				c = char(start_position) - 1;
		}

		Vig_square[i][j] = '\0';
	}
}

void encrypt(char* buffer, char* to_encrypt, int const& word_size, CString keyword, char** square)
{
	int const letters_count = strlen(square[0]), key_size = keyword.GetLength();
	bool my_upper = false;

	int index1, index2; 

	for (int i = 0, j = 0; i < word_size; i++, j++)
	{
		if (j == key_size)
			j = 0;

		if ((97 <= int(to_encrypt[i]) && int(to_encrypt[i]) <= 122) || int(to_encrypt[i]) <= 0)
		{
			if ((to_encrypt[i] >= 65 && to_encrypt[i] <= 90) || (to_encrypt[i] <= -33 && to_encrypt[i] >= -64))
			{
				to_encrypt[i] = tolower(to_encrypt[i]) + 32;
				my_upper = true;
			}

			index1 = my_strchr(square[0], keyword[j]);
			index2 = my_strchr(square[0], to_encrypt[i]);

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