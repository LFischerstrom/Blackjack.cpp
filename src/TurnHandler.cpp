#include "headers/Blackjack.h"
#include "headers/TurnHandler.h"

using namespace std;

TurnHandler::TurnHandler(Blackjack* game) {
	this->game = game;
}


void TurnHandler::handlePlayersTurn(Player* player) {
	this->player = player;
	this->turnIsDone = false;

	this->printTurnHeader();

	while(!this->turnIsDone) {
		int action = player->askForAction();
		this->executeAction(action);
	}

	cout << player->toString() << "s turn ended" << endl;
}


void TurnHandler::printTurnHeader() {
	cout << endl;
	cout << "=== " + this->player->toString() + "'s turn! ===" << endl;
}

void TurnHandler::executeAction(int action) {
	switch(action) {
		case 1: {
			Card* newCard = this->game->dealer.dealCard();
			this->player->getHand()->operator+=(newCard); // Operator += overloaded in Hand.cpp

			cout << endl;
			cout << "You got: " + newCard->toString() << endl;
			cout << endl;

			int currentBlackjackSum = this->player->getHand()->getBlackjackSum();

			if(currentBlackjackSum > 21) {
				cout << "You just got fat! :(" << endl;
				this->turnIsDone = true;
			} else if(currentBlackjackSum == 21) {
				cout << "BLACKJACK!!!" << endl;
				this->turnIsDone = true;
			} else {
				cout << "You now have the following hand:" << endl;
				this->player->printHand();
				this->turnIsDone = false;
			}

			break;
		}
		case 2: {
			cout << endl;
			cout << "You chose to stop at " << this->player->getHand()->getBlackjackSum() << endl;
			this->turnIsDone = true;

			break;
		}
	}
}


void TurnHandler::finishDealersTurn() {
	Dealer* dealer = &this->game->dealer;

	cout << endl;
	cout << "Dealer's turn!" << endl;

	while(dealer->currentBlackjackSum() < 17) {
		Card* c = dealer->dealCard();

		cout << "Dealer got " << c->toString() << endl;
		dealer->receiveCard(c);
	}

	if(dealer->currentBlackjackSum() > 21) {
		cout << "Dealer got fat!" << endl;
	} else {
		cout << "Dealer stopped at " << dealer->currentBlackjackSum() << endl;
	}
}


void TurnHandler::presentTurnOutcome() {
	cout << endl;
	cout << "=== Round ended! ===" << endl;
	cout << endl;
	cout << "Dealer got " << this->game->dealer.currentBlackjackSum() << endl;

	for(Player* player : this->game->players) {
		if(player->getHand()->getBlackjackSum() > 21) {
			cout << player->toString() << " got fat at " << player->getHand()->getBlackjackSum() << endl;
		} else if(player->getHand()->getBlackjackSum() > this->game->dealer.currentBlackjackSum()) {
			cout << player->toString() << " won over the dealer!" << endl;
		} else {
			cout << player->toString() + " lost against the dealer!" << endl;
		}
	}
}