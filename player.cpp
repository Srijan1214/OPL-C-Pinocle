
/*
 ************************************************************
 * Name: Srijan Prasad Joshi
 * Project: Pinochle C++
 * Class: OPL Fall 20
 * Date: 09/30/2020
 ************************************************************
*/

// Created by Srijan Joshi on 09/10/19.

#include "player.h"

// The names of meld for each meld number. E.g meld number 0 means "Flush".
const std::string Player::m_meld_names[9] = {"Flush",
											 "Royal Marriage",
											 "Marriage",
											 "Dix",
											 "Four Aces",
											 "Four Kings",
											 "Four Queens",
											 "Four Jacks",
											 "Pinochle"};

// The scores earned for each meld. The index of each meld corresponds to m_meld_names.
const int Player::m_meld_scores[9] = {150, 40, 20, 10, 100, 80, 60, 40, 40};

/* *********************************************************************
Function Name: Player
Purpose: The constructor for the player instance.
Parameters:
            None
Return Value: None
Assistance Received: none
********************************************************************* */
Player::Player()
	: m_current_meld_cards(12),
	  m_help_mode(false),
	  m_no_of_times_meld_has_been_played(9, 0),
	  m_which_card_used_for_meld(9, std::vector<bool>(48, false)),
	  m_trump_suit_of_current_game(-1) {}

/* *********************************************************************
Function Name: Give_Card_To_Player
Purpose: To get the card, which is gotten from the deck, given by the round and put it in hand.
Parameters:
            a_card_ptr, a Card*, that points to a Card instance.
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::Give_Card_To_Player(Card* a_card_ptr) {
	m_hand_card_pile.push_back(a_card_ptr);
	m_hand_meld_involvement_list.push_back({});
}

/* *********************************************************************
Function Name: Add_Cards_To_Player_Capture_Pile
Purpose: To add the two cards to the capture pile when player wins the round.
Parameters:
            a_card_ptr1, a Card*, that points to a Card instance.
            a_card_ptr2, a Card*, that points to a Card instance.
Return Value: The average grade in the class, a real value
Assistance Received: none
********************************************************************* */
void Player::Add_Cards_To_Player_Capture_Pile(Card* a_card_ptr1,
											  Card* a_card_ptr2) {
	m_capture_card_pile.push_back(a_card_ptr1);
	m_capture_card_pile.push_back(a_card_ptr2);
}

/* *********************************************************************
Function Name: Set_Trump_card
Purpose: To tell player which card is the trump card in the round.
Parameters:
            a_trump_card, a int, which tell the card id for the trump card.
Return Value: The average grade in the class, a real value
Assistance Received: none
********************************************************************* */
void Player::Set_Trump_card(int a_trump_card) {
	m_trump_card_id = a_trump_card;
}

/* *********************************************************************
Function Name: Get_Meld_Type_12_From_Cards
Purpose: To get a meld_12 number given by the cards in the a_meld_card_list.
Parameters:
            a_meld_card_list, the vector of card*, passed by const reference, that tells the cards queried for meld. 
Return Value: The meld number given by the cards. Returns -1 if meld invalid.
Local Variables:
            meld_card_identifiers, a vector of int, that translates the Card* to its id.
Algorithm:
            1) Creates a int vector from the Card* vector.
            2) Calls the overloaded Get_Meld_Type_12_From_Cards which takes in a int vector instead.
Assistance Received: none
********************************************************************* */
int Player::Get_Meld_Type_12_From_Cards(const std::vector<Card*>& a_meld_card_list) const {
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

	return Get_Meld_Type_12_From_Cards(meld_card_identifiers);
}

