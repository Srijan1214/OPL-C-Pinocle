#include "human.h"

Human::Human() {}

std::vector<Card*> Human::Ask_For_Meld_Cards() {
	int id = 0;
	return {m_hand_card_pile[id]};
}

const char Human::Get_Heads_Or_Tails() const { return 'H'; }

void Human::Print_Hand_and_Meld_With_Id() {
	int total_cards = m_hand_card_pile.size() + m_meld_card_pile.size();

	std::cout << "TRUMP:   " << Card::Get_String_From_Id(m_trump_card_id) << std::endl;

	std::cout << "INDEX:   ";
	for (int i = 0; i < total_cards; i++) {
		std::cout << i;
		if (i == m_hand_card_pile.size() - 1) {
			std::cout << "              ";
		}
		if (i < 10) {
			std::cout << "   ";
		} else {
			std::cout << "  ";
		}
	}
	std::cout << std::endl;
	std::cout << "CARD:    ";
	for (Card* card_ptr : m_hand_card_pile) {
		std::cout << card_ptr->Get_Card_String_Value() << "  ";
	}
	std::cout << "  Meld Cards: ";

	for (Card* card_ptr : m_meld_card_pile) {
		std::cout << card_ptr->Get_Card_String_Value() << "  ";
	}

	std::cout << std::endl;
	std::cout << std::endl;
}

Card* Human::Get_Card_To_Play(Card* a_lead_card_played) {
	int index = -1;
	int total_cards = m_hand_card_pile.size() + m_meld_card_pile.size();

	do {
		Print_Hand_and_Meld_With_Id();
		std::pair<int,int> recommended_card_with_best_meld = Find_IndexMeldPair_Of_Card_To_Throw();
		int & best_card_index = recommended_card_with_best_meld.first;
		int & best_meld_index = recommended_card_with_best_meld.second;
		if(best_card_index != -1) {
			std::cout << "Recommended Card Index: " << best_card_index << std::endl;
			std::cout << "Best Meld Availabe: " << m_meld_names[recommended_card_with_best_meld.second] << std::endl;
		} else {
			std::cout << "No Meld will be available after this. Play highest card." << std::endl;
		}
		std::cout << "Enter index of Cards From Above: ";
		index = Get_Integer_Input_From_User();
	} while (!(index >= 0 && index <= total_cards) || std::cin.fail());
	std::cout << std::endl;

	Card* ret_card;

	if(index < m_hand_card_pile.size()){
		ret_card = m_hand_card_pile[index];
		m_hand_card_pile.erase(m_hand_card_pile.begin() + index);
	} else {
		index-= m_hand_card_pile.size();
		ret_card = m_meld_card_pile[index];
		m_meld_card_pile.erase(m_meld_card_pile.begin() + index);
	}
	return ret_card;
}

