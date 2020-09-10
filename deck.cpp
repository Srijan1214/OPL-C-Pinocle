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

Deck::Deck() : m_card_list(48) {
	Initialize_Deck_With_48_CARDS();

	// TODO
	// Decided to seed random in this class. Might change later if needed.
	std::srand(unsigned(std::time(0)));

	Shuffle_Deck();
}

Deck::~Deck(){
	Destroy_All_Card_Pointers();
}

void Deck::Shuffle_Deck() {
	std::random_shuffle(m_card_list.begin(),m_card_list.end());
}

Card* Deck::Pop_Top_Card() {
	Card* ret_card = m_card_list.back();
	m_card_list.pop_back();
	return ret_card;
}

void Deck::Put_Card_Back_In_Deck(Card* a_card_ptr) {
	m_card_list.push_back(a_card_ptr);
}

std::string Deck::Get_Stock_String() {
	std::string ret_string = "";
	for(Card* card_ptr: m_card_list) {
		ret_string+= (card_ptr->Get_Card_String_Value() + " ");
	}
	return ret_string;
}