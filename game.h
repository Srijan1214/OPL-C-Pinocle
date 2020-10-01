
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
#include "round.h"
#include <iostream>

class Game{
	public:
		Game();
		void Start_The_Game();
	private:
		Round m_game_round;
		int m_player1_cumulative_score;
		int m_player2_cumulative_score;
};