#pragma once
#include <iostream>

class Card {
   public:
	int m_id;
	Card(int a_id);
	std::string Get_Card_String_Value();
	void Print_Card();

   private:
	char m_suit_char_arr[4] = {'S', 'C', 'H', 'D'};
	char m_face_char_arr[6] = {'9', 'X', 'J', 'Q', 'K', 'A'};
};
