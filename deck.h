#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <ctime>

#include "card.h"

class Deck {
   public:
	std::vector<Card*> m_card_list;

	Deck();
	~Deck();
	void Initialize_Deck_With_48_CARDS();
	void Shuffle_Deck();
	Card* Pop_Top_Card();
	void Put_Card_Back_In_Deck(Card* a_card_ptr);

   private:
	void Remove_All_Cards_From_Deck();
	void Destroy_All_Card_Pointers();
};
