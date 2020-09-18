#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <array>

#include "card.h"

class Player {
   public:
	Player();
	void Give_Card_To_Player(Card* a_card_ptr);
	std::vector<Card*> Ask_Which_Cards_To_Play_For_Meld();
	bool Validate_If_Meld_Can_Be_Played(std::vector<Card*> & a_meld_card_list, int meld_number);
	void Update_Meld_History(std::vector<Card*> &, int);
	void Add_Cards_To_Player_Capture_Pile (Card* a_card_ptr1, Card* a_card_ptr2);
	void Set_Trump_Suit(int);
	void Set_Round_Score(int a_score);
	void Set_Trump_card(int);
	std::string Get_Console_Message();

	static const int m_meld_scores[9];
	static const std::string m_meld_names[9];

	virtual Card* Get_Card_To_Play(Card* a_lead_card_played) = 0; // Argument is NULL if the current player is lead player.
	virtual int Get_Meld_To_Play() = 0;

   protected:
	std::vector<Card*> m_hand_card_pile;
	std::vector<std::vector<std::array<int,3>>> m_hand_meld_involvement_list;
	std::vector<std::vector<std::vector<int>>> m_current_meld_cards;
	void Remove_Card_From_Pile(int index);

	std::vector<Card*> m_capture_card_pile;
	std::vector<int> m_no_of_times_meld_has_been_played;
	std::vector<std::vector<bool>> m_which_card_used_for_meld;
	int Get_Meld_Type_12_From_Cards(std::vector<Card*> &);
	bool Is_Meld_Valid(std::vector<Card*> &);
	int m_trump_suit_of_current_game;
	int m_trump_card_id;
	int m_round_score;

	std::pair<int,int> Find_IndexMeldPair_Of_Card_To_Throw();

	std::vector<std::vector<int>> Get_Meld_Logic_Vector();
	void add_to_meld_logic_vector(std::vector<std::vector<int>>& a_meld_logic_vector, std::vector<Card*>& a_card_pile);
	int TO9(int a_12_based_index);
	int Get_Best_Meld_If_Card_Thrown(std::vector<std::vector<int>>& a_meld_logic_vector, Card* a_card_ptr);
	std::pair<int,int> Get_Best_MeldCardIndexPair_From_Pile(std::vector<std::vector<int>>& a_meld_logic_vector, std::vector<Card*> a_card_pile);

	std::pair<std::vector<int>,int> Get_Best_Meld_Cards();
	int Find_Index_In_Pile_From_Card_Id(int a_id);

	int Get_Card_Weight(Card* card_ptr);

	// Utilities
	int Find_Index_of_Smallest_Card_Greater_Than_Card(Card* card_ptr);
	int Find_Index_Of_Smallest_Card();
	int Find_Index_Of_Greatest_Card();
};
