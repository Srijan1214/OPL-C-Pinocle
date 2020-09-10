#include "round.h"

Round::Round() : m_players({&m_player1,&m_player2}), m_scores(2,0){
	std::cout << "round constructor" << std::endl;
}

void Round::Play_A_Round(int a_cur_round) {
	Deal_Cards_From_Deck_To_Players();
	if(a_cur_round == 0) {
		Set_Cur_Turn_Through_Coin_Toss(m_player1);
	}

	Pick_Up_Trump_Card();
	// Tell the players which suit is trump suit
	for(Player* player_ptr:m_players) {
		player_ptr->Set_Trump_Suit(m_trump_card->Get_Suit());
	}

	Print_Interface_Message();

	for(int i = 0; i < 11; i++) {
		// The following function will update lead and chase player based on the outcome of the battle.
		Play_Cards_Against_Each_Other();
		//lead player declares meld
		Print_Interface_Message();
		m_players[m_cur_lead_player]->Get_Meld_To_Play();

		const int chase_player = m_cur_lead_player ^ 1;
		m_players[m_cur_lead_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
		m_players[chase_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
		Print_Interface_Message();
	}

	// In the following battle, the stock pile is finished and the trump card is given to the chase player.
	Play_Cards_Against_Each_Other();
	//lead player declares meld
	Print_Interface_Message();
	m_players[m_cur_lead_player]->Get_Meld_To_Play();

	const int chase_player = m_cur_lead_player ^ 1;
	m_players[m_cur_lead_player]->Give_Card_To_Player(m_deck.Pop_Top_Card());
	m_players[chase_player]->Give_Card_To_Player(m_trump_card);

	Print_Interface_Message();
	//From now on cards aren't picked up from the stock pile.
	for(int i = 0; i < 12; i++) {
		// The following function will update lead and chase player based on the outcome of the battle.
		Play_Cards_Against_Each_Other();
		//lead player declares meld
		Print_Interface_Message();
		m_players[m_cur_lead_player]->Get_Meld_To_Play();

	}

}

void Round::Play_Cards_Against_Each_Other() {
	const int chase_player = m_cur_lead_player ^ 1; // The other of the lead players
	Card* lead_card = m_players[m_cur_lead_player]->Get_Card_To_Play(NULL);
	Card* chase_card = m_players[chase_player]->Get_Card_To_Play(lead_card);

	int who_won = -1;

	if(lead_card->Get_Suit() == m_trump_card->Get_Suit()) {
		if(chase_card->Get_Suit() != m_trump_card->Get_Suit()) {
			who_won = m_cur_lead_player;
		} else {
			if(lead_card->Get_Card_Weight() >= chase_card->Get_Card_Weight() ) {
				who_won = m_cur_lead_player;
			} else {
				who_won = chase_player;
			}
		}
	}else {
		if(lead_card->Get_Card_Weight() >= chase_card->Get_Card_Weight() ) {
			who_won = m_cur_lead_player;
		} else {
			who_won = chase_player;
		}
	}

	if(who_won == m_cur_lead_player) {
		m_scores[m_cur_lead_player] += (lead_card->Get_Card_Weight() + chase_card->Get_Card_Weight());
		m_players[m_cur_lead_player]->Add_Cards_To_Player_Capture_Pile(lead_card, chase_card);
	} else {
		m_scores[m_cur_lead_player] += (lead_card->Get_Card_Weight() + chase_card->Get_Card_Weight());
		m_players[m_cur_lead_player]->Add_Cards_To_Player_Capture_Pile(lead_card, chase_card);
		//switch lead and chase player uf chase player wins
		m_cur_lead_player^= 1;
	}

	if(who_won == 0) {
		std::cout << "Player 1 Won" << std::endl;
	} else
	{
		std::cout << "Player 2 won" << std::endl;
	}
	
}

void Round::Deal_Cards_From_Deck_To_Players() {
	m_deck.Shuffle_Deck();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			Card* card_to_give = m_deck.Pop_Top_Card();
			m_players[0]->Give_Card_To_Player(card_to_give);
		}

		for (int j = 0; j < 4; j++) {
			Card* card_to_give = m_deck.Pop_Top_Card();
			m_players[1]->Give_Card_To_Player(card_to_give);
		}
	}
}

void Round::Set_Cur_Turn_Through_Coin_Toss(const Human& a_human_player) {
	const int coin_toss_value = rand() % 1; // Tails is 1 and Heads is 0.

	char human_choice = a_human_player.Get_Heads_Or_Tails();

	bool was_user_correct = (human_choice == 'T' && coin_toss_value == 1 ) || (human_choice == 'H' && coin_toss_value == 0 );

	if(was_user_correct) {
		m_cur_lead_player = 0;
	} else {
		m_cur_lead_player = 1;
	}
}

void Round::Pick_Up_Trump_Card() {
	m_trump_card = m_deck.Pop_Top_Card();
}

const std::vector<int> & Round::Get_Scores() const {
	return m_scores;
}

void Round::Print_Player1_Hand() {
	std::cout << "Human:" << std::endl;
	std::cout << m_player1.Get_Console_Message() << std::endl;
}

void Round::Print_Player2_Hand() {
	std::cout << "Computer:" << std::endl;
	std::cout << m_player2.Get_Console_Message() << std::endl;
}

void Round::Print_Interface_Message() {
	std::cout << "Round: " << 1 << std::endl;
	std::cout << std::endl;
	Print_Player1_Hand();
	Print_Player2_Hand();
	std::cout << "Trump Card: " << m_trump_card->Get_Card_String_Value()  << std::endl;
	std::cout << "Stocks: " << m_deck.Get_Stock_String() <<std::endl;
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
}