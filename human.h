
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
#include <unordered_set>
#include <sstream>
#include <string>

#include "player.h"

class Human : public Player {
   public:
	Human();
	Card* Get_Card_To_Play(Card* a_lead_card_played) override; // Argument is NULL if the current player is lead player.
	int Get_Meld_To_Play() override;

   private:
	// Print the Cards with an Index above them to help in user input
	void Print_Hand_and_Meld_With_Id() const;

	// Parse a string of numbers into a vector of numbers
	std::vector<int> Parse_Indexes_Vector_From_String(std::string a_user_input) const;
	
	// Get Validated Input from User
	int Get_Integer_Input_From_User() const;
	char Get_Char_Input_From_User() const;

	// Help Mode Functions
	void Print_Computer_Card_Recomendation(Card* a_lead_card_played) const ;
	void Print_Computer_Meld_Recommendation() const ;
};
