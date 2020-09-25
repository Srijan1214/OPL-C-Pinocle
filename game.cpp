#include "game.h"

Game::Game()
	: m_cur_round(),
	  m_player1_cumulative_score(0),
	  m_player2_cumulative_score(0) {
}

void Game::Start_The_Game() {
	m_cur_round.Set_Previous_Scores({m_player1_cumulative_score, m_player2_cumulative_score});
	m_cur_round.Set_Cur_Round_Number(1);
	std::string user_input;

	auto get_yes_no_user_input = [&](const std::string& a_prompt_message) {
		do {
			std::cout << a_prompt_message;
			std::cin.clear();
			std::getline(std::cin, user_input);
		} while (!(user_input.size() == 1 && (tolower(user_input[0] == 'y') ||
											  tolower(user_input[1] == 'n'))));
	};

	get_yes_no_user_input("Do you want to load game from file? ");

	if(user_input[0] == 'y') {
		while(true) {
			std::cin.clear();
			std::getline(std::cin, user_input);
			try {
				m_cur_round.Load_From_File(user_input);
			} catch(const std::iostream::failure& e) {
				std::cerr << e.what() << std::endl;
				continue;
			}
			break;
		}
	} else {
		m_cur_round.Decide_First_Player_Through_Coin_Toss();
		m_cur_round.Deal_Cards_From_Deck_To_Players();
	}

	do{
		m_cur_round.Play_A_Round();
		m_player1_cumulative_score+= m_cur_round.Get_Scores()[0];
		m_player2_cumulative_score+= m_cur_round.Get_Scores()[1];

		// Reset Everything to make the next round ready
		m_cur_round.Reset_Round();
		m_cur_round.Deal_Cards_From_Deck_To_Players();
		get_yes_no_user_input("You want to play another Round (y/n) ? ");
	} while (!(user_input.size() == 1 && tolower(user_input[1] == 'n')));
	

}
