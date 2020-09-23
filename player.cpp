#include "player.h"

const int Player::m_meld_scores[9] = {150, 40, 20, 10, 100, 80, 60, 40, 40};
const std::string Player::m_meld_names[9] = {"Flush",
											 "Royal Marriage",
											 "Marriage",
											 "Dix",
											 "Four Aces",
											 "Four Kings",
											 "Four Queens",
											 "Four Jacks",
											 "Pinochle"};

Player::Player()
	: m_current_meld_cards(12),
	  m_no_of_times_meld_has_been_played(9, 0),
	  m_which_card_used_for_meld(9, std::vector<bool>(48, false)),
	  m_trump_suit_of_current_game(-1) {}

void Player::Give_Card_To_Player(Card* a_card_ptr) {
	m_hand_card_pile.push_back(a_card_ptr);
	m_hand_meld_involvement_list.push_back({});
}

void Player::Add_Cards_To_Player_Capture_Pile(Card* a_card_ptr1,
											  Card* a_card_ptr2) {
	m_capture_card_pile.push_back(a_card_ptr1);
	m_capture_card_pile.push_back(a_card_ptr2);
}

void Player::Set_Round_Score(int a_score) {
	m_round_score = a_score;
}

void Player::Set_Trump_card(int a_trump_card) {
	m_trump_card_id = a_trump_card;
}

int Player::Get_Meld_Type_12_From_Cards(std::vector<Card*>& a_meld_card_list) {
	// contains logic to detect if meld exists
	std::vector<int> meld_card_identifiers(a_meld_card_list.size());
	for (int i = 0; i < a_meld_card_list.size(); i++) {
		int card_id = a_meld_card_list[i]->Get_Card_Id();

		// Since there are duplicate cards, reduce the actual id to the lowest of a type
		if(card_id % 2 == 0) {
			meld_card_identifiers[i] = card_id;
		}else {
			meld_card_identifiers[i] = card_id - 1;
		}
	}

	std::sort(meld_card_identifiers.begin(),meld_card_identifiers.end());

	//Check for flush
	if(meld_card_identifiers.size() == 5) {
		const int ten_face = Card::Get_Face_From_Id(meld_card_identifiers[0]);
		const int ten_suit = Card::Get_Suit_From_Id(meld_card_identifiers[0]);

		const int jack_face = Card::Get_Face_From_Id(meld_card_identifiers[1]);
		const int jack_suit = Card::Get_Suit_From_Id(meld_card_identifiers[1]);

		const int queen_face = Card::Get_Face_From_Id(meld_card_identifiers[2]);
		const int queen_suit = Card::Get_Suit_From_Id(meld_card_identifiers[2]);

		const int king_face = Card::Get_Face_From_Id(meld_card_identifiers[3]);
		const int king_suit = Card::Get_Suit_From_Id(meld_card_identifiers[3]);

		const int ace_face = Card::Get_Face_From_Id(meld_card_identifiers[4]);
		const int ace_suit = Card::Get_Suit_From_Id(meld_card_identifiers[4]);

		if((ten_suit == m_trump_suit_of_current_game && 
			jack_suit == m_trump_suit_of_current_game &&
			queen_suit == m_trump_suit_of_current_game &&
			king_suit == m_trump_suit_of_current_game &&
			ace_suit == m_trump_suit_of_current_game) 
			&&
			(ten_face == 1  &&
			jack_face == 2 &&
			queen_face == 3 &&
			king_face == 4 &&
			ace_face == 5
			)
		) {
			return 0;
		}
	} else if (meld_card_identifiers.size() == 2) {
		const int first_face = Card::Get_Face_From_Id(meld_card_identifiers[0]);
		const int first_suit = Card::Get_Suit_From_Id(meld_card_identifiers[0]);

		const int second_face = Card::Get_Face_From_Id(meld_card_identifiers[1]);
		const int second_suit = Card::Get_Suit_From_Id(meld_card_identifiers[1]);

		if (first_face == 3 && second_face == 4) {
			// check for royal marriage
			if (second_suit == m_trump_suit_of_current_game &&
				first_suit == m_trump_suit_of_current_game) {
				return 1;
			} else {
				// regular marriage
				if(first_suit == second_suit){
					return 2 + first_suit;
				}
			}
		}
		// If not a marriage then can be a Pinochle
		if (first_face == 3 && second_face == 2 && first_suit == 0 &&
			second_suit == 3) {
				return 11;
		}
	}else if(meld_card_identifiers.size() == 1) {
		//dix
		const int nine_face = Card::Get_Face_From_Id(meld_card_identifiers[0]);
		const int nine_suit = Card::Get_Suit_From_Id(meld_card_identifiers[0]);
		if(nine_face == 0 && nine_suit == m_trump_suit_of_current_game) {
			return 6;
		}
	} else if (meld_card_identifiers.size() == 4) {
		const int first_face = Card::Get_Face_From_Id(meld_card_identifiers[0]);
		const int first_suit = Card::Get_Suit_From_Id(meld_card_identifiers[0]);

		const int second_face = Card::Get_Face_From_Id(meld_card_identifiers[1]);
		const int second_suit = Card::Get_Suit_From_Id(meld_card_identifiers[1]);

		const int third_face = Card::Get_Face_From_Id(meld_card_identifiers[2]);
		const int third_suit = Card::Get_Suit_From_Id(meld_card_identifiers[2]);

		const int fourth_face = Card::Get_Face_From_Id(meld_card_identifiers[3]);
		const int fourth_suit = Card::Get_Suit_From_Id(meld_card_identifiers[3]);

		if (first_suit == 0 && second_suit == 1 && third_suit == 2 &&
			fourth_suit == 3) {
			//four aces
			if (first_face == 5 && second_face == 5 && third_face == 5 &&
				fourth_face == 5) {
				return 7;
			}
			//four kings
			if (first_face == 4 && second_face == 4 && third_face == 4 &&
				fourth_face == 4) {
				return 8;
			}
			//four queens
			if (first_face == 3 && second_face == 3 && third_face == 3 &&
				fourth_face == 3) {
				return 9;
			}
			//four Jacks
			if (first_face == 2 && second_face == 2 && third_face == 2 &&
				fourth_face == 2) {
				return 10;
			}
		}
	}
	return 12;
}

