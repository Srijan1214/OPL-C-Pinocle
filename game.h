#pragma once
#include "round.h"
#include <iostream>

class Game{
	public:
		Round m_cur_round;
		int m_player1_cumulative_score;
		int m_player2_cumulative_score;
		Game();
		void load_game_from_file();
};