#include "game.h"

Game::Game()
	: m_cur_round(),
	  m_player1_cumulative_score(0),
	  m_player2_cumulative_score(0) {
	m_cur_round.Play_A_Round(0);
	m_player1_cumulative_score+= m_cur_round.Get_Scores()[0];
	m_player2_cumulative_score+= m_cur_round.Get_Scores()[1];
}
