#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

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
	virtual std::vector<Card*> Ask_For_Meld_Cards() = 0;
	virtual Card* Get_Card_To_Play(Card* a_lead_card_played) = 0; // Argument is NULL if the current player is lead player.
	virtual int Get_Meld_To_Play() = 0;

   protected:
	std::vector<Card*> m_hand_card_pile;
	std::vector<Card*> m_meld_card_pile;
	std::vector<Card*> m_capture_card_pile;
	std::vector<int> m_no_of_times_meld_has_been_played;
	std::vector<std::vector<bool>> m_which_card_used_for_meld;
	int Get_Meld_Type_From_Cards(std::vector<Card*> &);
	bool Is_Meld_Valid(std::vector<Card*> &);
	int m_trump_suit_of_current_game;
	int m_trump_card_id;
	int m_round_score;
	int Find_Index_Of_Card_To_Throw();

	std::vector<std::vector<int>> Get_Cards_Needed_For_Meld_Vector();
};
