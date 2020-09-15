#pragma once
#include "player.h"

class Computer: public Player {
	public:
		Card* Get_Card_To_Play(Card* a_lead_card_played) override; // Argument is NULL if the current player is lead player.
		int Get_Meld_To_Play() override;
};
