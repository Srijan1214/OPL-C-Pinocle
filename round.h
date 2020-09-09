#pragma once
#include <iostream>

#include "deck.h"
#include "human.h"
#include "player.h"

// Class is act as the referee for the game by controlling the public methods
// for the Players, Cards, and Deck. and telling them what to do.
class Round {
   public:
	Human m_human;
	Player m_player;
	Deck m_deck;
	Round();
	void Start_New_Round(int a_cur_round);
};