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
	: m_no_of_times_meld_has_been_played(9, 0),
	  m_which_card_used_for_meld(9, std::vector<bool>(48, false)),
	  m_current_meld_cards(12),
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

bool Player::Is_Meld_Valid(std::vector<Card*>& a_meld_card_list) {
	int meld_number = TO9(Get_Meld_Type_12_From_Cards(a_meld_card_list));
	if(meld_number < 0 && meld_number >= 9){ 
		return false;
	}

	return Validate_If_Meld_Can_Be_Played(a_meld_card_list,meld_number);
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

void Player::Update_Meld_History(std::vector<Card*> & a_meld_card_list, int a_meld_number) {
	m_no_of_times_meld_has_been_played[a_meld_number]++;
	for(Card* card_ptr: a_meld_card_list) {
		m_which_card_used_for_meld[a_meld_number][card_ptr->Get_Card_Id()] = true;
	}
}

bool Player::Validate_If_Meld_Can_Be_Played(std::vector<Card*> & a_meld_card_list, int a_meld_number){
	if(m_no_of_times_meld_has_been_played[a_meld_number] == 0 ) {
		return true;
	}

	//return false if one card as been played for the same meld.
	for(Card* card_ptr: a_meld_card_list) {
		if(m_which_card_used_for_meld[a_meld_number][card_ptr->Get_Card_Id()] == true) {
			return false;
		}
	}
	return true;
}

std::string Player::Get_Console_Message() {
	std::string message;
	message += "Hand: ";
	
	for(int i = 0; i < m_hand_card_pile.size(); i++) {
		if(m_hand_meld_involvement_list[i].empty()) {
			message += m_hand_card_pile[i]->Get_Card_String_Value() + " ";
		}
	}	


	message+= "\n Capture Pile : ";
	for(Card* card_ptr:m_capture_card_pile) {
		message+= card_ptr->Get_Card_String_Value() + ' ';
	}

	message+= "\n Melds: ";
	for(int i = 0; i < m_current_meld_cards.size(); i++) {
		for(auto& current_melds: m_current_meld_cards[i]) {
			for(int& hand_index: current_melds) {
				// std::cout << m_hand_card_pile[hand_index]->Get_Card_String_Value();
				message += m_hand_card_pile[hand_index]->Get_Card_String_Value();
				if(m_hand_meld_involvement_list[hand_index].size() > 1) {
					message += "*";
				}
				message += " ";
			}
		}
	}
	message+="\n";

	return message;
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
	for(Card* card_ptr: a_card_pile) {
		for(int i = 0; i < 12; i ++) {
			if (m_which_card_used_for_meld[TO9(i)][card_ptr->Get_Card_Id()] == false) {
				int& ele = a_meld_logic_vector[i][card_ptr->Get_Card_Id()];
				if(ele >=0 ) {
					ele++;
				}
			}
		}
	}
}

int Player::Get_Best_Meld_If_Card_Thrown(
	std::vector<std::vector<int>>& a_meld_logic_vector, Card* a_card_ptr) {

	int max_meld_score = INT_MIN;
	int meld_number_played = -1;
	std::vector<std::vector<int>> temp_vector = a_meld_logic_vector;
	for (int i = 0; i < 12; i++) {
		if (m_which_card_used_for_meld[TO9(i)][a_card_ptr->Get_Card_Id()] ==
			false) {
			int& ele = temp_vector[i][a_card_ptr->Get_Card_Id()];
			if (ele > 0) {
				ele--;
			}
		}
	}
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 48; j++) {
			int& ele1 = temp_vector[i][j];
			j++;
			int& ele2 = temp_vector[i][j];
			if (ele1 == 0 && ele2 == 0) {
				break;
			}
			if (j == 47) {	// if no 0's are found in the array then the meld is
							// possible.
				int cur_score = m_meld_scores[TO9(i)];
				if (cur_score > max_meld_score) {
					max_meld_score = cur_score;
					meld_number_played = TO9(i);
				} else if (cur_score == max_meld_score) {
					if (a_card_ptr->Get_Suit() ==
						m_trump_suit_of_current_game) {
						max_meld_score = cur_score;
						meld_number_played = TO9(i);
					}
				}
			}
		}
	}

	return meld_number_played;
}

