
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
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <ctime>

#include <string>
#include <sstream>

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

	int Get_Stock_Size() const;

	std::string Get_Stock_String() const;

	void Remove_All_Cards_From_Deck();

	void Load_Stock_Pile_From_String(const std::string &a_stock_string, std::vector<bool> &a_cards_that_have_been_used);
   private:
	void Destroy_All_Card_Pointers();
};
