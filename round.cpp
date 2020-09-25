#include "round.h"

Round::Round() : m_players({&m_player1,&m_player2}), m_scores(2,0){
}

void Round::Play_A_Round() {
	// Tell the players which suit is trump suit
	for(Player* player_ptr:m_players) {
		player_ptr->Set_Trump_Suit(m_trump_card->Get_Suit());
		player_ptr->Set_Trump_card(m_trump_card->Get_Card_Id());
	}

	while(m_deck.Get_Stock_Size() > 1) {
		Play_Turn_And_Pick_Up_From_Stock();
		std::cout << std::endl << "--------------------------------------------------------New Battle--------------------------------------------------------------------------------------------------" << std::endl;
	}

	if(m_deck.Get_Stock_Size() == 1) {
		Play_Turn_And_Dont_Pick_Up_From_Stock();
		const int chase_player = m_cur_lead_player ^ 1;
		m_players[m_cur_lead_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
		m_players[chase_player]->Give_Card_To_Player(m_trump_card);

		std::cout << std::endl << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "------------------------------------------------------------------------STOCK PILE EMPTIED----------------------------------------------------------------------------------" << std::endl;
		std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}

	//From now on cards aren't picked up from the stock pile.
	while(m_players[m_cur_lead_player]->Get_No_Of_Remaining_Cards() > 0) {
		Play_Turn_And_Dont_Pick_Up_From_Stock();
		m_players[m_cur_lead_player]->Set_Round_Score(m_scores[m_cur_lead_player]);
	}

}

void Round::Play_Cards_Against_Each_Other() {
	const int chase_player = m_cur_lead_player ^ 1; // The other of the lead players
	Ask_Input_From_Menu(m_cur_lead_player);
	Card* lead_card = m_players[m_cur_lead_player]->Get_Card_To_Play(NULL);

	Ask_Input_From_Menu(chase_player);
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
	Pick_Up_Trump_Card();
}

void Round::Decide_First_Player_Through_Coin_Toss() {
	const int coin_toss_value = rand() % 1; // Tails is 1 and Heads is 0.

	std::string user_input;

	do {
		std::cout << "Coin was flipped. Give heads or tails (H/T) ?";
		std::cin.clear();
		std::getline(std::cin, user_input);
	} while (!(user_input.size() == 1 && (tolower(user_input[0] == 'h') ||
											tolower(user_input[1] == 't'))));

	char human_choice = toupper(user_input[0]);

	bool was_user_correct = (human_choice == 'T' && coin_toss_value == 1 ) || (human_choice == 'H' && coin_toss_value == 0 );

	if(was_user_correct) {
		m_cur_lead_player = 0;
	} else {
		m_cur_lead_player = 1;
	}
}

void Round::Decide_Next_Round_Starting_Player() {
	if(m_scores[0] > m_scores[1]) {
		m_cur_lead_player = 0;
	} else if (m_scores[0] == m_scores[1]) {
		Decide_First_Player_Through_Coin_Toss();
	} else{
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
	std::string stock_load_string;
	int round_number;
	int computer_round_score, computer_prev_score;
	int human_round_score, human_prev_score;
	int trump_card_id;
	int next_player;

	auto get_score_pairs_from_string = [](std::string& line) {
		std::pair<int,int> ret_val;
		int start = line.find(':') + 1;
		start = line.find_first_not_of(' ', start);
		int end = line.find_first_of(' ', start);
		std::string num_str = line.substr(start, end - start);
		ret_val.first = std::stoi(num_str);
		start = line.find_first_not_of(' ', end);
		start = line.find_first_not_of(' ', start + 1);
		num_str = line.substr(start, line.size() - start);
		ret_val.second = std::stoi(num_str);
		return ret_val;
	};

	if(file.is_open()){
		int line_number = 1;
		while(std::getline(file, line)) {
			if(line_number == 1) {
				int start = line.find(':') + 1;
				std::string num_str = line.substr(start, line.size() - start);
				round_number = std::stoi(num_str);
			}
			if(line_number == 4) {
				std::pair<int, int> prev_score_and_cur_score = get_score_pairs_from_string(line);
				computer_prev_score = prev_score_and_cur_score.first;
				computer_round_score = prev_score_and_cur_score.second;
			}
			if(line_number == 10) {
				std::pair<int, int> prev_score_and_cur_score = get_score_pairs_from_string(line);
				human_prev_score = prev_score_and_cur_score.first;
				human_round_score = prev_score_and_cur_score.second;
			}
			if(line_number == 15) {
				int start = line.find(':') + 1;
				start = line.find_first_not_of(' ', start);
				std::string card_str = line.substr(start, line.size() - start);
				trump_card_id = Card::Get_Card_Id_From_String(card_str);
			}
			if(line_number == 16) {
				int start = line.find(':') + 1;
				start = line.find_first_not_of(' ', start);
				int end = line.find_last_not_of(' ');
				stock_load_string = line.substr(start, end - start + 1);
			}
			if(line_number == 18) {
				int start = line.find(':') + 1;
				start = line.find_first_not_of(' ', start);
				int end = line.find_last_not_of(' ');

				std::string turn_str = line.substr(start, end - start + 1);
				if(turn_str == "Human") {
					next_player = 0;
				} else if (turn_str == "Computer"){
					next_player = 1;
				}
			}
			if(line_number >= 5 && line_number < 8) {
				computer_load_string += (line + '\n');
			}
			if(line_number >= 11 && line_number < 14) {
				human_load_string += (line + '\n');
			}
			line_number++;
		}
	} else {
		throw std::iostream::failure("Error Opening File");
	}

	// Destroy all previous cards so that new cards can be created from file.
	m_deck.Remove_All_Cards_From_Deck();

	std::vector<bool> cards_that_have_been_used(48,false);
	m_trump_card = new Card(trump_card_id);
	cards_that_have_been_used[trump_card_id] = true;
	m_players[0]->Set_Trump_card(trump_card_id);
	m_players[1]->Set_Trump_card(trump_card_id);
	m_players[0]->Set_Trump_Suit(Card::Get_Suit_From_Id(trump_card_id));
	m_players[1]->Set_Trump_Suit(Card::Get_Suit_From_Id(trump_card_id));

	

	m_deck.Load_Stock_Pile_From_String(stock_load_string, cards_that_have_been_used);
	m_players[0]->Load_Members_From_Serialization_String(human_load_string, cards_that_have_been_used);
	m_players[1]->Load_Members_From_Serialization_String(computer_load_string, cards_that_have_been_used);
	m_cur_lead_player = next_player;

	m_prev_scores[0] = human_prev_score;
	m_prev_scores[1] = computer_prev_score;
	m_scores[0] = human_round_score;
	m_scores[1] = computer_round_score;

	m_cur_round_number = round_number;
}

void Round::Play_Turn_And_Dont_Pick_Up_From_Stock() {
	Print_Interface_Message();
	// The following function will update lead and chase player based on the outcome of the battle.
	Play_Cards_Against_Each_Other();
	//lead player declares meld
	// Print_Interface_Message();
	int meld_index = m_players[m_cur_lead_player]->Get_Meld_To_Play();
	if (meld_index >= 0 && meld_index < 9) {
		m_scores[m_cur_lead_player] += Player::m_meld_scores[meld_index];
	}
	m_players[m_cur_lead_player]->Set_Round_Score(m_scores[m_cur_lead_player]);
}

void Round::Play_Turn_And_Pick_Up_From_Stock() {
	Play_Turn_And_Dont_Pick_Up_From_Stock();
	const int chase_player = m_cur_lead_player ^ 1;
	m_players[m_cur_lead_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
	m_players[chase_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
}

void Round::Reset_Round() {
	// Put all cards back in deck
	for(Player* player_ptr: m_players) {
		std::vector<Card*> capture_pile_cards = player_ptr->Pop_All_Cards_From_Capture_Pile();
		for(Card* card_ptr: capture_pile_cards) {
			m_deck.Put_Card_Back_In_Deck(card_ptr);
		}
	}
	if(m_deck.Get_Stock_Size() != 48) {
		throw 123; // assert that all the cards are restored
	}
	// Reset Meld history for both players
	for(Player* player_ptr: m_players) {
		player_ptr->Reset_Meld_History();
	}
}

void Round::Ask_Input_From_Menu(int a_cur_player) {
	auto get_int_input_from_user = []() {
		int ret_val = -1;
		std::string user_input;

		std::cin.clear();
		std::getline(std::cin, user_input);

		try {
			ret_val = std::stoi(user_input);
		} catch (const std::invalid_argument&) {
		} catch (const std::out_of_range&) {
		}

		return ret_val;
	};
	if(a_cur_player == 0) {
		std::cout << "1. Save the game" << std::endl;
		std::cout << "2. Make a move" << std::endl;
		std::cout << "3. Ask for help" << std::endl;
		std::cout << "4 .Quit the game" << std::endl;
	}else {
		std::cout << "1. Make a move" << std::endl;
		std::cout << "2 .Quit the game" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Enter the option number from 1 - 4: ";
	const int max_option_number = (a_cur_player == 0) ? 4 : 2;

	int user_input;
	do{
		user_input = get_int_input_from_user();
	} while (!(user_input >= 1 && user_input <= max_option_number));

	if (a_cur_player != 0) {
		if(user_input == 1) {
			user_input = 2;
		}else if (user_input == 2) {
			user_input = 4;
		}
	}

	m_players[a_cur_player]->Turn_Off_Help_Mode();
	if (user_input == 1) {
		// Save Game and quit
		Save_To_File("serialize");
		exit(EXIT_SUCCESS);
	} else if (user_input == 2) {
		return;
	} else if (user_input == 3) {
		m_players[a_cur_player]->Turn_On_Help_Mode();
		return;
	} else if (user_input == 4) {
		exit(EXIT_SUCCESS);
	}
}

void Round::Increase_Round_Number() {
	m_cur_round_number+=1;
}