void Player::Set_Trump_Suit(int a_trump_suit) {
	m_trump_suit_of_current_game = a_trump_suit;
}

void Player::Update_Meld_History(std::vector<Card*> & a_meld_card_list, int a_meld_number_9) {
	m_no_of_times_meld_has_been_played[a_meld_number_9]++;
	for(Card* card_ptr: a_meld_card_list) {
		m_which_card_used_for_meld[a_meld_number_9][card_ptr->Get_Card_Id()] = true;
		// if Royal Flush declared, then don't allow Royal Marriage of the same cards for the rest of the game
		if(a_meld_number_9 == 0) {
			m_which_card_used_for_meld[1][card_ptr->Get_Card_Id()] = true;
		}
	}
}

bool Player::Is_Meld_Allowed_By_History(std::vector<Card*> & a_meld_card_list, int a_meld_number_9){
	if(m_no_of_times_meld_has_been_played[a_meld_number_9] == 0 ) {
		return true;
	}

	//return false if one card as been played for the same meld.
	for(Card* card_ptr: a_meld_card_list) {
		if(m_which_card_used_for_meld[a_meld_number_9][card_ptr->Get_Card_Id()] == true) {
			return false;
		}
	}
	return true;
}

std::string Player::Get_Console_Message() {
	std::string message;
	message += "Hand: ";
	message += Get_Hand_Pile_String();

	message+= "\nCapture Pile : ";
	message+= Get_Capture_Pile_String();

	message+= "\nMelds: ";
	message+= Get_Melds_String();

	message+="\n";

	return message;
}

std::string Player::Get_Hand_Pile_String() {
	std::string message;
	for (int i = 0; i < m_hand_card_pile.size(); i++) {
		if (m_hand_meld_involvement_list[i].empty()) {
			message += m_hand_card_pile[i]->Get_Card_String_Value();
			if(i != m_hand_card_pile.size() - 1) {
				message += " ";
			}
		}
	}
	return message;
}

std::string Player::Get_Capture_Pile_String() {
	std::string message;
	int i;
	for(Card* card_ptr:m_capture_card_pile) {
		message+= card_ptr->Get_Card_String_Value();
		if(i != m_capture_card_pile.size() - 1) {
			message += " ";
		}
		i++;
	}
	return message;
}

