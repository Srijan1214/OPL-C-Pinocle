
/*
 ************************************************************
 * Name: Srijan Prasad Joshi
 * Project: Pinochle C++
 * Class: OPL Fall 20
 * Date: 09/30/2020
 ************************************************************
*/

// Created by Srijan Joshi on 09/10/19.

#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <array>

#include "card.h"

class Player {
   public:
	Player();
	void Give_Card_To_Player(Card* a_card_ptr);
	void Add_Cards_To_Player_Capture_Pile (Card* a_card_ptr1, Card* a_card_ptr2);

	void Set_Trump_Suit(int);
	void Set_Trump_card(int);
	
	std::string Get_Console_Message() const;
	std::string Get_Hand_Pile_String() const;
	std::string Get_Capture_Pile_String() const;
	std::string Get_Melds_String() const;

	static const int m_meld_scores[9];
	static const std::string m_meld_names[9];

	virtual Card* Get_Card_To_Play(Card* a_lead_card_played) = 0; // Argument is NULL if the current player is lead player.
	virtual int Get_Meld_To_Play() = 0;

	void Load_Members_From_Serialization_String(std::string a_serialization_string, std::vector<bool> &a_cards_that_have_been_used);
	void Load_Capture_Cards_From_String(std::string &a_meld_string, std::vector<bool> &a_cards_that_have_been_used);
	void Load_Hand_Cards_From_String(std::string &a_meld_string, std::vector<bool> &a_cards_that_have_been_used);
	void Load_Meld_Cards_From_String(std::string &a_meld_string, std::vector<bool> &a_cards_that_have_been_used);

	int Get_No_Of_Remaining_Cards() const;
	std::vector<Card*> Pop_All_Cards_From_Capture_Pile();
	void Reset_Meld_History();

	void Turn_On_Help_Mode();
	void Turn_Off_Help_Mode();

   protected:
	std::vector<Card*> m_hand_card_pile;
	std::vector<std::vector<std::array<int,3>>> m_hand_meld_involvement_list;
	std::vector<std::vector<std::vector<int>>> m_current_meld_cards;
	void Remove_Card_From_Pile(int index);

	bool m_help_mode;

	// Pile that stores the capture cards
	std::vector<Card*> m_capture_card_pile;

	// Facilitates Meld History of Round
	std::vector<int> m_no_of_times_meld_has_been_played;
	std::vector<std::vector<bool>> m_which_card_used_for_meld;
	
	int Get_Meld_Type_12_From_Cards(const std::vector<Card*> &) const;
	int Get_Meld_Type_12_From_Cards(std::vector<int> &) const;
	int m_trump_suit_of_current_game;
	int m_trump_card_id;


	// AI logic.
	// Best Card and the resulting meld if the card thrown.
	// Used when player is the lead player.
	std::pair<int,int> Find_IndexMeldPair_Of_Card_To_Throw() const;

	// AI logic for melds.
	// Gets the indexes of the cards resulting in the best meld
	// and the meld number_9
	std::pair<std::vector<int>,int> Get_Indexes_And_Meld_Number12_Best_Meld() const;

	// Meld History Functions
	bool Is_Meld_Allowed_By_History(std::vector<Card*> & a_meld_card_list, int meld_number) const;
	void Update_Meld_History(const std::vector<Card*> &, int);

	// Utilities
	int Find_Index_of_Smallest_Card_Greater_Than_Card(Card* card_ptr) const;
	int Find_Index_Of_Smallest_Card() const;
	int Find_Index_Of_Greatest_Card() const;
	int TO9(int a_12_based_index) const;
	int Search_Card_In_Pile(int a_id) const;
	bool Is_First_Card_Greater_Than_Lead(int a_card_1, int a_card_2) const;
	int Get_Card_Weight(Card* card_ptr) const;

   private:
	// The rest are function only used by the Find_IndexMeldPair_Of_Card_To_Throw function.
	std::vector<std::vector<int>> Get_Meld_Logic_Vector() const;
	void add_to_meld_logic_vector(std::vector<std::vector<int>>& a_meld_logic_vector, const std::vector<Card*>& a_card_pile) const;
	void Update_Logic_Vector_With_History(std::vector<std::vector<int>>& a_meld_logic_vector) const;
	int Get_Best_Meld_If_Card_Thrown(std::vector<std::vector<int>>& a_meld_logic_vector, Card* a_card_ptr) const;
	std::pair<int,int> Get_Best_MeldCardIndexPair_From_Logic(std::vector<std::vector<int>>& a_meld_logic_vector) const;

};
