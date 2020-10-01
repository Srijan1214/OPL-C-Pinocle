
/*
 ************************************************************
 * Name: Srijan Prasad Joshi
 * Project: Pinochle C++
 * Class: OPL Fall 20
 * Date: 09/30/2020
 ************************************************************
*/

// Created by Srijan Joshi on 09/10/19.

#include "game.h"

Game::Game()
	: m_game_round(),
	  m_player1_cumulative_score(0),
	  m_player2_cumulative_score(0) {
}

void Game::Start_The_Game() {
	m_game_round.Set_Previous_Scores({m_player1_cumulative_score, m_player2_cumulative_score});
	m_game_round.Set_Cur_Round_Number(1);
	std::string user_input;

	auto get_yes_no_user_input = [&](const std::string& a_prompt_message) {
		do {
			std::cout << a_prompt_message;
			std::cin.clear();
			std::getline(std::cin, user_input);
		} while (!(user_input.size() == 1 && (tolower(user_input[0]) == 'y' || tolower(user_input[0])  == 'n')));
	};

	get_yes_no_user_input("Do you want to load game from file (y/n)? ");

	if(tolower(user_input[0]) == 'y') {
		while(true) {
			std::cout << "Enter File Name: ";
			std::cin.clear();
			std::getline(std::cin, user_input);
			try {
				m_game_round.Load_From_File(user_input);
			} catch(const std::iostream::failure& e) {
				std::cerr << e.what() << std::endl;
				continue;
			}
			break;
		}
	} else {
		m_game_round.Decide_First_Player_Through_Coin_Toss();
		m_game_round.Deal_Cards_From_Deck_To_Players();
	}

	do{
		m_game_round.Play_A_Round();
		m_player1_cumulative_score+= m_game_round.Get_Scores()[0];
		m_player2_cumulative_score+= m_game_round.Get_Scores()[1];
		m_game_round.Set_Previous_Scores({m_player1_cumulative_score, m_player2_cumulative_score});

		// Reset Everything to make the next round ready
		m_game_round.Reset_Round();

		m_game_round.Increase_Round_Number();
		m_game_round.Deal_Cards_From_Deck_To_Players();
		if(m_player1_cumulative_score > m_player2_cumulative_score) {
			m_game_round.Set_Player_1_Lead_Player();
		} else if (m_player1_cumulative_score == m_player2_cumulative_score) {
			m_game_round.Decide_First_Player_Through_Coin_Toss();
		} else {
			m_game_round.Set_Player_2_Lead_Player();
		}
		std::cout << "Human Cumulative Score: " << m_player1_cumulative_score << std::endl;
		std::cout << "Computer Cumulative Score: " << m_player2_cumulative_score << std::endl;
		std::cout << std::endl;
		get_yes_no_user_input("You want to play another Round (y/n) ? ");
	} while (!(user_input.size() == 1 && tolower(user_input[0]) == 'n'));

	if (m_player1_cumulative_score > m_player2_cumulative_score) {
		std::cout << "Human Wins" << std::endl;
	} else if (m_player1_cumulative_score == m_player2_cumulative_score) {
		std::cout << "Draw" << std::endl;
	} else {
		std::cout << "Computer Wins" << std::endl;
	}
}