/* *********************************************************************
Function Name: Get_Meld_Type_12_From_Cards
Purpose: To get a meld_12 number given by the cards in the a_meld_card_identifiers.
Parameters:
            a_meld_card_identifiers, the vector of int that tells the ids for the cards queried for meld. 
				This vector is sorted and hence modified by the function.
Return Value: The meld number given by the cards. Returns -1 if meld invalid.
Algorithm:
            1) Sorts the a_meld_card_identifiers.
            2) Checks the size of the vector.
            3) Has if conditions for each size of the vector.
            4) Checks if each id inside the vectors matches up to any of the expected melds.
            5) For e.g. a pinochle meld requires the card id vector to be {id of QS, id of JD}
            5) Does this checking for all possible melds and returns the number of the meld if match found.
			6) Returns an out of range value if the ids do not produce any melds.
			7) Does not take meld history into account.
Assistance Received: none
********************************************************************* */
int Player::Get_Meld_Type_12_From_Cards(std::vector<int>& a_meld_card_identifiers) const {
	// contains logic to detect if meld exists


	std::sort(a_meld_card_identifiers.begin(),a_meld_card_identifiers.end());

	//Check for flush
	if(a_meld_card_identifiers.size() == 5) {
		const int ten_face = Card::Get_Face_From_Id(a_meld_card_identifiers[0]);
		const int ten_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[0]);

		const int jack_face = Card::Get_Face_From_Id(a_meld_card_identifiers[1]);
		const int jack_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[1]);

		const int queen_face = Card::Get_Face_From_Id(a_meld_card_identifiers[2]);
		const int queen_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[2]);

		const int king_face = Card::Get_Face_From_Id(a_meld_card_identifiers[3]);
		const int king_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[3]);

		const int ace_face = Card::Get_Face_From_Id(a_meld_card_identifiers[4]);
		const int ace_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[4]);

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
	} else if (a_meld_card_identifiers.size() == 2) {
		const int first_face = Card::Get_Face_From_Id(a_meld_card_identifiers[0]);
		const int first_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[0]);

		const int second_face = Card::Get_Face_From_Id(a_meld_card_identifiers[1]);
		const int second_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[1]);

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
	}else if(a_meld_card_identifiers.size() == 1) {
		//dix
		const int nine_face = Card::Get_Face_From_Id(a_meld_card_identifiers[0]);
		const int nine_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[0]);
		if(nine_face == 0 && nine_suit == m_trump_suit_of_current_game) {
			return 6;
		}
	} else if (a_meld_card_identifiers.size() == 4) {
		const int first_face = Card::Get_Face_From_Id(a_meld_card_identifiers[0]);
		const int first_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[0]);

		const int second_face = Card::Get_Face_From_Id(a_meld_card_identifiers[1]);
		const int second_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[1]);

		const int third_face = Card::Get_Face_From_Id(a_meld_card_identifiers[2]);
		const int third_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[2]);

		const int fourth_face = Card::Get_Face_From_Id(a_meld_card_identifiers[3]);
		const int fourth_suit = Card::Get_Suit_From_Id(a_meld_card_identifiers[3]);

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

/* *********************************************************************
Function Name: Set_Trump_Suit
Purpose: To tell the player what the trump suit of the current round is.
Parameters:
            a_trump_suit, an int passed by value, that tells the suit number of the trump suit.
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::Set_Trump_Suit(int a_trump_suit) {
	m_trump_suit_of_current_game = a_trump_suit;
}

/* *********************************************************************
Function Name: Update_Meld_History
Purpose: Updates the meld history given the meld number and the cards used for that meld.
Parameters:
            a_meld_card_list, a card* vector, passed by const reference, that tells the cards used for a meld.
            a_meld_number_9, the meld number in 9 index format, that tells the meld played.
Return Value: None
Algorithm:
            1) increases the m_no_of_times_meld_has_been_played for the meld played.
            2) Loops through the card ids.
            3) Sets the m_which_card_used_for_meld to true, for the card ids of the meld number. 
            4) If Royal flush is the meld number, the indexes for the Royal Marriages are also set to true.
Assistance Received: none
********************************************************************* */
void Player::Update_Meld_History(const std::vector<Card*> & a_meld_card_list, int a_meld_number_9) {
	m_no_of_times_meld_has_been_played[a_meld_number_9]++;
	for(Card* card_ptr: a_meld_card_list) {
		m_which_card_used_for_meld[a_meld_number_9][card_ptr->Get_Card_Id()] = true;
		// if Royal Flush declared, then don't allow Royal Marriage of the same cards for the rest of the game
		if(a_meld_number_9 == 0) {
			m_which_card_used_for_meld[1][card_ptr->Get_Card_Id()] = true;
		}
	}
}

