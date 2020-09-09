#pragma once
#include <iostream>
#include <vector>

#include "card.h"

class Player {
   public:
	Player();
	void Give_Card_To_Player(Card* a_card_ptr);
	Card* Get_Card_To_Play();

   private:
	std::vector<Card*> m_hand_card_pile;
	std::vector<Card*> m_meld_card_pile;
};
