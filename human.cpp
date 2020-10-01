#include "human.h"

Human::Human() {}

void Human::Print_Hand_and_Meld_With_Id() const{
	std::string index_string = "";
	std::string cards_string = "";

	index_string += "Index:       ";
	cards_string += "Hand-Pile:   ";

	for(int i = 0; i < m_hand_card_pile.size(); i++) {
		if(m_hand_meld_involvement_list[i].empty()) {
			// std::cout << m_hand_card_pile[i]->Get_Card_String_Value() << " ";
			index_string += (std::to_string(i) + "  "); 
			cards_string += m_hand_card_pile[i]->Get_Card_String_Value() + " ";
			if(i >= 10) {
				cards_string += " ";
			}
		}
	}

	// std::cout << "Melds: " << std::endl;
	index_string += "        ";
	cards_string += "Melds:  ";

	for(int i = 0; i < m_current_meld_cards.size(); i++) {
		for(auto& current_melds: m_current_meld_cards[i]) {
			for(const int& hand_index: current_melds) {
				// std::cout << m_hand_card_pile[hand_index]->Get_Card_String_Value();
				index_string += (std::to_string(hand_index) + " "); 
				cards_string += m_hand_card_pile[hand_index]->Get_Card_String_Value();
				if(m_hand_meld_involvement_list[hand_index].size() > 1) {
					// std::cout << "*";
					index_string += " ";
					cards_string += "*";
				}
				if(hand_index >= 10) {
					cards_string+= " ";
				}
				// std::cout << " ";
				index_string += " "; 
				cards_string += " ";
			}
		}
	}
	std::cout << index_string << std::endl;
	std::cout << cards_string << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

Card* Human::Get_Card_To_Play(Card* a_lead_card_played) {
	int index = -1;
	int total_cards = m_hand_card_pile.size();

	do {
		if(m_help_mode) {
			Print_Computer_Card_Recomendation(a_lead_card_played);
		}
		Print_Hand_and_Meld_With_Id();
		std::cout << "Enter index of Cards From Above: ";
		index = Get_Integer_Input_From_User();
	} while (!(index >= 0 && index < total_cards) || std::cin.fail());
	std::cout << std::endl;

	Card* ret_card = m_hand_card_pile[index];
	Remove_Card_From_Pile(index);
	return ret_card;
}

std::vector<int> Human::Parse_Indexes_Vector_From_String(std::string a_user_input) const {
	std::vector<int> ret_vec;

	std::stringstream s(a_user_input);

	std::string word;
	std::unordered_set<int> u_set;
	while (std::getline(s, word, ' ')) {
		int insert_val = -1;
		try{
			insert_val = std::stoi(word);
		} catch(const std::invalid_argument&) {

		} catch(const std::out_of_range&) {

		}
		if (insert_val >= 0 &&
			insert_val < (m_hand_card_pile.size())){
			u_set.insert(insert_val);
		}
	}
	for(int ele: u_set) {
		ret_vec.push_back(ele);
	}
	return ret_vec;

}

int Human::Get_Meld_To_Play() {
	std::string user_line_input;
	char meld_yes_no;
	int meld_number_9 = -1;

	if(m_help_mode) {
		Print_Computer_Meld_Recommendation();
	}

	std::vector<int> user_input_indexes;
	std::vector<int> hand_pile_user_selection_indexes;
	do{
		std::cout << "Do you want to meld this round (y/n)? ";
		meld_yes_no = Get_Char_Input_From_User();
	}while(!(tolower(meld_yes_no) == 'y'|| tolower(meld_yes_no) == 'n'));

	std::cout << std::endl;

	if(meld_yes_no == 'n') {
		return 9;
	}
	do {
		Print_Hand_and_Meld_With_Id();

		std::string user_meld_input;
		std::cout << "Enter the sequences of indexes separated by a space (e.g 1 2 3): ";
		std::cin.clear();
		std::getline(std::cin, user_meld_input);
		// std::cout << std::endl;
		user_input_indexes = Parse_Indexes_Vector_From_String(user_meld_input);
		// sorting necessary as user could have mixed meld deck and hand deck.
		std::sort(user_input_indexes.begin(),user_input_indexes.end()); 


		for(int& ele: user_input_indexes) {
			hand_pile_user_selection_indexes.push_back(ele);
		}

		std::vector<Card*> meld_cards;

		for(int& ele: hand_pile_user_selection_indexes) {
			meld_cards.push_back(m_hand_card_pile[ele]);
		}

		int meld_number_12 = Get_Meld_Type_12_From_Cards(meld_cards); 
		meld_number_9 = TO9(meld_number_12);
		if (meld_number_9 >= 0 && meld_number_9 < 9 && Is_Meld_Allowed_By_History(meld_cards, meld_number_9)) {
			Update_Meld_History(meld_cards,meld_number_9);

			// meld is valid so update the hand to meld involvement
			for(int i = 0; i < hand_pile_user_selection_indexes.size(); i++) {
				int& index = hand_pile_user_selection_indexes[i];
				int number_of_similar_melds = m_current_meld_cards[meld_number_12].size();
				m_hand_meld_involvement_list[index].push_back({meld_number_12, number_of_similar_melds, i});
			}

			m_current_meld_cards[meld_number_12].push_back(hand_pile_user_selection_indexes);

		} else {
			meld_number_9 = 9;
			do{
				std::cout << "The Meld is not valid. Do you still want to meld (y/n)? ";
				meld_yes_no = Get_Char_Input_From_User();
			}while(!(tolower(meld_yes_no) == 'y'|| tolower(meld_yes_no) == 'n'));

			if (meld_yes_no == 'n') {
				return 9;
			}
		}

		user_input_indexes.clear();
		hand_pile_user_selection_indexes.clear();
		std::cout << std::endl;


	} while (!(meld_number_9 >= 0 && meld_number_9 < 9));

	return meld_number_9;
}

int Human::Get_Integer_Input_From_User() const{
	int ret_val = -1;
	std::string user_input;

	std::cin.clear();
	std::getline(std::cin, user_input);

	try{
		ret_val = std::stoi(user_input);
	}catch (const std::invalid_argument&) {

	} catch (const std:: out_of_range&) {

	}

	return ret_val;
}

char Human::Get_Char_Input_From_User() const {
	char ret_val;
	std::string user_input;

	std::cin.clear();
	std::getline(std::cin, user_input);

	ret_val = user_input[0];

	return ret_val;
}

void Human::Print_Computer_Card_Recomendation(Card* a_lead_card_played) const {
	std::cout << "Help Mode Recomendation." << std::endl;
	int index;
	if (a_lead_card_played == NULL) {
		// computer is lead player
		std::pair<int,int> recommended_card_with_best_meld = Find_IndexMeldPair_Of_Card_To_Throw();
		int & best_card_index = recommended_card_with_best_meld.first;

		if(best_card_index != -1) {
			// meld is possible if best card is thrown
			index = best_card_index;
			int id = m_hand_card_pile[index]->Get_Card_Id();
			std::cout << "I recommend you play " << "\"" << Card::Get_String_From_Id(id) << "\"" << " because it is the card resulting in the highest possible meld." << std::endl;
		} else {
			// no meld is possible from any card thrown
			// throw the greatest card to maximize winning chances
			index = Find_Index_Of_Greatest_Card();
			int id = m_hand_card_pile[index]->Get_Card_Id();
			std::cout << "I recommend you play " << "\"" << Card::Get_String_From_Id(id) << "\"" << ", which is the greatest card, because you have no melds possible available." << std::endl;
		}
	} else {
		// computer is chase player
		index = Find_Index_of_Smallest_Card_Greater_Than_Card(a_lead_card_played);
		if(index == -1) {
			index = Find_Index_Of_Smallest_Card();
			int id = m_hand_card_pile[index]->Get_Card_Id();
			std::cout << "I recommend you play " << "\"" << Card::Get_String_From_Id(id) << "\"" << ", which is the smallest card, because you have no possibility of winning." << std::endl;
		} else {
			int id = m_hand_card_pile[index]->Get_Card_Id();
			std::cout << "I recommend you play " << "\"" << Card::Get_String_From_Id(id) << "\"" << " because it is the smallest card greater than the lead player's card." << std::endl;
		}
	}
	std::cout << std::endl;
}

void Human::Print_Computer_Meld_Recommendation() const {
	std::pair<std::vector<int>,int> recommended_card_with_best_meld = Get_Indexes_And_Meld_Number12_Best_Meld();

	int recommended_meld_number = TO9(recommended_card_with_best_meld.second);

	if(recommended_meld_number != -1) {
		std::cout << "I recommend you present";
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
	} else {
		std::cout << "You have no melds available." << std::endl;
	}
}
