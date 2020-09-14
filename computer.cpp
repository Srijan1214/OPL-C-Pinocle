#include "computer.h"

Card* Computer::Get_Card_To_Play(Card* a_lead_card_played) {
	std::cout << "Computer turn." << std::endl;
	int index;
	if (a_lead_card_played == NULL) {
		// computer is lead player
		std::pair<int,int> recommended_card_with_best_meld = Find_IndexMeldPair_Of_Card_To_Throw();
		int & best_card_index = recommended_card_with_best_meld.first;
		int & best_meld_index = recommended_card_with_best_meld.second;

		if(best_card_index != -1) {
			// meld is possible if best card is thrown
			index = best_card_index;
			int id;
			if(index < m_hand_card_pile.size()) id = m_hand_card_pile[index]->Get_Card_Id();
			else id = m_meld_card_pile[index - m_hand_card_pile.size()]->Get_Card_Id();
			std::cout << "Computer played " << "\"" << Card::Get_String_From_Id(id) << "\"" << " because it was the card resulting in the highest possible meld." << std::endl;
		} else {
			// no meld is possible from any card thrown
			// throw the greatest card to maximize winning chances
			index = Find_Index_Of_Greatest_Card();
			int id;
			if(index < m_hand_card_pile.size()) id = m_hand_card_pile[index]->Get_Card_Id();
			else id = m_meld_card_pile[index - m_hand_card_pile.size()]->Get_Card_Id();
			std::cout << "Computer played " << "\"" << Card::Get_String_From_Id(id) << "\"" << ", which is the greatest card, because it had no melds possible available." << std::endl;
		}
	} else {
		// computer is chase player
		index = Find_Index_of_Smallest_Card_Greater_Than_Card(a_lead_card_played);
		if(index == -1) {
			index = Find_Index_Of_Smallest_Card();
			int id;
			if(index < m_hand_card_pile.size()) id = m_hand_card_pile[index]->Get_Card_Id();
			else id = m_meld_card_pile[index - m_hand_card_pile.size()]->Get_Card_Id();
			std::cout << "Computer played " << "\"" << Card::Get_String_From_Id(id) << "\"" << ", which is the smallest card, because it had no possibility of winning." << std::endl;
		} else {
			int id;
			if(index < m_hand_card_pile.size()) id = m_hand_card_pile[index]->Get_Card_Id();
			else id = m_meld_card_pile[index - m_hand_card_pile.size()]->Get_Card_Id();
			std::cout << "Computer played " << "\"" << Card::Get_String_From_Id(id) << "\"" << " because it was the smallest greater than the lead player's card." << std::endl;
		}
	}
	std::cout << std::endl;
	if(index > m_hand_card_pile.size()) {
		index = index - m_hand_card_pile.size();
		Card* card_ptr = m_meld_card_pile[index];
		m_meld_card_pile[index] = m_meld_card_pile.back();
		m_meld_card_pile.pop_back();
		return card_ptr;
	}
	Card* card_ptr = m_hand_card_pile[index];
	m_hand_card_pile[index] = m_hand_card_pile.back();
	m_hand_card_pile.pop_back();
	return card_ptr;
}

int Computer::Get_Meld_To_Play() {
	std::pair<std::vector<int>,int> recommended_card_with_best_meld = Get_Best_Meld_Cards();

	int& recommended_meld_number = recommended_card_with_best_meld.second;

	if(recommended_meld_number != -1) {
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
		std::cout << " as a \"" << m_meld_names[recommended_meld_number] << "\" meld to earn "<< m_meld_scores[recommended_meld_number] <<" points." << std::endl;

		// if can play the move, then move all the cards from meld pile to hand pile.
		std::vector<int> hand_pile_indexes;
		for(int& card_id: recommended_card_with_best_meld.first) {
			int card_index = Find_Index_In_Pile_From_Card_Id(card_id);
			if(card_index < m_hand_card_pile.size()) {
				hand_pile_indexes.push_back(card_index);
			}
		}
		std::sort(hand_pile_indexes.begin(),hand_pile_indexes.end());

		// Move the cards that are played for meld from the hand pile to the meld pile.
		for (int i = hand_pile_indexes.size() - 1; i >= 0; i--) {
			int& index = hand_pile_indexes[i];
			m_meld_card_pile.push_back(m_hand_card_pile[index]);
			m_hand_card_pile[index] = m_hand_card_pile.back();
			m_hand_card_pile.pop_back();
		}
	} else {
		std::cout << "Computer found no possible melds. So, it did not play any." << std::endl;
	}
	return recommended_meld_number;
}

int Computer::Find_Index_of_Smallest_Card_Greater_Than_Card(Card* a_card_ptr) {
	const int& hand_pile_size = m_hand_card_pile.size();
	const int& meld_pile_size = m_meld_card_pile.size();

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
	
	for (int i = 0; i < meld_pile_size; i++) {
		int cur_card_weight = Get_Card_Weight(m_meld_card_pile[i]);
		if(cur_card_weight > argument_card_weight && cur_card_weight < min_greatest_card_weight) {
			index = i + hand_pile_size;
			min_greatest_card_weight = cur_card_weight;
		}
	}

	return index;
}

int Computer::Find_Index_Of_Smallest_Card() {
	const int& hand_pile_size = m_hand_card_pile.size();
	const int& meld_pile_size = m_meld_card_pile.size();

	int min_card_weight = INT_MAX;
	int index =-1;
	for(int i = 0; i < hand_pile_size; i++) {
		int cur_card_weight = Get_Card_Weight(m_hand_card_pile[i]);
		if(cur_card_weight < min_card_weight) {
			index = i;
			min_card_weight = cur_card_weight;
		}
	}
	for(int i = 0; i < meld_pile_size; i++) {
		int cur_card_weight = Get_Card_Weight(m_meld_card_pile[i]);
		if(cur_card_weight < min_card_weight) {
			index = i;
			min_card_weight = cur_card_weight;
		}
	}

	return index;
}

int Computer::Find_Index_Of_Greatest_Card() {
	const int& hand_pile_size = m_hand_card_pile.size();
	const int& meld_pile_size = m_meld_card_pile.size();

	int max_card_weight = INT_MIN;
	int index =-1;
	for(int i = 0; i < hand_pile_size; i++) {
		int cur_card_weight = Get_Card_Weight(m_hand_card_pile[i]);
		if(cur_card_weight > max_card_weight) {
			index = i;
			max_card_weight = cur_card_weight;
		}
	}
	for(int i = 0; i < meld_pile_size; i++) {
		int cur_card_weight = Get_Card_Weight(m_meld_card_pile[i]);
		if(cur_card_weight > max_card_weight) {
			index = i;
			max_card_weight = cur_card_weight;
		}
	}

	return index;
}








