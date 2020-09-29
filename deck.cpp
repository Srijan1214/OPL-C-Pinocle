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
	// std::srand(17);
	// std::srand(19);

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
	for (int i = m_card_list.size() - 1; i >= 0; i-- ) {
		Card* card_ptr = m_card_list[i];
		ret_string+= (card_ptr->Get_Card_String_Value() + " ");
	}
	return ret_string;
}

void Deck::Load_Stock_Pile_From_String(std::string &a_stock_string, std::vector<bool> &a_cards_that_have_been_used) {
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

int Deck::Get_Stock_Size() {
	return m_card_list.size();
}