std::string Player::Get_Melds_String() {
	std::string message;
	for(int i = 0; i < m_current_meld_cards.size(); i++) {
		for(auto& current_melds: m_current_meld_cards[i]) {
			for(int& hand_index: current_melds) {
				message += m_hand_card_pile[hand_index]->Get_Card_String_Value();
				if(m_hand_meld_involvement_list[hand_index].size() > 1) {
					message += "*";
				}
				if(i == m_current_meld_cards.size() - 1 && hand_index == current_melds.back()) {
					continue;	// do not add the last space if is the last character.
				}
				message += " ";
			}
		}
	}
	return message;
}

void Player::Load_Members_From_Serialization_String(std::string a_serialization_string, std::vector<bool> &a_cards_that_have_been_used) {
	auto trim = [](std::string& str) -> std::string& { 
		// right trim
		while (str.length () > 0 && (str [str.length ()-1] == ' ' || str [str.length ()-1] == '\t'))
			str.erase (str.length ()-1, 1);

		// left trim
		while (str.length () > 0 && (str [0] == ' ' || str [0] == '\t'))
			str.erase (0, 1);
		return str;
	};

	int start, end;
	start = a_serialization_string.find(':') + 1;
	end = a_serialization_string.find('\n',start);
	std::string hand_cards_string = a_serialization_string.substr(start, end - start);

	start = a_serialization_string.find(':', end) + 1;
	end = a_serialization_string.find('\n',start);
	std::string capture_cards_string = a_serialization_string.substr(start, end - start);

	start = a_serialization_string.find(':', end) + 1;
	end = a_serialization_string.find('\n',start);
	std::string meld_cards_string = a_serialization_string.substr(start, end - start);

	trim(hand_cards_string);
	trim(capture_cards_string);
	trim(meld_cards_string);

	// Clear m_hand_card_pile and m_capture_card_pile
	m_hand_card_pile.clear();
	m_hand_meld_involvement_list.clear();
	m_capture_card_pile.clear();

	Load_Hand_Cards_From_String(hand_cards_string, a_cards_that_have_been_used);
	Load_Capture_Cards_From_String(capture_cards_string, a_cards_that_have_been_used);
	Load_Meld_Cards_From_String(meld_cards_string, a_cards_that_have_been_used);
}

void Player::Load_Hand_Cards_From_String(std::string &a_meld_string, std::vector<bool> &a_cards_that_have_been_used) {
	std::stringstream s(a_meld_string);
	std::string card_str;
	while (std::getline(s, card_str, ' ')) {
		int id = Card::Get_Card_Id_From_String(card_str);
		if(a_cards_that_have_been_used[id]) {
			id+=1;
		}
		a_cards_that_have_been_used[id] = true;
		m_hand_card_pile.push_back(new Card(id));
		m_hand_meld_involvement_list.push_back({});
	}
}

void Player::Load_Capture_Cards_From_String(std::string &a_meld_string, std::vector<bool> &a_cards_that_have_been_used) {
	std::stringstream s(a_meld_string);
	std::string card_str;
	while (std::getline(s, card_str, ' ')) {
		int id = Card::Get_Card_Id_From_String(card_str);
		if(a_cards_that_have_been_used[id]) {
			id+=1;
		}
		a_cards_that_have_been_used[id] = true;
		m_capture_card_pile.push_back(new Card(id));
	}
}

