#include "card.h"

const char Card::m_suit_char_arr[4] = {'S', 'C', 'H', 'D'};
const char Card::m_face_char_arr[6] = {'9', 'X', 'J', 'Q', 'K', 'A'};
const int Card::m_face_weights[6] = {0, 10, 2, 3, 4, 11};

Card::Card(int a_id) : m_id(a_id) {
	m_card_type = Get_Card_String_Value();
}


// There are 2 cards of the same suit and face.
// There are 6 faces and 4 suits.
// Storing spades first, then clubs, then hearts, then diamonds.
// Within each face two of the same cards are stored side by side and in increasing order.
// Cards are stored in the following order 9S, 9S, 10S, 10S, ..., 9C, ..., 9H, ..., AD, AD
std::string Card::Get_Card_String_Value() const {
	const int suit_val = m_id / 12;
	const int face_val = (m_id % 12) / 2;

	std::string return_string = "  ";
	char f = m_face_char_arr[face_val];
	char b = m_suit_char_arr[suit_val];

	return_string[0] = f;
	return_string[1] = b;
	// std::string return_string = m_face_char_arr[face_val] + m_suit_char_arr[suit_val];
	return return_string;
}

std::string Card::Get_String_From_Id(int a_id) {
	const int suit_val = a_id / 12;
	const int face_val = (a_id % 12) / 2;

	std::string return_string = "  ";
	char f = m_face_char_arr[face_val];
	char b = m_suit_char_arr[suit_val];

	return_string[0] = f;
	return_string[1] = b;
	// std::string return_string = m_face_char_arr[face_val] + m_suit_char_arr[suit_val];
	return return_string;
}

void Card::Print_Card () const {
	const int suit_val = m_id / 12;
	const int face_val = (m_id % 12) / 2;

	std::cout << m_face_char_arr[face_val] << m_suit_char_arr[suit_val] << "  ";
}

int Card::Get_Card_Weight() const {
	return m_face_weights[Get_Face()];
}

int Card::Get_Face() const {
	return (m_id % 12) / 2;
}

int Card::Get_Suit() const {
	return m_id / 12;
}

int Card::Get_Face_From_Id(int a_id) {
	return (a_id % 12) / 2;
}
int Card::Get_Suit_From_Id(int a_id) {
	return a_id / 12;
}

int Card::Get_Weight_From_Id(int a_id) {
	return m_face_weights[Get_Face_From_Id(a_id)];
}

int Card::Get_Card_Id_From_String(std::string &a_card_str) {
	int face = -1, suit = -1;

	switch (a_card_str[0]) {
		case '9': face = 0; break;
		case 'X': face = 1; break;
		case 'J': face = 2; break;
		case 'Q': face = 3; break;
		case 'K': face = 4; break;
		case 'A': face = 5; break;
		default: break;
	}
	switch (a_card_str[1]) {
		case 'S': suit = 0; break;
		case 'C': suit = 1; break;
		case 'H': suit = 2; break;
		case 'D': suit = 3; break;
		default: break;
	}
	return suit * 12 + face * 2;
}

int Card::Get_Card_Id() const {
	return m_id;
}