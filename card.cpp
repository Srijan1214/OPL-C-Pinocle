
/*
 ************************************************************
 * Name: Srijan Prasad Joshi
 * Project: Pinochle C++
 * Class: OPL Fall 20
 * Date: 09/30/2020
 ************************************************************
*/

// Created by Srijan Joshi on 09/10/19.

#include "card.h"

// Converts a suit number to the suits. E.g, here suit of 0 means spades.
const char Card::m_suit_char_arr[4] = {'S', 'C', 'H', 'D'};

// Converts a face number to the faces. E.g, here face of 0 means nine.
const char Card::m_face_char_arr[6] = {'9', 'X', 'J', 'Q', 'K', 'A'};

// Converts a face number to the weight/points held by the face. E.g, here face of 0, which is nine, holds 0 weight.
const int Card::m_face_weights[6] = {0, 10, 2, 3, 4, 11};


/**********************************************************************
 * Function Name: Card
 * Purpose: To construct the card object
 * Parameters:
 *              a_id, an int passed by value, that tells which card it is.
 *
 * Assistance Received: None
 **********************************************************************/
Card::Card(int a_id) : m_id(a_id) {
}


/**********************************************************************
* Function Name: Get_Card_String_Value
* Purpose: Get the string representation of the card.
* Parameters:
*              None
* Return Value: The string representation of the Card.
* Algorithm:
*              1) Divide the Id by 12 to get the suit number.
*              2) Mod the card Id by 12 and then divide that number
*              by 2 to get the face number. 
*              3) Get the characters needed from the m_suit_char_arr and m_face_char_arr
* Assistance Received: None
**********************************************************************/
std::string Card::Get_Card_String_Value() const {
	const int suit_val = m_id / 12;
	const int face_val = (m_id % 12) / 2;

	std::string return_string = "  ";
	char f = m_face_char_arr[face_val];
	char b = m_suit_char_arr[suit_val];

	return_string[0] = f;
	return_string[1] = b;
	return return_string;
}

/**********************************************************************
* Function Name: Get_String_From_Id
* Purpose: Static Utility function to get the string representation from an Id.
* Parameters:
*              a_id, an int passed by value. It holds the id to be converted.
* Return Value: The string representation that the ID represents.
* Algorithm:
*              1) Divide the a_id by 12 to get the suit number.
*              2) Mod the card Id by 12 and then divide that number
*              by 2 to get the face number. 
*              3) Get the characters needed from the m_suit_char_arr and m_face_char_arr
* Assistance Received: None
**********************************************************************/
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

/* *********************************************************************
Function Name: Get_Card_Face_Weight
Purpose: To get the weight of the card's face.
Parameters:
            None

Return Value: The points earned when a card type of this face is eaten.
Assistance Received: none
********************************************************************* */
int Card::Get_Card_Face_Weight() const {
	return m_face_weights[Get_Face()];
}

/* *********************************************************************
Function Name: Get_Face
Purpose: To get the face number of the card.
Parameters:
            None

Return Value: The face number of the card.
Assistance Received: none
********************************************************************* */
int Card::Get_Face() const {
	return (m_id % 12) / 2;
}

/* *********************************************************************
Function Name: Get_Suit
Purpose: To get the suit number of the card.
Parameters:
            None

Return Value: The suit number of the card.
Assistance Received: none
********************************************************************* */
int Card::Get_Suit() const {
	return m_id / 12;
}

/* *********************************************************************
Function Name: Get_Face_From_Id
Purpose: Static Utility function to get the face number of a card id.
Parameters:
*           a_id, an int passed by value. It holds the id that the card represents
*
Return Value: The face number of the id.
Assistance Received: none
********************************************************************* */
int Card::Get_Face_From_Id(int a_id) {
	return (a_id % 12) / 2;
}


// -------------------------------------------------------------------------------------------------------//
/* *********************************************************************
Function Name: Get_Face_From_Id
Purpose: Static Utility function to get the Suit number of a card id.
Parameters:
*           a_id, an int passed by value. It holds the id that the card represents
*
Return Value: The suit number of the id.
Assistance Received: none
********************************************************************* */
int Card::Get_Suit_From_Id(int a_id) {
	return a_id / 12;
}

/* *********************************************************************
Function Name: Get_Weight_From_Id
Purpose: Static Utility function to get the face weight from the card id.
Parameters:
*           a_id, an int passed by value. It holds the id that the card represents
*
Return Value: The weight value of the face of the id.
Assistance Received: none
********************************************************************* */
int Card::Get_Weight_From_Id(int a_id) {
	return m_face_weights[Get_Face_From_Id(a_id)];
}

/* *********************************************************************
Function Name: Get_Card_Id_From_String
Purpose: Static Utility function to get the appropriate card id from a card string.
Parameters:
*           a_card_str, an int passed by const reference. It holds the card string value. E.g. "XH"
*
Return Value: The appropriate card id.
Assistance Received: none
********************************************************************* */
int Card::Get_Card_Id_From_String(const std::string &a_card_str) {
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

/* *********************************************************************
Function Name: Get_Card_Id
Purpose: Getter for the card id of a card.
Parameters:
*           None
Return Value: The card id of this card.
Assistance Received: none
********************************************************************* */
int Card::Get_Card_Id() const {
	return m_id;
}