void Player::Load_Meld_Cards_From_String(std::string &a_meld_string, std::vector<bool> &a_cards_that_have_been_used) {
	std::vector<std::vector<int>> logic_vector = Get_Meld_Logic_Vector();
	logic_vector[2 + m_trump_suit_of_current_game] = std::vector<int>(48,-1);
	std::vector<int> total_cards_in_melds(logic_vector.size(),0);
	for(int r = 0; r < logic_vector.size(); r++) {
		for(int c = 0; c < logic_vector[r].size(); c+=2) {
			if(logic_vector[r][c] == 0) {
				total_cards_in_melds[r]+=1;
			}
		}
	}
	
	std::vector<int> prev_meld_possibilities;
	std::stringstream s(a_meld_string);
	std::string card_str;
	std::vector<std::vector<std::string>> meld_card_strings;
	std::vector<int> meld_numbers_12_vec;

	int no_of_cards_in_meld = 0;
	while (std::getline(s, card_str, ' ')) {
		no_of_cards_in_meld += 1;
		int id = Card::Get_Card_Id_From_String(card_str);
		std::vector<int> cur_meld_possibilities;
		if(prev_meld_possibilities.empty()) {
			for(int i = 0; i < logic_vector.size(); i++) {
				if(logic_vector[i][id] == 0) {
					cur_meld_possibilities.push_back(i);
				}
			}
			meld_card_strings.push_back(std::vector<std::string>());
		} else {
			for(int& meld_number_12: prev_meld_possibilities) {
				if(logic_vector[meld_number_12][id] == 0) {
					cur_meld_possibilities.push_back(meld_number_12);
				}
			}
		}
		meld_card_strings.back().push_back(card_str);
		if(cur_meld_possibilities.size() == 1) {
			if(no_of_cards_in_meld == total_cards_in_melds[cur_meld_possibilities[0]]) {
				meld_numbers_12_vec.push_back(cur_meld_possibilities[0]);
				no_of_cards_in_meld = 0;
				cur_meld_possibilities.clear();
			}
		} else if(cur_meld_possibilities.empty()) { // royal flush and royal marriage overlapping
			for(int possible_meld: prev_meld_possibilities) {
				if(no_of_cards_in_meld - 1 == total_cards_in_melds[possible_meld]) {
					meld_card_strings.back().pop_back();
					meld_card_strings.push_back(std::vector<std::string>());
					meld_card_strings.back().push_back(card_str);
					meld_numbers_12_vec.push_back(possible_meld);
					no_of_cards_in_meld = 1;
					for(int i = 0; i < logic_vector.size(); i++) {
						if(logic_vector[i][id] == 0) {
							cur_meld_possibilities.push_back(i);
						}
					}
				}
			}
		}
		prev_meld_possibilities = cur_meld_possibilities;
	}

	// Now I have the data structure of meld_card_strings 
	// and meld_numbers_12_vec to help me give the correct id to each card string.

	// the following will keep the history of each card and 
	// tells what id the current card should be.
	std::vector<int> id_to_change_cur_card_to(48, -1);
	std::vector<std::vector<int>> meld_card_ids;

	// a lambda function to change the id to a duplicate card
	auto get_id_for_duplicate_cards = [](int a_id){
		if(a_id % 2 == 0){
			return a_id + 1;
		} else {
			return a_id - 1;
		}
	};

	// the following keeps the count of the id for the stared id
	std::vector<int> count_of_ids_for_stared_cards(48, -1);

	for(int i = 0; i < meld_card_strings.size(); i++) {
		meld_card_ids.push_back(std::vector<int>());
		int cur_meld_12 = meld_numbers_12_vec[i];
		for(std::string& card_str: meld_card_strings[i]) {
			int id = Card::Get_Card_Id_From_String(card_str);
			if(id_to_change_cur_card_to[id] != -1) {
				id = id_to_change_cur_card_to[id];
			}
			if(logic_vector[cur_meld_12][id] != 0) {
				id = get_id_for_duplicate_cards(id);
			}
			meld_card_ids.back().push_back(id);
			logic_vector[cur_meld_12][id] = 1;
			if(card_str.size() == 2) {
				if(count_of_ids_for_stared_cards[id] == -1) {
					count_of_ids_for_stared_cards[id] = 1;
				} else{
					count_of_ids_for_stared_cards[id]++;
				}
				id = get_id_for_duplicate_cards(id);
			}
			id_to_change_cur_card_to[Card::Get_Card_Id_From_String(card_str)] = id;
		}
	}

	// do not allow cards that contains * to only have 1 id.
	// i.e cards that have * will need to have multiple id's.
	auto update_stared_id_with_insufficient_count =
		[&](int starred_id) {
			for (int i = 0; i < meld_card_ids.size(); i++) {
				int cur_meld_12 = meld_numbers_12_vec[i];
				for (int& card_id : meld_card_ids[i]) {
					if (get_id_for_duplicate_cards(card_id) == starred_id) {
						if(logic_vector[TO9(cur_meld_12)][starred_id] == 0) {
							card_id = starred_id;
							return;
						}
					}
				}
			}
		};


	for(int stared_id = 0; stared_id < count_of_ids_for_stared_cards.size(); stared_id++) {
		if(count_of_ids_for_stared_cards[stared_id] != 1) {
			// This entire loop is only if the count of a certain stared meld card is 1.
			continue;
		}
		update_stared_id_with_insufficient_count(stared_id);
		
	}

	//change meld card id if card has been used before in loading
	for(std::vector<int> &card_ids: meld_card_ids) {
		for(int& id: card_ids) {
			if(a_cards_that_have_been_used[id]) {
				id = get_id_for_duplicate_cards(id);
			}
		}
	}
	// update a_cards_that_have_been_used
	for(std::vector<int> &card_ids: meld_card_ids) {
		for(int& id: card_ids) {
			a_cards_that_have_been_used[id] = true;
		}
	}

	// reset meld history
	for(std::vector<bool> &ele1: m_which_card_used_for_meld) {
		for(int j = 0; j < ele1.size(); j++) {
			ele1[j] = false;
		}
	}
	for(int& ele: m_no_of_times_meld_has_been_played) {
		ele = 0;
	}
	// update meld history
	for(int i = 0; i < meld_card_ids.size(); i++) {
		int meld_num_12 = meld_numbers_12_vec[i];
		for(int card_id : meld_card_ids[i]) {
			m_which_card_used_for_meld[TO9(meld_num_12)][card_id] = true;
		}
		m_no_of_times_meld_has_been_played[TO9(meld_num_12)]++;
	}

	// now simply update m_current_meld_cards and m_hand_meld_involvement_list

	// create the cards and add them to m_hand_card_pile and m_hand_meld_involvement_list
	std::vector<int> id_to_index(48, -1);
	for (std::vector<int>& ele : meld_card_ids){
		for(int& card_id: ele) {
			int index;
			if(id_to_index[card_id] == -1) {
				index = m_hand_card_pile.size();
				m_hand_card_pile.push_back(new Card(card_id));
				m_hand_meld_involvement_list.push_back({});
				id_to_index[card_id] = index;
			}
		}
	}

	// reset m_current_meld_cards
	for(std::vector<std::vector<int>>& ele: m_current_meld_cards) {
		ele.clear();
	}
	// update the m_current_meld_cards and m_hand_meld_involvement_list
	for(int i = 0; i < meld_card_ids.size(); i++) {
		int meld_num_12 = meld_numbers_12_vec[i];
		std::vector<int> index_vector;
		for(int& card_id: meld_card_ids[i]) {
			index_vector.push_back(id_to_index[card_id]);
		}
		m_current_meld_cards[meld_num_12].push_back(index_vector);
		for(int i = 0; i < index_vector.size(); i++) {
			int position_in_melds = m_current_meld_cards[meld_num_12].size() - 1;
			m_hand_meld_involvement_list[index_vector[i]].push_back({meld_num_12,position_in_melds, i});
		}
	}

}

