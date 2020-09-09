#include "game.h"

Game::Game()
	: m_cur_round(),
	  m_player1_cumulative_score(0),
	  m_player2_cumulative_score(0) {
		  m_cur_round.Start_New_Round(0);

}
