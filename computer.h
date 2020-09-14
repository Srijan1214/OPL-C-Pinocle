#pragma once
#include "player.h"

class Computer: public Player {
	public:
		std::vector<Card*> Ask_For_Meld_Cards() override;
		Card* Get_Card_To_Play(Card* a_lead_card_played) override; // Argument is NULL if the current player is lead player.
		int Get_Meld_To_Play() override;
	private:
		int Find_Index_of_Smallest_Card_Greater_Than_Card (Card* card_ptr);
		int Find_Index_Of_Smallest_Card();
		int Find_Index_Of_Greatest_Card();
};
