#include "round.h"

Round::Round() : m_players({&m_player1,&m_player2}), m_scores(2,0){
}

void Round::Play_A_Round() {
	Deal_Cards_From_Deck_To_Players();
	if(m_cur_round_number == 1) {
		Set_Cur_Turn_Through_Coin_Toss(m_player1);
	}

	Pick_Up_Trump_Card();
	// Tell the players which suit is trump suit
	for(Player* player_ptr:m_players) {
		player_ptr->Set_Trump_Suit(m_trump_card->Get_Suit());
		player_ptr->Set_Trump_card(m_trump_card->Get_Card_Id());
	}

	// Load_From_File("serialize");
	Print_Interface_Message();

	for(int i = 0; i < 11; i++) {
		// The following function will update lead and chase player based on the outcome of the battle.
		Play_Cards_Against_Each_Other();
		//lead player declares meld
		// Print_Interface_Message();
		int meld_index = m_players[m_cur_lead_player]->Get_Meld_To_Play();
		if (meld_index >= 0 && meld_index < 9) {
			m_scores[m_cur_lead_player] += Player::m_meld_scores[meld_index];
		}
		m_players[m_cur_lead_player]->Set_Round_Score(m_scores[m_cur_lead_player]);

		const int chase_player = m_cur_lead_player ^ 1;
		m_players[m_cur_lead_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
		m_players[chase_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
		std::cout << std::endl << "--------------------------------------------------------New Battle--------------------------------------------------------------------------------------------------" << std::endl;
		Print_Interface_Message();
	}
	Save_To_File("serialize1");

	// In the following battle, the stock pile is finished and the trump card is given to the chase player.
	Play_Cards_Against_Each_Other();
	//lead player declares meld
	// Print_Interface_Message();
	int meld_index = m_players[m_cur_lead_player]->Get_Meld_To_Play();
	if (meld_index >= 0 && meld_index < 9) {
		m_scores[m_cur_lead_player] += Player::m_meld_scores[meld_index];
	}
	m_players[m_cur_lead_player]->Set_Round_Score(m_scores[m_cur_lead_player]);

	const int chase_player = m_cur_lead_player ^ 1;
	m_players[m_cur_lead_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
	m_players[chase_player]->Give_Card_To_Player(m_trump_card);

	std::cout << std::endl << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "------------------------------------------------------------------------STOCK PILE EMPTIED----------------------------------------------------------------------------------" << std::endl;
	std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	//From now on cards aren't picked up from the stock pile.
	for(int i = 0; i < 12; i++) {
		// The following function will update lead and chase player based on the outcome of the battle.
		std::cout << std::endl << "--------------------------------------------------------New Battle--------------------------------------------------------------------------------------------------" << std::endl;
		Print_Interface_Message();
		Play_Cards_Against_Each_Other();
		//lead player declares meld
		// Print_Interface_Message();
		meld_index = m_players[m_cur_lead_player]->Get_Meld_To_Play();
		if (meld_index >= 0 && meld_index < 9) {
			m_scores[m_cur_lead_player] += Player::m_meld_scores[meld_index];
		}
		m_players[m_cur_lead_player]->Set_Round_Score(m_scores[m_cur_lead_player]);
	}

}

void Round::Play_Cards_Against_Each_Other() {
	const int chase_player = m_cur_lead_player ^ 1; // The other of the lead players
	Card* lead_card = m_players[m_cur_lead_player]->Get_Card_To_Play(NULL);
	Card* chase_card = m_players[chase_player]->Get_Card_To_Play(lead_card);

	int who_won = -1;

	if(lead_card->Get_Suit() == chase_card->Get_Suit()) {
		if(lead_card->Get_Card_Weight() >= chase_card->Get_Card_Weight() ) {
			who_won = m_cur_lead_player;
		} else {
			who_won = chase_player;
		}
	}else {
		if(chase_card->Get_Suit() == m_trump_card->Get_Suit()) {
			who_won = chase_player;
		}else {
			who_won = m_cur_lead_player;
		}
	}

	if(who_won == m_cur_lead_player) {
		m_scores[m_cur_lead_player] += (lead_card->Get_Card_Weight() + chase_card->Get_Card_Weight());
		m_players[m_cur_lead_player]->Add_Cards_To_Player_Capture_Pile(lead_card, chase_card);
	} else {
		m_scores[chase_player] += (lead_card->Get_Card_Weight() + chase_card->Get_Card_Weight());
		m_players[chase_player]->Add_Cards_To_Player_Capture_Pile(lead_card, chase_card);
		//switch lead and chase player uf chase player wins
		m_cur_lead_player^= 1;
	}

	if(who_won == 0) {
		std::cout << "--------------------------------Player 1 WINS BATTLE--------------------------------" << std::endl << std::endl;
	} else {
		std::cout << "--------------------------------Player 2 WINS BATTLE--------------------------------" << std::endl << std::endl;
	}
	
}

void Round::Deal_Cards_From_Deck_To_Players() {
	m_deck.Shuffle_Deck();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			Card* card_to_give = m_deck.Pop_Top_Card();
			m_players[1]->Give_Card_To_Player(card_to_give);
		}

		for (int j = 0; j < 4; j++) {
			Card* card_to_give = m_deck.Pop_Top_Card();
			m_players[0]->Give_Card_To_Player(card_to_give);
		}
	}
}

void Round::Set_Cur_Turn_Through_Coin_Toss(const Human& a_human_player) {
	const int coin_toss_value = rand() % 1; // Tails is 1 and Heads is 0.

	char human_choice = a_human_player.Get_Heads_Or_Tails();

	bool was_user_correct = (human_choice == 'T' && coin_toss_value == 1 ) || (human_choice == 'H' && coin_toss_value == 0 );

	if(was_user_correct) {
		m_cur_lead_player = 0;
	} else {
		m_cur_lead_player = 1;
	}
}

void Round::Pick_Up_Trump_Card() {
	m_trump_card = m_deck.Pop_Top_Card();
}

const std::vector<int> & Round::Get_Scores() const {
	return m_scores;
}

void Round::Print_Player1_Hand() {
	std::cout << "Human:" << " Score: " << m_scores[0] << std::endl;
	std::cout << m_player1.Get_Console_Message() << std::endl;
}

void Round::Print_Player2_Hand() {
	std::cout << "Computer:" << " Score: " << m_scores[1] << std::endl;
	std::cout << m_player2.Get_Console_Message() << std::endl;
}

void Round::Print_Interface_Message() {
	std::cout << "Round: " << 1 << std::endl;
	std::cout << std::endl;
	Print_Player1_Hand();
	Print_Player2_Hand();
	std::cout << "Trump Card: " << m_trump_card->Get_Card_String_Value()  << std::endl;
	std::cout << "Stocks: " << m_deck.Get_Stock_String() <<std::endl;
	// std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
	std::cout << std::endl << "--------------------------------------------------------------------------------------";
	std::cout << std::endl << std::endl;
}

void Round::Set_Cur_Round_Number(int a_cur_round) {
	m_cur_round_number = a_cur_round;
}

void Round::Set_Previous_Scores(std::vector<int> a_prev_scores) {
	m_prev_scores = a_prev_scores;
}

void Round::Save_To_File(std::string a_path) {
	std::ofstream file;
	file.open(a_path);

	file << "Round: " << m_cur_round_number << "\n\n";

	file << "Computer:\n";
	file << "   Score: " << m_prev_scores[1] << " / " << m_scores[1] << "\n";
	file << "   Hand: " <<m_players[1]->Get_Hand_Pile_String() << "\n";
	file << "   Capture Pile : " <<m_players[1]->Get_Capture_Pile_String() << "\n";
	file << "   Melds : " <<m_players[1]->Get_Melds_String() << "\n\n";

	file << "Human:\n";
	file << "   Score: " << m_prev_scores[0] << " / " << m_scores[0] << "\n";
	file << "   Hand: " <<m_players[0]->Get_Hand_Pile_String() << "\n";
	file << "   Capture Pile : " <<m_players[0]->Get_Capture_Pile_String() << "\n";
	file << "   Melds : " <<m_players[0]->Get_Melds_String() << "\n\n";

	file << "Trump Card: " <<m_trump_card->Get_Card_String_Value() << "\n";
	file << "Stocks: " << m_deck.Get_Stock_String() << "\n\n";

	file << "Next Player: ";
	if(m_cur_lead_player == 0) {
		file << "Human";
	} else if(m_cur_lead_player == 1) {
		file << "Computer";
	}
	file.close();
}

void Round::Load_From_File(std::string a_path) {
	std::ifstream file(a_path);
	std::string line;
	std::string computer_load_string;
	std::string human_load_string;

	if(file.is_open()){
		int line_number = 1;
		while(std::getline(file, line)) {
			if(line_number >= 5 && line_number < 8) {
				computer_load_string += (line + '\n');
			}
			if(line_number >= 11 && line_number < 14) {
				human_load_string += (line + '\n');
			}
			line_number++;
		}
	} else {
		std::cout << "Error Opening file" << std::endl;
		throw 123;
	}

	m_players[0]->Load_Members_From_Serialization_String(human_load_string);
	m_players[1]->Load_Members_From_Serialization_String(computer_load_string);
}