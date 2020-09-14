#pragma once
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <string>

#include "player.h"

class Human : public Player {
   public:
	Human();
	const char Get_Heads_Or_Tails() const;
	std::vector<Card*> Ask_For_Meld_Cards();
	Card* Get_Card_To_Play(Card* a_lead_card_played) override; // Argument is NULL if the current player is lead player.
	int Get_Meld_To_Play() override;
	private:
	void Print_Hand_and_Meld_With_Id();
	std::vector<int> Parse_Indexes_Vector_From_String(std::string a_user_input);
	private:
	int Get_Integer_Input_From_User();
	char Get_Char_Input_From_User();
};
