#pragma once
#include <iostream>

class Card {
   public:
	int m_id;
	Card(int a_id);
	std::string Get_Card_String_Value();
	int Get_Card_Weight();
	int Get_Face();
	int Get_Suit();
	void Print_Card();
	int Get_Card_Id();
	static const char m_suit_char_arr[4]; 
	static const char m_face_char_arr[6];
	static const int m_face_weights[6];
	static int Get_Face_From_Id(int a_id);
	static int Get_Suit_From_Id(int a_id);
	static std::string Get_String_From_Id(int);
	std::string m_card_type;

   private:
	// char m_suit_char_arr[4] = {'S', 'C', 'H', 'D'};
	// char m_face_char_arr[6] = {'9', 'X', 'J', 'Q', 'K', 'A'};
	// int m_face_weights[6] = {0, 10, 2, 3, 4, 11};
};