std::vector<int> Human::Parse_Indexes_Vector_From_String(std::string a_user_input) {
	std::vector<int> ret_vec;

	std::stringstream s(a_user_input);

	std::string word;
	std::unordered_set<int> u_set;
	while (std::getline(s, word, ' ')) {
		int insert_val = -1;
		try{
			insert_val = std::stoi(word);
		} catch(std::invalid_argument) {

		}
		if (insert_val >= 0 &&
			insert_val < (m_hand_card_pile.size() + m_meld_card_pile.size())){
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
	int meld_number = -1;

	std::vector<int> user_input_indexes;
	std::vector<int> hand_pile_user_selection_ids;
	std::vector<int> meld_pile_user_selection_ids;
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
		std::pair<std::vector<int>,int> recommended_meld_with_ids = Get_Best_Meld_Cards();

		int recommended_meld_number = recommended_meld_with_ids.second;
		if(recommended_meld_number>=0 && recommended_meld_number < 9){
			std::cout << "I recommend you present";
			for(int i = 0; i < recommended_meld_with_ids.first.size(); i++) {
				if(i != 0) {
					if(i == recommended_meld_with_ids.first.size()-1) {
						std::cout << " and";
					}else {
						std::cout << ",";
					}
				}
				std::cout << " " << Card::Get_String_From_Id(recommended_meld_with_ids.first[i]);
			}
			std::cout << " as a \"" << m_meld_names[recommended_meld_number] << "\" meld to earn "<< m_meld_scores[recommended_meld_number] <<" points." << std::endl;
		} else {
			std::cout << "No Meld is possible here." << std::endl;
		}
		std::string user_meld_input;
		std::cout << "Enter the sequences of indexes separated by a space (e.g 1 2 3): ";
		std::cin.clear();
		std::getline(std::cin, user_meld_input);
		// std::cout << std::endl;
		user_input_indexes = Parse_Indexes_Vector_From_String(user_meld_input);
		// sorting necessary as user could have mixed meld deck and hand deck.
		std::sort(user_input_indexes.begin(),user_input_indexes.end()); 


		for(int& ele: user_input_indexes) {
			if (ele < m_hand_card_pile.size()) {
				hand_pile_user_selection_ids.push_back(ele);
			}else {
				meld_pile_user_selection_ids.push_back(ele - m_hand_card_pile.size());
			}
		}

		std::vector<Card*> meld_cards;

		for(int& ele: hand_pile_user_selection_ids) {
			meld_cards.push_back(m_hand_card_pile[ele]);
		}

		for(int& ele: meld_pile_user_selection_ids) {
			meld_cards.push_back(m_meld_card_pile[ele]);
		}

		meld_number = Get_Meld_Type_From_Cards(meld_cards);
		if (meld_number >= 0 && meld_number < 9 && Is_Meld_Valid(meld_cards)) {
			Update_Meld_History(meld_cards,meld_number);

			// sorting is necessary for deletion
			std::sort(hand_pile_user_selection_ids.begin(),hand_pile_user_selection_ids.end()); 
			for(int i = hand_pile_user_selection_ids.size() - 1; i >=0; i--) {
				m_hand_card_pile[hand_pile_user_selection_ids[i]] = m_hand_card_pile.back();
				m_hand_card_pile.pop_back();
			}

			// sorting is necessary for deletion
			std::sort(meld_pile_user_selection_ids.begin(),meld_pile_user_selection_ids.end()); 
			for(int i = meld_pile_user_selection_ids.size() - 1; i >=0; i--) {
				m_meld_card_pile[meld_pile_user_selection_ids[i]] = m_meld_card_pile.back();
				m_meld_card_pile.pop_back();
			}

			// add cards to the meld pile after removing the them from hand and meld pile.
			// this ensures protection from duplicate adding to the meld pile.
			for(Card* card_ptr: meld_cards) {
				m_meld_card_pile.push_back(card_ptr);
			}
		} else {
			meld_number = 9;
			do{
				std::cout << "The Meld is not valid. Do you still want to meld (y/n)? ";
				meld_yes_no = Get_Char_Input_From_User();
			}while(!(tolower(meld_yes_no) == 'y'|| tolower(meld_yes_no) == 'n'));

			if (meld_yes_no == 'n') {
				return 9;
			}
		}

		user_input_indexes.clear();
		hand_pile_user_selection_ids.clear();
		meld_pile_user_selection_ids.clear();
		std::cout << std::endl;


	} while (!(meld_number >= 0 && meld_number < 9));

	return meld_number;
}

int Human::Get_Integer_Input_From_User() {
	int ret_val = -1;
	std::string user_input;

	std::cin.clear();
	std::getline(std::cin, user_input);

	try{
		ret_val = std::stoi(user_input);
	}catch (std::invalid_argument) {

	} catch (std:: out_of_range) {

	}

	return ret_val;
}


char Human::Get_Char_Input_From_User() {
	char ret_val;
	std::string user_input;

	std::cin.clear();
	std::getline(std::cin, user_input);

	ret_val = user_input[0];

	return ret_val;
}