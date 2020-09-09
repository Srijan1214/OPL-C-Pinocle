#include "round.h"

Round::Round() : m_human(), m_player() {
	std::cout << "round constructor" << std::endl;
}

void Round::Start_New_Round(int a_cur_round) {
	m_deck.Shuffle_Deck();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			
		}
	}
	
}