
/*
 ************************************************************
 * Name: Srijan Prasad Joshi
 * Project: Pinochle C++
 * Class: OPL Fall 20
 * Date: 09/30/2020
 ************************************************************
*/

// Created by Srijan Joshi on 09/10/19.

#pragma once
#include <iostream>

class Card {
   public:
	// There are 2 cards of the same suit and face.
	// There are 6 faces and 4 suits.
	// Storing spades first, then clubs, then hearts, then diamonds.
	// Within each face two of the same cards are stored side by side and in increasing order.
	// Cards are stored in the following order 9S, 9S, 10S, 10S, ..., 9C, ..., 9H, ..., AD, AD
	int m_id;

	Card(int a_id);
	std::string Get_Card_String_Value() const;
	int Get_Card_Face_Weight() const;
	int Get_Face() const;
	int Get_Suit() const;
	int Get_Card_Id() const;
	static const char m_suit_char_arr[4]; 
	static const char m_face_char_arr[6];
	static const int m_face_weights[6];
	static int Get_Face_From_Id(int a_id);
	static int Get_Suit_From_Id(int a_id);
	static int Get_Weight_From_Id(int a_id);
	static std::string Get_String_From_Id(int);

	static int Get_Card_Id_From_String(const std::string &a_card_str);
};
