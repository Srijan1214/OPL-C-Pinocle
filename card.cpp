#include "card.h"

Card::Card(int a_id) : m_id(a_id) {

}


// There are 2 cards of the same suit and face.
// There are 6 faces and 4 suits.
// Storing spades first, then clubs, then hearts, then diamonds.
// Within each face two of the same cards are stored side by side and in increasing order.
// Cards are stored in the following order 9S, 9S, 10S, 10S, ..., 9C, ..., 9H, ..., AD, AD
std::string Card::Get_Card_String_Value() {
	const int suit_val = m_id / 12;
	const int face_val = (m_id % 12) / 2;

	std::string return_string = " ";
	return_string[0] = m_face_char_arr[face_val];
	return_string[1] = m_suit_char_arr[suit_val];

	return return_string;
}

void Card::Print_Card () {
	const int suit_val = m_id / 12;
	const int face_val = (m_id % 12) / 2;

	std::cout << m_face_char_arr[face_val] << m_suit_char_arr[suit_val] << "  ";
}

