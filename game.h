#pragma once
#include "round.h"
#include <iostream>

class Game{
	public:
		Game();
		void Start_The_Game();
	private:
		Round m_game_round;
		int m_player1_cumulative_score;
		int m_player2_cumulative_score;
};