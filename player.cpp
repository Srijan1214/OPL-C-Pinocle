#include "player.h"

const int Player::m_meld_scores[9] = {150, 40, 20, 10, 100, 80, 60, 40, 40};

Player::Player()
	: m_no_of_times_meld_has_been_played(9, 0),
	  m_which_card_used_for_meld(9, std::vector<bool>(48, false)),
	  m_trump_suit_of_current_game(-1) {}

void Player::Give_Card_To_Player(Card* a_card_ptr) {
	m_hand_card_pile.push_back(a_card_ptr);
}

// Card* Player::Get_Card_To_Play(Card* a_lead_card) {
// 	Card* ret_card = m_hand_card_pile.back();
// 	m_hand_card_pile.pop_back();
// 	return ret_card;
// }

void Player::Add_Cards_To_Player_Capture_Pile(Card* a_card_ptr1,
											  Card* a_card_ptr2) {
	m_capture_card_pile.push_back(a_card_ptr1);
	m_capture_card_pile.push_back(a_card_ptr2);
}

bool Player::Is_Meld_Valid(std::vector<Card*>& a_meld_card_list) {
	int meld_number = Get_Meld_Type_From_Cards(a_meld_card_list);
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

int Player::Get_Meld_Type_From_Cards(std::vector<Card*>& a_meld_card_list) {
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
				return 2;
			}
		}
		// If not a marriage then can be a Pinochle
		if (first_face == 3 && second_face == 2 && first_suit == 0 &&
			second_suit == 3) {
				return 8;
		}
	}else if(meld_card_identifiers.size() == 1) {
		//dix
		const int nine_face = Card::Get_Face_From_Id(meld_card_identifiers[0]);
		const int nine_suit = Card::Get_Suit_From_Id(meld_card_identifiers[0]);
		if(nine_face == 0 && nine_suit == m_trump_suit_of_current_game) {
			return 3;
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
				return 4;
			}
			//four kings
			if (first_face == 4 && second_face == 4 && third_face == 4 &&
				fourth_face == 4) {
				return 5;
			}
			//four queens
			if (first_face == 3 && second_face == 3 && third_face == 3 &&
				fourth_face == 3) {
				return 6;
			}
			//four Jacks
			if (first_face == 2 && second_face == 2 && third_face == 2 &&
				fourth_face == 2) {
				return 7;
			}
		}
	}
	return 9;
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
	for(Card* card_ptr:m_hand_card_pile) {
		message+= card_ptr->Get_Card_String_Value() + ' ';
	}
	message+= "\n Capture Pile : ";
	for(Card* card_ptr:m_capture_card_pile) {
		message+= card_ptr->Get_Card_String_Value() + ' ';
	}

	message+= "\n Melds: ";
	for(Card* card_ptr:m_meld_card_pile) {
		message+= card_ptr->Get_Card_String_Value() + ' ';
	}
	message+="\n";
	return message;
}