std::vector<std::vector<int>> Player::Get_Meld_Logic_Vector() {
	std::vector<int> flush_requirements = {m_trump_suit_of_current_game * 6 + 1,
								 m_trump_suit_of_current_game * 6 + 2,
								 m_trump_suit_of_current_game * 6 + 3,
								 m_trump_suit_of_current_game * 6 + 4,
								 m_trump_suit_of_current_game * 6 + 5};
	std::vector<int> royal_marriage_requirements = {m_trump_suit_of_current_game * 6 + 3,
										  m_trump_suit_of_current_game * 6 + 4};
	std::vector<int> spades_marriage_requirements = {6 * 0 + 3, 6 * 0 + 4};
	std::vector<int> clubs_marriage_requirements = {6 * 1 + 3, 6 * 1 + 4};
	std::vector<int> hearts_marriage_requirements = {6 * 2 + 3, 6 * 2 + 4};
	std::vector<int> diamonds_marriage_requirements = {6 * 3 + 3, 6 * 3 + 4};

	std::vector<int> dix_requirements = {6 * m_trump_suit_of_current_game + 0};
	std::vector<int> four_aces_requirements = {5, 5 + 6, 5 + 12, 5 + 18};
	std::vector<int> four_kings_requirements = {4, 4 + 6, 4 + 12, 4 + 18};
	std::vector<int> four_queens_requirements = {3, 3 + 6, 3 + 12, 3 + 18};
	std::vector<int> four_jacks_requirements = {2, 2 + 6, 2 + 12, 2 + 18};

	std::vector<int> pinochle_requirements = {2 + 6 * 3, 3 + 6 * 0};

	std::vector<std::vector<int>> all_requirements = {
		flush_requirements,
		royal_marriage_requirements,
		spades_marriage_requirements,
		clubs_marriage_requirements,
		hearts_marriage_requirements,
		diamonds_marriage_requirements,
		dix_requirements,
		four_aces_requirements,
		four_kings_requirements,
		four_queens_requirements,
		four_jacks_requirements,
		pinochle_requirements};

	std::vector<std::vector<int>> meld_logic_vector(12, std::vector<int>(48,-1));
	int i = 0;
	for(std::vector<int> &requirements: all_requirements) {
		if( i == (2 + m_trump_suit_of_current_game)) {
			// skip the duplicate flush of royal flush
			// make all the values 0 so that to satisfy the meld, all the cards are needed which is immpossible.
			meld_logic_vector[i] = std::vector<int>(48, 0);
		}
		for(int& index: requirements) {
			// Two cards needed are sequentially stored.
			meld_logic_vector[i][index*2] = 0;
			meld_logic_vector[i][index*2 + 1] = 0;
		}
		i++;
	}

	return meld_logic_vector;
}

