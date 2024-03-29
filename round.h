
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
#include <fstream>
#include <string>

#include "deck.h"
#include "human.h"
#include "player.h"
#include "computer.h"

// Class is act as the referee for the game by controlling the public methods
// for the Players, Cards, and Deck. and telling them what to do.
class Round {
   public:
	Human m_player1;
	Computer m_player2;
	//The two players should be above m_players because I am going to create a reference of them in my array.
	std::vector<Player*> m_players;
	Deck m_deck;

	int m_cur_round_number;
	Round();
	void Play_A_Round();
	void Deal_Cards_From_Deck_To_Players();
	void Decide_First_Player_Through_Coin_Toss();
	void Set_Player_1_Lead_Player();
	void Set_Player_2_Lead_Player();
	const std::vector<int> &Get_Scores() const;

	void Reset_Round();

	void Set_Cur_Round_Number(int a_cur_round);
	void Increase_Round_Number();
	void Set_Previous_Scores(std::vector<int> a_prev_scores);
	void Save_To_File(std::string a_path) const;
	void Load_From_File(std::string a_path);

   private:
	std::vector<int> m_scores;
	std::vector<int> m_prev_scores;
	int m_cur_lead_player;
	Card* m_trump_card;
	void Pick_Up_Trump_Card();
	void Play_Cards_Against_Each_Other();
	void Play_Turn_And_Pick_Up_From_Stock();
	void Play_Turn_And_Dont_Pick_Up_From_Stock();

	void Print_Player1_Hand() const;
	void Print_Player2_Hand() const;
	void Print_Interface_Message() const;

	void Ask_Input_From_Menu(int a_cur_player) const;

	bool m_has_trump_card_been_used;
};