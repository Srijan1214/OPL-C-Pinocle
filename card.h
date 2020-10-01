
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
	int m_id;
	Card(int a_id);
	std::string Get_Card_String_Value() const;
	int Get_Card_Weight() const;
	int Get_Face() const;
	int Get_Suit() const;
	void Print_Card() const;
	int Get_Card_Id() const;
	static const char m_suit_char_arr[4]; 
	static const char m_face_char_arr[6];
	static const int m_face_weights[6];
	static int Get_Face_From_Id(int a_id);
	static int Get_Suit_From_Id(int a_id);
	static int Get_Weight_From_Id(int a_id);
	static std::string Get_String_From_Id(int);

	static int Get_Card_Id_From_String(std::string &a_card_str);
	std::string m_card_type;
};
