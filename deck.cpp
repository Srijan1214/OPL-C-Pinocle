
/*
 ************************************************************
 * Name: Srijan Prasad Joshi
 * Project: Pinochle C++
 * Class: OPL Fall 20
 * Date: 09/30/2020
 ************************************************************
*/

// Created by Srijan Joshi on 09/10/19.

#include "deck.h"

void Deck::Remove_All_Cards_From_Deck() {
	Destroy_All_Card_Pointers();
	m_card_list.clear();
}

void Deck::Destroy_All_Card_Pointers() {
	for (Card* ele : m_card_list) {
		delete ele;
	}
}

void Deck::Initialize_Deck_With_48_CARDS() {
	for (int i = 0; i < 48; i++) {
		m_card_list[i] = new Card(i);
	}
}

/* *********************************************************************
Function Name: Deck
Purpose: To construct the deck instance, and to call the random seed.
Parameters:
            None
Return Value: None
Assistance Received: none
********************************************************************* */
Deck::Deck() : m_card_list(48) {
	Initialize_Deck_With_48_CARDS();

	std::srand(unsigned(std::time(0)));

	Shuffle_Deck();
}

/* *********************************************************************
Function Name: ~Deck
Purpose: To destroy the deck and delete the cards created by the deck.
Parameters:
            None
Return Value: None
Assistance Received: none
********************************************************************* */
Deck::~Deck(){
	Destroy_All_Card_Pointers();
}

/* *********************************************************************
Function Name: Shuffle_Deck
Purpose: To shuffle the deck.
Parameters:
            None
Return Value: None
Assistance Received: none
********************************************************************* */
void Deck::Shuffle_Deck() {
	std::random_shuffle(m_card_list.begin(),m_card_list.end());
}

/* *********************************************************************
Function Name: Pop_Top_Card
Purpose: To remove top card from the deck and return it.
Parameters:
            None
Return Value: A card* which is the top card in the deck.
Assistance Received: none
********************************************************************* */
Card* Deck::Pop_Top_Card() {
	Card* ret_card = m_card_list.back();
	m_card_list.pop_back();
	return ret_card;
}

/* *********************************************************************
Function Name: Put_Card_Back_In_Deck
Purpose: To put a card that was given to a player back in the deck.
Parameters:
            a_card_ptr, a Card*, which points to a card instance in memory.
Return Value: None
Assistance Received: none
********************************************************************* */
void Deck::Put_Card_Back_In_Deck(Card* a_card_ptr) {
	m_card_list.push_back(a_card_ptr);
}

/* *********************************************************************
Function Name: Get_Stock_String
Purpose: To get the card in the stock, in string representation separated by a space.
Parameters:
            None
Return Value: The average grade in the class, a real value
Assistance Received: none
********************************************************************* */
std::string Deck::Get_Stock_String() const {
	std::string ret_string = "";
	for (int i = m_card_list.size() - 1; i >= 0; i-- ) {
		Card* card_ptr = m_card_list[i];
		ret_string+= (card_ptr->Get_Card_String_Value() + " ");
	}
	return ret_string;
}

/* *********************************************************************
Function Name: Load_Stock_Pile_From_String
Purpose: To load the stock string stored in the serialization file into the deck.
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
void Deck::Load_Stock_Pile_From_String(const std::string &a_stock_string, std::vector<bool> &a_cards_that_have_been_used) {
	std::stringstream s(a_stock_string);
	std::string card_str;
	while (std::getline(s, card_str, ' ')) {
		int id = Card::Get_Card_Id_From_String(card_str);
		if(a_cards_that_have_been_used[id]) {
			id+=1;
		}
		a_cards_that_have_been_used[id] = true;
		m_card_list.push_back(new Card(id));
	}

	std::reverse(m_card_list.begin(), m_card_list.end());
}

/* *********************************************************************
Function Name: Get_Stock_Size
Purpose: Getter for the number of cards remaining in the stock.
Parameters:
            None
Return Value: The size of the m_card_list.
Assistance Received: none
********************************************************************* */
int Deck::Get_Stock_Size() const {
	return m_card_list.size();
}