int Player::TO9(int a_12_based_index) {
	if (a_12_based_index < 2) {
		return a_12_based_index;
	} else if (a_12_based_index >= 2 && a_12_based_index <= 5) {
		return 2;
	} else {
		return a_12_based_index - 3;
	}
}

void Player::add_to_meld_logic_vector(std::vector<std::vector<int>>& a_meld_logic_vector, std::vector<Card*>& a_card_pile) {
	for(int i = 0; i < 12; i ++) {
		for(Card* card_ptr: a_card_pile) {
			if (m_which_card_used_for_meld[TO9(i)][card_ptr->Get_Card_Id()] == false) {
				int& ele = a_meld_logic_vector[i][card_ptr->Get_Card_Id()];
				if(ele >=0 ) {
					ele++;
				}
			}
		}
	}
}

void Player::update_logic_vector_with_history(std::vector<std::vector<int>>& a_meld_logic_vector) {
	for(int i = 0; i < 12; i ++) {
		for(int id = 0; id < 48; id++) {
			if (m_which_card_used_for_meld[TO9(i)][id] == true) {
				a_meld_logic_vector[i][id] = 0;
			}
		}
	}
}

int Player::Get_Best_Meld_If_Card_Thrown(
	std::vector<std::vector<int>>& a_meld_logic_vector, Card* a_card_ptr) {

	int max_meld_score = INT_MIN;
	int meld_number_played = -1;
	std::vector<std::vector<int>> temp_logic_vector = a_meld_logic_vector;
	for (int i = 0; i < 12; i++) {
		if (m_which_card_used_for_meld[TO9(i)][a_card_ptr->Get_Card_Id()] ==
			false) {
			int& ele = temp_logic_vector[i][a_card_ptr->Get_Card_Id()];
			if (ele > 0) {
				ele--;
			}
		}
	}
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 48; j++) {
			int& ele1 = temp_logic_vector[i][j];
			j++;
			int& ele2 = temp_logic_vector[i][j];
			if (ele1 == 0 && ele2 == 0) {
				break;
			}
			if (j == 47) {	// if no 0's are found in the array then the meld is
							// possible.
				int cur_score = m_meld_scores[TO9(i)];
				if (cur_score > max_meld_score) {
					max_meld_score = cur_score;
					meld_number_played = TO9(i);
				}
			}
		}
	}

	return meld_number_played;
}

std::pair<int,int> Player::Get_Best_MeldCardIndexPair_From_Logic(std::vector<std::vector<int>>& a_meld_logic_vector) {
	int max_meld_score = INT_MIN;
	int best_meld_number = -1;
	int best_meld_index = -1;
	int best_card_weight = -1;

	int index = 0;
	for(Card* card_ptr: m_hand_card_pile) {
		int meld_number_played = Get_Best_Meld_If_Card_Thrown(a_meld_logic_vector, card_ptr);
		if(meld_number_played != -1) {
			int cur_score = m_meld_scores[meld_number_played];
			if(cur_score > max_meld_score) {
				max_meld_score = cur_score;
				best_meld_number = meld_number_played;
				best_meld_index = index;
				best_card_weight = Get_Card_Weight(m_hand_card_pile[index]);
			} else if (cur_score == max_meld_score) {
				int cur_card_weight = Get_Card_Weight(card_ptr);
				if(cur_card_weight > best_card_weight) {
					max_meld_score = cur_score;
					best_meld_number = meld_number_played;
					best_meld_index = index;
					best_card_weight = cur_card_weight;
				}
			}
		}
		index++;
	}

	return {best_meld_index, best_meld_number};

}

