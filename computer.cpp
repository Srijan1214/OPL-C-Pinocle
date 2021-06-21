
/*
 ************************************************************
 * Name: Srijan Prasad Joshi
 * Project: Pinochle C++
 * Class: OPL Fall 20
 * Date: 09/30/2020
 ************************************************************
*/

// Created by Srijan Joshi on 09/10/19.

#include "computer.h"

/* *********************************************************************
Function Name: Get_Card_To_Play
Purpose: To get the card that the computer wants to play
Parameters:
            a_lead_card_played, the card that the lead player played.
Return Value: The Card* for the card that the lead player wants to play.
Local Variables:
            temp[], an integer array used to sort the grades
Algorithm:
            1) Get best card from Find_IndexMeldPair_Of_Card_To_Throw if the computer is lead. Hence, find 
			the best meld possible and play the card that results in that best meld.
            2) If computer is lead, then play the biggest card.
Assistance Received: none
********************************************************************* */
Card* Computer::Get_Card_To_Play(Card* a_lead_card_played) {
	std::cout << "Computer turn." << std::endl;
	int index;
	if (a_lead_card_played == NULL) {
		// computer is lead player
		std::pair<int,int> recommended_card_with_best_meld = Find_IndexMeldPair_Of_Card_To_Throw();
		int & best_card_index = recommended_card_with_best_meld.first;

		if(best_card_index != -1) {
			// meld is possible if best card is thrown
			index = best_card_index;
			int id = m_hand_card_pile[index]->Get_Card_Id();
			std::cout << "Computer played " << "\"" << Card::Get_String_From_Id(id) << "\"" << " because it was the card resulting in the highest possible meld." << std::endl;
		} else {
			// no meld is possible from any card thrown
			// throw the greatest card to maximize winning chances
			index = Find_Index_Of_Greatest_Card();
			int id = m_hand_card_pile[index]->Get_Card_Id();
			std::cout << "Computer played " << "\"" << Card::Get_String_From_Id(id) << "\"" << ", which is the greatest card. However, it has no melds available." << std::endl;
		}
	} else {
		// computer is chase player
		index = Find_Index_of_Smallest_Card_Greater_Than_Card(a_lead_card_played);
		if(index == -1) {
			index = Find_Index_Of_Smallest_Card();
			int id = m_hand_card_pile[index]->Get_Card_Id();
			std::cout << "Computer played " << "\"" << Card::Get_String_From_Id(id) << "\"" << ", which is the smallest card, because it had no possibility of winning." << std::endl;
		} else {
			int id = m_hand_card_pile[index]->Get_Card_Id();
			std::cout << "Computer played " << "\"" << Card::Get_String_From_Id(id) << "\"" << " because it was the smallest greater card than the lead player's card." << std::endl;
		}
	}
	std::cout << std::endl;
	Card* card_ptr = m_hand_card_pile[index];
	Remove_Card_From_Pile(index);
	return card_ptr;
}

/* *********************************************************************
Function Name: Get_Meld_To_Play
Purpose: To Get the meld to play.
Parameters:
            None
Return Value: Meld number played.
Algorithm:
            1) Play the best meld possible in the hand.
Assistance Received: none
********************************************************************* */
int Computer::Get_Meld_To_Play() {
	std::pair<std::vector<int>,int> recommended_card_with_best_meld = Get_Indexes_And_Meld_Number12_Best_Meld();

	int recommended_meld_number_12 = recommended_card_with_best_meld.second;
	int recommended_meld_number_9 = TO9(recommended_card_with_best_meld.second);

	if(recommended_meld_number_9 != -1) {
		std::cout << "The computer presented";
		for(int i = 0; i < recommended_card_with_best_meld.first.size(); i++) {
			if(i != 0) {
				if(i == recommended_card_with_best_meld.first.size()-1) {
					std::cout << " and";
				}else {
					std::cout << ",";
				}
			}
			std::cout << " " << Card::Get_String_From_Id(recommended_card_with_best_meld.first[i]);
		}
		std::cout << " as a \"" << m_meld_names[recommended_meld_number_9] << "\" meld to earn "<< m_meld_scores[recommended_meld_number_9] <<" points." << std::endl;

		// Create the Index vector for meld cards.
		std::vector<int> meld_card_indexes;
		for(int& card_id: recommended_card_with_best_meld.first) {
			int card_index = Search_Card_In_Pile(card_id);
			if(card_index < m_hand_card_pile.size()) {
				meld_card_indexes.push_back(card_index);
			}
		}

		// meld is valid so update the hand to meld involvement
		for(int i = 0; i < meld_card_indexes.size(); i++) {
			int& index = meld_card_indexes[i];
			int number_of_similar_melds = m_current_meld_cards[recommended_meld_number_12].size();
			m_hand_meld_involvement_list[index].push_back({recommended_meld_number_12, number_of_similar_melds, i});
		}
		m_current_meld_cards[recommended_meld_number_12].push_back(meld_card_indexes);

		//Create cards vector
		std::vector<Card*> meld_cards;
		for(int& index: meld_card_indexes) {
			meld_cards.push_back(m_hand_card_pile[index]);
		}
		// Update that the current meld has been played the history.
		Update_Meld_History(meld_cards, recommended_meld_number_9);

	} else {
		std::cout << "Computer found no possible melds. So, it did not play any." << std::endl;
	}
	return recommended_meld_number_9;
}
