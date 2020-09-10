#include "human.h"

Human::Human() { std::cout << "Human Constructor" << std::endl; }

std::vector<Card*> Human::Ask_For_Meld_Cards() {
	int id;
	return {m_hand_card_pile[id]};
}

const char Human::Get_Heads_Or_Tails() const { return 'H'; }

void Human::Print_Hand_and_Meld_With_Id() {
		std::cout << "INDEX:   ";
		for(int i = 0; i < total_cards; i++) {
			std::cout << i;
			if(i==m_hand_card_pile.size()) {
				std::cout << "              ";
			}
			if(i < 10) {
				std::cout << "   ";
			}else {
				std::cout << "  ";
			}
		}
		std::cout << std::endl;
		std::cout << "CARD:    ";
		for(Card* card_ptr: m_hand_card_pile) {
			std::cout << card_ptr->Get_Card_String_Value() << "  ";
		}
		std::cout << "  Meld Cards: ";

		for(Card* card_ptr: m_meld_card_pile) {
			std::cout << card_ptr->Get_Card_String_Value() << "  ";
		}

		std::cout << std::endl;
}

Card* Human::Get_Card_To_Play(Card* a_lead_card_played) {
	int index = -1;
	int total_cards = m_hand_card_pile.size() + m_meld_card_pile.size();

	do {
		Print_Hand_and_Meld_With_Id();
		std::cout << "Enter index of Cards From Above" << std::endl;
		std::cin >> index;
	} while (!(index >= 0 && index <= total_cards));

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

int Human::Get_Meld_To_Play() {
	char yes_no;
	char meld_yes_no;
	int index;
	int meld_number = -1;
	std::unordered_set<int> u_set;

	do {
		do{
			std::cout << " Do you want perform to meld" << std::endl;
			std::cin >> meld_yes_no;
		}while(!(tolower(meld_yes_no) == 'y'|| tolower(meld_yes_no) == 'n'));

		if(meld_yes_no == 'n') {
			break;
		}

		do {
			do {
				std::cout << "Enter the Card index you want to Play."
						  << std::endl;
				std::cin >> index;
			} while (!(index < m_hand_card_pile.size() && index >= 0));
			u_set.insert(index);
			std::cout << "Do you want to continue(y/n)? " << std::endl;
			std::cin >> yes_no;
		} while (!(tolower(yes_no) == 'n'));

		std::vector<Card*> meld_cards;
		std::vector<int> ids;

		for(const int ele: u_set ){
			meld_cards.push_back(m_hand_card_pile[ele]);
			ids.push_back(ele);
		}

		meld_number = Get_Meld_Type_From_Cards(meld_cards);
		if (meld_number >=0 && meld_number<9) {
			Update_Meld_History(meld_cards,meld_number);
			std::sort(ids.begin(),ids.end());

			for(int i = ids.size() - 1; i >=0; i--) {
				m_hand_card_pile[ids[i]] = m_hand_card_pile.back();
				m_hand_card_pile.pop_back();
			}

		}

		u_set.clear();
	} while (!(meld_number >= 0 && meld_number < 9));

	return meld_number;
}