std::pair<int, int> Player::Find_IndexMeldPair_Of_Card_To_Throw() {
	// Finds out highest meld is available after a certain card is removed.
	// Does this for all cards. Returns the card that results in the highest value.

	std::vector<std::vector<int>> meld_logic_vector = Get_Meld_Logic_Vector();

	add_to_meld_logic_vector(meld_logic_vector, m_hand_card_pile);

	std::pair<int,int> hand_pile_best_meld_pair = Get_Best_MeldCardIndexPair_From_Logic(meld_logic_vector);

	if(hand_pile_best_meld_pair.second == -1) { // No melds found
		return {-1, -1};
	}

	return hand_pile_best_meld_pair;
}

std::pair<std::vector<int>, int> Player::Get_Indexes_And_Meld_Number12_Best_Meld() {
	std::vector<std::vector<int>> meld_logic_vector = Get_Meld_Logic_Vector();

	add_to_meld_logic_vector(meld_logic_vector, m_hand_card_pile);

	// update meld_logic_according to meld history.
	update_logic_vector_with_history(meld_logic_vector);
	

	int max_meld_score = INT_MIN;
	int meld_number_played = -1;

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 48; j++) {
			int& ele1 = meld_logic_vector[i][j];
			j++;
			int& ele2 = meld_logic_vector[i][j];
			if (ele1 == 0 && ele2 == 0) {
				break;
			}
			if (j == 47) {	// if no 0's are found in the array then the meld is
							// possible.
				int cur_meld_score = m_meld_scores[TO9(i)];
				if(cur_meld_score > max_meld_score) {
					max_meld_score = cur_meld_score;
					meld_number_played = i;
				}
			}
		}
	}

	if(meld_number_played == -1) {
		return {{},-1};
	} else {
		std::vector<int> return_vector;
		for (int j = 0; j < 48; j++) {
			int& ele = meld_logic_vector[meld_number_played][j];
			if (ele > 0) {
				return_vector.push_back(j);
			}
		}
		return {return_vector, (meld_number_played)};
	}
}

int Player::Search_Card_In_Pile(int a_id) {
	const size_t& hand_pile_size = m_hand_card_pile.size();

	for(int i = 0; i < hand_pile_size; i++) {
		if(m_hand_card_pile[i]->Get_Card_Id() == a_id) {
			return i;
		}
	}

	return -1;
}

bool Player::Is_First_Card_Greater_Than_Lead(int a_card_first, int a_card_lead) {
	if (Card::Get_Suit_From_Id(a_card_first) == m_trump_suit_of_current_game &&
		Card::Get_Suit_From_Id(a_card_lead) != m_trump_suit_of_current_game) {
		return true;
	}
	if (Card::Get_Suit_From_Id(a_card_lead) == m_trump_suit_of_current_game &&
		Card::Get_Suit_From_Id(a_card_first) != m_trump_suit_of_current_game) {
		return false;
	}

	// The the suit of second card is not the same as the first's
	// then the lead, which is second, will automatically win
	if(Card::Get_Suit_From_Id(a_card_first) != Card::Get_Suit_From_Id(a_card_lead)) {
		return false;
	}
	// if same suit and weight is greater, then first card wins
	if(Card::Get_Weight_From_Id(a_card_first) > Card::Get_Weight_From_Id(a_card_lead)) {
		return true;
	}
	//if the same card, the lead card wins
	return false;
}

int Player::Find_Index_of_Smallest_Card_Greater_Than_Card(Card* a_card_ptr) {
	const int& hand_pile_size = m_hand_card_pile.size();

	int argument_card_id = a_card_ptr->Get_Card_Id();
	int min_card_index = -1;
	int min_greatest_card_weight = INT_MAX;
	for (int i = 0; i < hand_pile_size; i++) {
		int cur_card_id = m_hand_card_pile[i]->Get_Card_Id();
		int cur_card_weight = Get_Card_Weight(m_hand_card_pile[i]);
		if(Is_First_Card_Greater_Than_Lead(cur_card_id, argument_card_id)) {
			if(cur_card_weight < min_greatest_card_weight) {
				min_card_index = i;
				min_greatest_card_weight = cur_card_weight;
			}
		}
	}
	
	return min_card_index;
}