/* *********************************************************************
Function Name: Is_Meld_Allowed_By_History
Purpose: To check if the cards used in a meld are allowed by history so that duplicate cards cannot be played
			for the same meld.
Parameters:
			a_meld_card_list, a card* vector, passed by const reference, that tells the cards used for a meld.
            a_meld_number_9, an int passed by value, that tells the meld played.
Return Value: True if meld is allowed by history. False if not.
Algorithm:
            1) Returns true if the meld has not been played yet.
            2) Checks if all the cards in m_which_card_used_for_meld for the meld is false.
Assistance Received: none
********************************************************************* */
bool Player::Is_Meld_Allowed_By_History(std::vector<Card*> & a_meld_card_list, int a_meld_number_9) const {
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

/* *********************************************************************
Function Name: Get_Console_Message
Purpose: To give the information about the hand pile, capture pile and melds in string format.
Parameters:
            None
Return Value: A string containing all the information about the player's cars.
Assistance Received: none
********************************************************************* */
std::string Player::Get_Console_Message() const{
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

/* *********************************************************************
Function Name: Get_Hand_Pile_String
Purpose: To Get the hand pile in string format.
Parameters:
			None

Return Value: The hand pile in string format.
Assistance Received: none
********************************************************************* */
std::string Player::Get_Hand_Pile_String() const {
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

/* *********************************************************************
Function Name: Get_Capture_Pile_String
Purpose: To Get the capture pile in string format.
Parameters:
			None

Return Value: The capture pile in string format.
Assistance Received: none
********************************************************************* */
std::string Player::Get_Capture_Pile_String() const {
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

/* *********************************************************************
Function Name: Get_Melds_String
Purpose: To Get the melds in string format.
Parameters:
			None

Return Value: The melds in string format.
Assistance Received: none
********************************************************************* */
std::string Player::Get_Melds_String() const {
	std::string message;
	for(int i = 0; i < m_current_meld_cards.size(); i++) {
		for(auto& current_melds: m_current_meld_cards[i]) {
			for(const int& hand_index: current_melds) {
				message += m_hand_card_pile[hand_index]->Get_Card_String_Value();
				if(m_hand_meld_involvement_list[hand_index].size() > 1) {
					message += "*";
				}
				if(i == m_current_meld_cards.size() - 1 && hand_index == current_melds.back()) {
					continue;	// do not add the last space if is the last character.
				}
				if(!(&hand_index == &(current_melds.back()))) {
					message += " ";
				}
			}
			message += ", ";
		}
	}
	if(!message.empty() && message.back() == ' '){
		message.pop_back();
	}
	if(!message.empty() && message.back() == ',') {
		message.pop_back();
	}
		return message;
}

/* *********************************************************************
Function Name: Load_Members_From_Serialization_String
Purpose: To load the cards string stored in the serialization file into the deck.
Parameters:
            a_serialization_string, the string value passed by const reference, that stores the all the cards for the player.
            a_cards_that_have_been_used, a vector of bool passed by reference, that tells us which cards id's has already been
				used by the serialization loading of players. Helpful since two of the same card strings cannot
				have the same id.
				After a new card id is used, the particular index for that id is set to true.
Local Variables:
            card_str, a string that temporarily stores the card string of a particular card.
            s, a stringstream that helps split the argument string
Algorithm:
            1) Loop through all the card strings from the a_stock_string.
            2) Parse the strings for hands, captures and melds and pass them into functions that loads according
			to those strings.
Assistance Received: none
********************************************************************* */
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

/* *********************************************************************
Function Name: Load_Hand_Cards_From_String
Purpose: To load the meld string stored in the serialization file into the deck.
Parameters:
            a_meld_string, the string value passed by const reference, that stores the stock cards.
            a_cards_that_have_been_used, a vector of bool passed by reference, that tells us which cards id's has already been
				used by the serialization loading of players. Helpful since two of the same card strings cannot
				have the same id.
				After a new card id is used, the particular index for that id is set to true.
Local Variables:
            card_str, a string that temporarily stores the card string of a particular card.
            s, a stringstream that helps split the argument string
Algorithm:
            1) Loop through all the card strings from the a_stock_string.
            2) Get the id of that card and change the id if it is already used.
            3) Create a new card with that id to play the game.
            4) Set the index represented by the id in a_cards_that_have_been_used to true.
Assistance Received: none
********************************************************************* */
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

/* *********************************************************************
Function Name: Load_Capture_Cards_From_String
Purpose: To load the capture string stored in the serialization file into the deck.
Parameters:
            a_stock_string, the string value passed by const reference, that stores the stock cards.
            a_cards_that_have_been_used, a vector of bool passed by reference, that tells us which cards id's has already been
				used by the serialization loading of players. Helpful since two of the same card strings cannot
				have the same id.
				After a new card id is used, the particular index for that id is set to true.
Local Variables:
            card_str, a string that temporarily stores the card string of a particular card.
            s, a stringstream that helps split the argument string
Algorithm:
            1) Loop through all the card strings from the a_stock_string.
            2) Get the id of that card and change the id if it is already used.
            3) Create a new card with that id to play the game.
            4) Set the index represented by the id in a_cards_that_have_been_used to true.
Assistance Received: none
********************************************************************* */
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

/* *********************************************************************
Function Name: Load_Meld_Cards_From_String
Purpose: To load the stock string stored in the serialization file into the deck.
Parameters:
            a_meld_string, the string value passed by const reference, that stores the meld cards.
            a_cards_that_have_been_used, a vector of bool passed by reference, that tells us which cards id's has already been
				used by the serialization loading of players. Helpful since two of the same card strings cannot
				have the same id.
				After a new card id is used, the particular index for that id is set to true.
Local Variables:
            card_str, a string that temporarily stores the card string of a particular card.
            s, a stringstream that helps split the argument string
Algorithm:
            1) Loop through all the card strings from the a_meld_string.
            2) Get the id of that card and change the id if it is already used.
            3) Create a new card with that id to play the game.
            4) Set the index represented by the id in a_cards_that_have_been_used to true.
			5) Get the meld numbers represented by those ids.
			6) If there is a *, then the meld ids are same.
			7) Make appropriate changes to the m_hand_meld_involvement_list and m_current_meld_cards
			   so that the appropriate graph needed for the melds is created.
Assistance Received: none
********************************************************************* */
void Player::Load_Meld_Cards_From_String(std::string &a_meld_string, std::vector<bool> &a_cards_that_have_been_used) {
	std::vector<std::vector<int>> logic_vector = Get_Meld_Logic_Vector();
	logic_vector[2 + m_trump_suit_of_current_game] = std::vector<int>(48,-1);
	
	std::stringstream s(a_meld_string);
	std::string card_str;
	std::vector<std::vector<std::string>> meld_card_strings;
	std::vector<int> meld_numbers_12_vec;
	std::vector<int> cur_meld_ids;

	while (std::getline(s, card_str, ' ')) {
		if(card_str == " ")  {
			continue;
		}
		int id = Card::Get_Card_Id_From_String(card_str);
		cur_meld_ids.push_back(id);
		if(meld_card_strings.empty()) {
			meld_card_strings.push_back({});
		}
		if(card_str.back() == ',') {
			meld_numbers_12_vec.push_back(Get_Meld_Type_12_From_Cards(cur_meld_ids));
			cur_meld_ids.clear();
			card_str.pop_back();
			meld_card_strings.back().push_back(card_str);
			meld_card_strings.push_back({});
		} else {
			meld_card_strings.back().push_back(card_str);
		}
	}
	meld_numbers_12_vec.push_back(Get_Meld_Type_12_From_Cards(cur_meld_ids));

	// Now I have the data structure of meld_card_strings 
	// and meld_numbers_12_vec to help me give the correct id to each card string.

	// a lambda function to change the id to a duplicate card
	auto get_id_for_duplicate_cards = [](int a_id){
		if(a_id % 2 == 0){
			return a_id + 1;
		} else {
			return a_id - 1;
		}
	};

	// the following will keep the history of each card and 
	// tells what id the current card should be.
	std::vector<int> id_to_change_cur_card_to(48, -1);
	std::vector<std::vector<int>> meld_card_ids;

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
				id = get_id_for_duplicate_cards(id);
			} else if (card_str.size() == 3) {
				if(count_of_ids_for_stared_cards[id] == -1) {
					count_of_ids_for_stared_cards[id] = 1;
				} else{
					count_of_ids_for_stared_cards[id]++;
				}
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

/* *********************************************************************
Function Name: Get_Meld_Logic_Vector
Purpose: To create a logic vector that tells which card ids are valid for each melds.
Parameters:
			None
Return Value: The appropriate meld vector logic for that round.
Local Variables:
            all_requirements, a vector of vector int of that stores the card ids needed for each meld.
            , a vector of vector int of that stores the card ids needed for each meld.
			meld_logic_vector, a 12 * 48, vector of vector of int that will be returned after appropriate changes
			have been made to it.
Algorithm:
            1) Creates a 12 * 48 vector representing the 12 melds and 48 cards. With initial value of -1.
            2) Creates a requirements vector of size 12 which tells the id of the cards needed for each meld.
            3) Sets the appropriate id indexes for each meld inside the 12 * 48 vector using the requirements
				to a value of 0. This indicates that this card can be used for this meld.
Assistance Received: none
********************************************************************* */
std::vector<std::vector<int>> Player::Get_Meld_Logic_Vector() const{
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

/* *********************************************************************
Function Name: TO9
Purpose: A utility function that convert a 12 index meld number to the standard 9 index meld number.
		I created the 12 index meld to make marriage make marriage of every suit seem like a different marriage.
Parameters:
            a_12_based_index, a int passed by value, that tells the 12 index meld number.
Return Value: The average grade in the class, a real value
Assistance Received: none
********************************************************************* */
int Player::TO9(int a_12_based_index) const{
	if (a_12_based_index < 2) {
		return a_12_based_index;
	} else if (a_12_based_index >= 2 && a_12_based_index <= 5) {
		return 2;
	} else {
		return a_12_based_index - 3;
	}
}

/* *********************************************************************
Function Name: add_to_meld_logic_vector
Purpose: To add all the cards to the meld logic vector to find out all the possible melds with the current hand.
Parameters:
            add_to_meld_logic_vector, a vector of vector of int, passed by value, that tells the number of required cards 
				available in hand for each meld. If card is not required then the value is -1.
				The ids for the required meld for each card in the pile will be incremented in the meld logic vector.
			a_card_pile: The card pile that contains the cards which will be added the meld logic vector.
Return Value: The average grade in the class, a real value
Algorithm:
            1) Increment the ids of all the cards in the logic vector, for each row, if the value is not already -1.
Assistance Received: none
********************************************************************* */
void Player::add_to_meld_logic_vector(std::vector<std::vector<int>>& a_meld_logic_vector, const std::vector<Card*>& a_card_pile) const {
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

/* *********************************************************************
Function Name: Update_Logic_Vector_With_History
Purpose: To add all the cards in the history vector to the meld logic vector 
         to find out if a card id have already been used for a meld.
Parameters:
            add_to_meld_logic_vector, a vector of vector of int, passed by value, that tells the number of required cards 
				available in hand for each meld. If card is not required then the value is -1.
				The ids for the required meld for each card in the pile will be incremented in the meld logic vector.
Return Value: The average grade in the class, a real value
Algorithm:
            1) Increment the ids of all the card id that have been used for meld 
			in the logic vector, for each row.
Assistance Received: none
********************************************************************* */
void Player::Update_Logic_Vector_With_History(std::vector<std::vector<int>>& a_meld_logic_vector) const {
	for(int i = 0; i < 12; i ++) {
		for(int id = 0; id < 48; id++) {
			if (m_which_card_used_for_meld[TO9(i)][id] == true) {
				a_meld_logic_vector[i][id] = 0;
			}
		}
	}
}

/* *********************************************************************
Function Name: Get_Best_Meld_If_Card_Thrown
Purpose: To get the best meld if a card is thrown.
Parameters:
            add_to_meld_logic_vector, a vector of vector of int, passed by value, that tells the number of required cards 
				available in hand for each meld. If card is not required then the value is -1.
			a_card_ptr, the card pointer passed by value, that we are simulating throwing.
Return Value: the meld number of the best meld. Returns out of bounds meld if no meld is possible.
Algorithm:
            1) Subtracts the card from the meld vector logic accordingly.
            2) Now searches which melds are possible from the meld vector logic and return the max meld value.
Assistance Received: none
********************************************************************* */
int Player::Get_Best_Meld_If_Card_Thrown(
	std::vector<std::vector<int>>& a_meld_logic_vector, Card* a_card_ptr) const{

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

/* *********************************************************************
Function Name: Get_Best_MeldCardIndexPair_From_Logic
Purpose: To get the best card ids to play for a meld along with it meld number.
Parameters:
			a_meld_logic_vector, the logic vector passed by const reference containing information of all the hand file cards.
Return Value: a pair of int, int with the first being the best meld index and the second being the best meld number.
Local Variables:
            max_meld_score, the max meld score from the hand.
            best_meld_number, the meld number corresponding to the max_meld_score
            best_meld_index, the index of the card belonging to hte meld.
            best_card_weight, the card weight for the best card.
Algorithm:
            1) Simulate throwing every card and find the best meld for each throw.
			2) Get the max among all throws and return the best pair.
Assistance Received: none
********************************************************************* */
std::pair<int,int> Player::Get_Best_MeldCardIndexPair_From_Logic(std::vector<std::vector<int>>& a_meld_logic_vector) const{
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

/* *********************************************************************
Function Name: Find_IndexMeldPair_Of_Card_To_Throw
Purpose: To get the best card index to play for a meld along with it meld number.
Parameters:
			None
Return Value: a pair of int, int with the first being the best meld index and the second being the best meld number.
Assistance Received: none
********************************************************************* */
std::pair<int, int> Player::Find_IndexMeldPair_Of_Card_To_Throw() const {
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

/* *********************************************************************
Function Name: Get_Indexes_And_Meld_Number12_Best_Meld
Purpose: To get the best card ids to play for a meld along with it meld number.
Parameters:
			a_meld_logic_vector, the logic vector passed by const reference containing information of all the hand file cards.
Return Value: a pair of vector<int>, int, with the first being the best meld ids needed for the meld
				and the second being the best meld number.
Local Variables:
            max_meld_score, the max meld score from the hand.
Algorithm:
            1) Simulate throwing every card and find the best meld for each throw.
			2) Get the max among all throws and return the best pair.
Assistance Received: none
********************************************************************* */
std::pair<std::vector<int>, int> Player::Get_Indexes_And_Meld_Number12_Best_Meld() const{
	std::vector<std::vector<int>> meld_logic_vector = Get_Meld_Logic_Vector();

	add_to_meld_logic_vector(meld_logic_vector, m_hand_card_pile);

	// update meld_logic_according to meld history.
	Update_Logic_Vector_With_History(meld_logic_vector);
	

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
				if(j % 2 == 0) {
					j++;
				}
			}
		}
		return {return_vector, (meld_number_played)};
	}
}

/* *********************************************************************
Function Name: Search_Card_In_Pile
Purpose: Get index of card id in pile
Parameters:
            a_id, an int passed by value, that gives the id for the best index.
Return Value: The index of the found id in hand.
Algorithm:
            1) Serially search and give the index for the id.
Assistance Received: none
********************************************************************* */
int Player::Search_Card_In_Pile(int a_id) const {
	const size_t& hand_pile_size = m_hand_card_pile.size();

	for(int i = 0; i < hand_pile_size; i++) {
		if(m_hand_card_pile[i]->Get_Card_Id() == a_id) {
			return i;
		}
	}

	return -1;
}

/* *********************************************************************
Function Name: Is_First_Card_Greater_Than_Lead
Purpose: Get index of card id in pile
Parameters:
            a_card_first, the first card id.
            a_card_lead, the lead card id.
Return Value: True if first card bigger than lead.
Assistance Received: none
********************************************************************* */
bool Player::Is_First_Card_Greater_Than_Lead(int a_card_first, int a_card_lead) const{
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

/* *********************************************************************
Function Name: Find_Index_of_Smallest_Card_Greater_Than_Card
Purpose: Get the index of smallest card greter than the lead player's card.
Parameters:
            a_card_ptr, a card*, that tells the lead player's card.
Return Value: -1 if no card is greater than lead's. Else, the index of the smallest greater card.
Assistance Received: none
********************************************************************* */
int Player::Find_Index_of_Smallest_Card_Greater_Than_Card(Card* a_card_ptr) const {
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

/* *********************************************************************
Function Name: Find_Index_Of_Smallest_Card
Purpose: Get the index of smallest card in hand.
Parameters:
			None
Return Value: The index of the smallest card.
Assistance Received: none
********************************************************************* */
int Player::Find_Index_Of_Smallest_Card() const {
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

/* *********************************************************************
Function Name: Find_Index_Of_Greatest_Card
Purpose: Get the index of greatest card in hand.
Parameters:
			None
Return Value: The index of the greatest card.
Assistance Received: none
********************************************************************* */
int Player::Find_Index_Of_Greatest_Card() const {
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

/* *********************************************************************
Function Name: Get_Card_Weight
Purpose: To get card weight according to the round.
Parameters:
			card_ptr, a Card*, whose weight we need to find.
Return Value: The weight of the card according to the round. Trump card's weight is always highest.
Assistance Received: none
********************************************************************* */
int Player::Get_Card_Weight(Card* card_ptr) const {
	int face_weight = card_ptr->Get_Card_Face_Weight();
	if(card_ptr->Get_Suit() == m_trump_suit_of_current_game) {
		// add a big number to face weight.
		return face_weight + 100;
	}
	return face_weight;
}

/* *********************************************************************
Function Name: Remove_Card_From_Pile
Purpose: To handle the removal of a card so that the melds are adjucted and the card pointers are removed
			out of the member vectors.
Parameters:
			a_index, the index int passed by value, that tells the index of the card to remove.
Return Value: None
Algorithm:
            1) Deletes connections in the graph created by m_hand_meld_involvement_list and m_current_meld_cards.
            2) Removes the card from m_hand_card_pile
Assistance Received: none
********************************************************************* */
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
		cards_to_remove_from_meld = m_current_meld_cards[removal_index_to_meld_pointer_triplet[0]].back();
		m_current_meld_cards[removal_index_to_meld_pointer_triplet[0]].pop_back();
	}	

	// Change the meld lists pointing to hand at the back to change location of last card to the one that is about to be removed. 
	if(a_index != m_hand_meld_involvement_list.size() - 1) {
		for(auto& ele: m_hand_meld_involvement_list.back()) {
			m_current_meld_cards[ele[0]][ele[1]][ele[2]] = a_index;
		}
	}
	// Remove Card Completely from the Hand
	m_hand_card_pile[a_index] = m_hand_card_pile.back();
	m_hand_card_pile.pop_back();
	m_hand_meld_involvement_list[a_index] = m_hand_meld_involvement_list.back();
	m_hand_meld_involvement_list.pop_back();
}

/* *********************************************************************
Function Name: Get_No_Of_Remaining_Cards
Purpose: Get the number of player cards.
Parameters:
			None
Return Value: The size of m_hand_card_pile
Assistance Received: none
********************************************************************* */
int Player::Get_No_Of_Remaining_Cards() const {
	return m_hand_card_pile.size();
}

/* *********************************************************************
Function Name: Pop_All_Cards_From_Capture_Pile
Purpose: Give all the cards in capture pile back to the round.
Parameters:
			None
Return Value: A vector of Card* in the capture pile.
Assistance Received: none
********************************************************************* */
std::vector<Card*> Player::Pop_All_Cards_From_Capture_Pile() {
// Only called when the round ends and the cards are placed back on the deck
	std::vector<Card*> ret_vec = m_capture_card_pile;
	m_capture_card_pile.clear();
	return ret_vec;
}

/* *********************************************************************
Function Name: Reset_Meld_History
Purpose: Reset_Meld_History for the next round.
Parameters:
			None
Return Value: None
Assistance Received: none
********************************************************************* */
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


/* *********************************************************************
Function Name: Turn_On_Help_Mode
Purpose: Set the m_help_mode to true to turn on help.
Parameters:
			None
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::Turn_On_Help_Mode() {
	m_help_mode = true;
}


/* *********************************************************************
Function Name: Turn_Off_Help_Mode
Purpose: Set the m_help_mode to false to turn off help.
Parameters:
			None
Return Value: None
Assistance Received: none
********************************************************************* */
void Player::Turn_Off_Help_Mode() {
	m_help_mode = false;
}