std::pair<int,int> Player::Get_Best_MeldCardIndexPair_From_Pile(std::vector<std::vector<int>>& a_meld_logic_vector, std::vector<Card*> a_card_pile) {
	int max_meld_score = INT_MIN;
	int best_meld_number = -1;
	int best_meld_index = -1;

	int index = 0;
	for(Card* card_ptr: a_card_pile) {
		int meld_number_played = Get_Best_Meld_If_Card_Thrown(a_meld_logic_vector, card_ptr);
		if(meld_number_played != -1) {
			int cur_score = m_meld_scores[meld_number_played];
			if(cur_score > max_meld_score) {
				max_meld_score = cur_score;
				best_meld_number = meld_number_played;
				best_meld_index = index;
			} else if (cur_score == max_meld_score) {
				if ((card_ptr->Get_Suit() == m_trump_suit_of_current_game &&
					 a_card_pile[best_meld_index]->Get_Suit() !=
						 m_trump_suit_of_current_game) ||
					(a_card_pile[best_meld_index]->Get_Suit() !=
						 m_trump_suit_of_current_game &&
					 card_ptr->Get_Card_Weight() >
						 a_card_pile[best_meld_index]->Get_Card_Weight()) ||
					(a_card_pile[best_meld_index]->Get_Suit() ==
						 m_trump_suit_of_current_game &&
					 card_ptr->Get_Suit() == m_trump_suit_of_current_game &&
					 card_ptr->Get_Card_Weight() >
						 a_card_pile[best_meld_index]->Get_Card_Weight())) {
					max_meld_score = cur_score;
					best_meld_number = meld_number_played;
					best_meld_index = index;
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

	std::pair<int,int> hand_pile_best_meld_pair = Get_Best_MeldCardIndexPair_From_Pile(meld_logic_vector, m_hand_card_pile);

	int best_hand_pile_meld_score = INT_MIN;
	int best_meld_pile_meld_score = INT_MIN;
	if(hand_pile_best_meld_pair.second!= -1) {
		best_hand_pile_meld_score = m_meld_scores[hand_pile_best_meld_pair.second];
	}
	if(hand_pile_best_meld_pair.second == -1) { // No melds found
		return {-1, -1};
	}

	return hand_pile_best_meld_pair;
}

std::pair<std::vector<int>, int> Player::Get_Best_Meld_Cards() {
	std::vector<std::vector<int>> meld_logic_vector = Get_Meld_Logic_Vector();

	add_to_meld_logic_vector(meld_logic_vector, m_hand_card_pile);

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

int Player::Find_Index_In_Pile_From_Card_Id(int a_id) {
	const size_t& hand_pile_size = m_hand_card_pile.size();

	for(int i = 0; i < hand_pile_size; i++) {
		if(m_hand_card_pile[i]->Get_Card_Id() == a_id) {
			return i;
		}
	}

	return -1;
}

int Player::Get_Card_Weight(Card* card_ptr) {
	int face_weight = card_ptr->Get_Card_Weight();
	if(card_ptr->Get_Suit() == m_trump_suit_of_current_game) {
		// add a big number to face weight.
		return face_weight + 100;
	}
	return face_weight;
}

int Player::Find_Index_of_Smallest_Card_Greater_Than_Card(Card* a_card_ptr) {
	const int& hand_pile_size = m_hand_card_pile.size();

	int argument_card_weight = Get_Card_Weight(a_card_ptr);
	int index = -1;
	int min_greatest_card_weight = INT_MAX;
	for (int i = 0; i < hand_pile_size; i++) {
		int cur_card_weight = Get_Card_Weight(m_hand_card_pile[i]);
		if(cur_card_weight > argument_card_weight && cur_card_weight < min_greatest_card_weight) {
			index = i;
			min_greatest_card_weight = cur_card_weight;
		}
	}
	
	return index;
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