int Player::Find_Index_Of_Smallest_Card() {
	const int& hand_pile_size = m_hand_card_pile.size();

	int min_card_weight = INT_MAX;
	int index =-1;
	for(int i = 0; i < hand_pile_size; i++) {
		int cur_card_weight = Get_Card_Weight(m_hand_card_pile[i]);
		if(cur_card_weight < min_card_weight) {
			index = i;
			min_card_weight = cur_card_weight;
		}
	}

	return index;
}

int Player::Find_Index_Of_Greatest_Card() {
	const int& hand_pile_size = m_hand_card_pile.size();

	int max_card_weight = INT_MIN;
	int index =-1;
	for(int i = 0; i < hand_pile_size; i++) {
		int cur_card_weight = Get_Card_Weight(m_hand_card_pile[i]);
		if(cur_card_weight > max_card_weight) {
			index = i;
			max_card_weight = cur_card_weight;
		}
	}

	return index;
}

int Player::Get_Card_Weight(Card* card_ptr) {
	int face_weight = card_ptr->Get_Card_Weight();
	if(card_ptr->Get_Suit() == m_trump_suit_of_current_game) {
		// add a big number to face weight.
		return face_weight + 100;
	}
	return face_weight;
}

void Player::Remove_Card_From_Pile(int a_index) {
	// Remove the cards connected to the current card from the meld pile.
	for(auto& removal_index_to_meld_pointer_triplet: m_hand_meld_involvement_list[a_index]) {
		auto& cards_to_remove_from_meld = m_current_meld_cards[removal_index_to_meld_pointer_triplet[0]][removal_index_to_meld_pointer_triplet[1]];
		for(int& index_to_remove_from_meld: cards_to_remove_from_meld) {
			for(int i = 0; i < m_hand_meld_involvement_list[index_to_remove_from_meld].size(); i++) {
				auto& search_triplet = m_hand_meld_involvement_list[index_to_remove_from_meld][i];
				if(search_triplet[0] == removal_index_to_meld_pointer_triplet[0]){
					if(&search_triplet != &removal_index_to_meld_pointer_triplet) {
						search_triplet = m_hand_meld_involvement_list[index_to_remove_from_meld].back();
						m_hand_meld_involvement_list[index_to_remove_from_meld].pop_back();
					}
				}
			}
		}
		// m_current_meld_cards[removal_index_to_meld_pointer_triplet[0]][removal_index_to_meld_pointer_triplet[1]] = m_current_meld_cards[removal_index_to_meld_pointer_triplet[0]].back();
		cards_to_remove_from_meld = m_current_meld_cards[removal_index_to_meld_pointer_triplet[0]].back();
		m_current_meld_cards[removal_index_to_meld_pointer_triplet[0]].pop_back();
	}	

	// Change the meld lists pointing to hand at the back to change location of last card to the one that is about to be removed. 
	for(auto& ele: m_hand_meld_involvement_list.back()) {
		m_current_meld_cards[ele[0]][ele[1]][ele[2]] = a_index;
	}
	// Remove Card Completely from the Hand
	m_hand_card_pile[a_index] = m_hand_card_pile.back();
	m_hand_card_pile.pop_back();
	m_hand_meld_involvement_list[a_index] = m_hand_meld_involvement_list.back();
	m_hand_meld_involvement_list.pop_back();
}

int Player::Get_No_Of_Remaining_Cards() {
	return m_hand_card_pile.size();
}

// Only called when the round ends and the cards are placed back on the deck
std::vector<Card*> Player::Pop_All_Cards_From_Capture_Pile() {
	std::vector<Card*> ret_vec = m_capture_card_pile;
	m_capture_card_pile.clear();
	return ret_vec;
}

void Player::Reset_Meld_History() {
	for(int& number_for_meld: m_no_of_times_meld_has_been_played){
		number_for_meld = 0;
	}
	for(int meld_number_9 = 0; meld_number_9 < m_which_card_used_for_meld.size(); meld_number_9++) {
		for(int card_id = 0; card_id < 48; card_id++) {
			m_which_card_used_for_meld[meld_number_9][card_id] = false;
		}
	}
}
