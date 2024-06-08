/* 
 * File:   main.cpp
 * Author: ajporter
 *
 *      Created: May 21, 2024, 11:08 AM
 *      Purpose: Project 2 - Building on Project 1 Blackjack game with trees and
 *                           graphs concepts from past month
 */

// System Libraries
#include <iostream>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <random>
#include <vector>
#include <unordered_map>
using namespace std;

#include "deck.h"

// Function prototypes
void prntTitle();
int factorial(int n);
int fibonacci(int n);
void quicksort(vector<Card>& cards, int low, int high);
int partition(vector<Card>& cards, int low, int high);
void inorderTraversal(Node* root);

int main() {
    prntTitle();
    cout << "--------------------------\n";
    list<Card> deck;
    list<Card> playerHand;
    list<Card> dealerHand;

    // Add a betting feature
    int playerBalance = 1000; // Initial balance
    int bet = 0;
    int playerScore = 0;

    // Hash table to keep track of cards in the deck
    unordered_map<string, Card> cardHashTable;

    // Binary Search Tree (BST) for storing cards
    BST cardBST;

    while (playerBalance > 0) {
        // Ask for bet amount
        cout << "Current balance: $" << playerBalance << endl;
        cout << "Place your bet: $";
        cin >> bet;

        if (bet <= 0 || bet > playerBalance) {
            cout << "Invalid bet amount. Please place a bet within your balance." << endl;
            continue;
        }

        // Step 2: Create and shuffle the deck
        Deck deckObj;
        deckObj.shuffleDeck();

        // Step 3: Deal initial cards to player and dealer
        deckObj.dealInitialCards(playerHand, dealerHand);

        // Sort the player's hand using recursive quicksort
        vector<Card> playerHandVec(playerHand.begin(), playerHand.end());
        quicksort(playerHandVec, 0, playerHandVec.size() - 1);
        playerHand = list<Card>(playerHandVec.begin(), playerHandVec.end());

        // Sort the dealer's hand using recursive quicksort
        vector<Card> dealerHandVec(dealerHand.begin(), dealerHand.end());
        quicksort(dealerHandVec, 0, dealerHandVec.size() - 1);
        dealerHand = list<Card>(dealerHandVec.begin(), dealerHandVec.end());

        // Step 4: Display initial hands
        deckObj.displayInitialHands(playerHand, dealerHand);

        // Step 5: Player's turn
        deckObj.playerTurn(playerHand);

        // Step 6: Dealer's turn
        deckObj.dealerTurn(dealerHand, &Deck::calculateHandValue);

        // Step 7: Compare hands and determine the winner
        deckObj.determineWinner(playerHand, dealerHand);

        // Step 8: Update player's score based on game outcome
        deckObj.updateScore(playerScore, playerHand, dealerHand);

        // Update player's balance based on the bet outcome
        if (playerScore > 0) {
            playerBalance += bet;
        } else {
            playerBalance -= bet;
        }

        // Ask for replay
        char choice;
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            break;
        }

        // Reset hands
        playerHand.clear();
        dealerHand.clear();
    }

    cout << "Game over. Your final balance is $" << playerBalance << endl;
    // Exit stage right
    return 0;
}

void prntTitle() {
    cout << "WELCOME TO BLACKJACK\n";
    cout << " ___   ___\n";
    cout << "|A  | |10 |\n";
    cout << "| ♣ | | ♦ |\n";
    cout << "|__A| |_10|\n";
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void quicksort(vector<Card>& cards, int low, int high) {
    if (low < high) {
        int pi = partition(cards, low, high);
        quicksort(cards, low, pi - 1);
        quicksort(cards, pi + 1, high);
    }
}

int partition(vector<Card>& cards, int low, int high) {
    Card pivot = cards[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (cards[j].value < pivot.value) {
            i++;
            swap(cards[i], cards[j]);
        }
    }
    swap(cards[i + 1], cards[high]);
    return (i + 1);
}

void inorderTraversal(Node* root) {
    if (root == nullptr) return;
    inorderTraversal(root->left);
    cout << root->card.rank << " of " << root->card.suit << endl;
    inorderTraversal(root->right);
}
//DECK FUNCTIONS
Deck::Deck() {
    string suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
    string vals[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    for (const auto& suit : suits) {
        for (const auto& value : vals) {
            Card card;
            card.suit = suit;
            card.rank = value;
            deck.push_back(card);
            remainingCards[card.rank + card.suit]++;
        }
    }
}

void Deck::shuffleDeck() {
    random_device rd;
    mt19937 rng(rd());
    list<Card> shuffledDeck;
    while (!deck.empty()) {
        auto it = next(deck.begin(), rng() % deck.size());
        shuffledDeck.splice(shuffledDeck.end(), deck, it);
    }
    deck.swap(shuffledDeck);
}

void Deck::dealInitialCards(list<Card>& playerHand, list<Card>& dealerHand) {
    shuffleDeck();

    playerHand.push_back(deck.front());
    usedCards.insert(deck.front().rank + deck.front().suit);
    deck.pop_front();
    updateRemainingCards(playerHand.back());
    playerDeck.push(playerHand.back());

    dealerHand.push_back(deck.front());
    usedCards.insert(deck.front().rank + deck.front().suit);
    deck.pop_front();
    updateRemainingCards(dealerHand.back());
    dealerDeck.push(dealerHand.back());

    playerHand.push_back(deck.front());
    usedCards.insert(deck.front().rank + deck.front().suit);
    deck.pop_front();
    updateRemainingCards(playerHand.back());
    playerDeck.push(playerHand.back());

    while (true) {
        const Card& dealerCard = deck.front();
        if (usedCards.find(dealerCard.rank + dealerCard.suit) == usedCards.end()) {
            dealerHand.push_back(dealerCard);
            usedCards.insert(dealerCard.rank + dealerCard.suit);
            deck.pop_front();
            updateRemainingCards(dealerHand.back());
            dealerDeck.push(dealerHand.back());
            break;
        }
        shuffleDeck();
    }
}

void Deck::updateRemainingCards(const Card& card) {
    remainingCards[card.rank + card.suit]--;
}

void Deck::displayInitialHands(const list<Card>& playerHand, const list<Card>& dealerHand) const {
    cout << "PLAYER'S HAND:" << endl;
    for (const auto& card : playerHand) {
        cout << card.rank << " of " << card.suit << endl;
    }
    cout << endl;

    cout << "DEALER'S HAND:" << endl;
    const Card& firstCard = dealerHand.front();
    cout << firstCard.rank << " of " << firstCard.suit << endl;
    cout << "[Hidden Card]" << endl;
}

void Deck::playerTurn(list<Card>& playerHand) {
    char choice;
    do {
        cout << "\nDo you want to hit (h) or stand (s)?" << endl;
        cin >> choice;

        if (choice == 'h') {
            playerHand.push_back(deck.front());
            deck.pop_front();
            updateRemainingCards(playerHand.back());

            cout << "PLAYER'S HAND:" << endl;
            for (const auto& card : playerHand) {
                cout << card.rank << " of " << card.suit << endl;
            }
            cout << endl;

            int handValue = calculateHandValue(playerHand);
            if (handValue > 21) {
                cout << "Player busts! Your hand value is over 21." << endl;
                break;
            }
        }
    } while (choice == 'h');
}

void Deck::dealerTurn(list<Card>& dealerHand, int (Deck::*calculateHandValue)(const list<Card>&) const) {
    cout << "DEALER'S HAND:" << endl;
    for (const auto& card : dealerHand) {
        cout << card.rank << " of " << card.suit << endl;
    }
    cout << endl;

    while ((this->*calculateHandValue)(dealerHand) < 17) {
        if (dealerDeck.empty()) {
            cout << "Dealer's deck is empty!" << endl;
            break;
        }

        dealerHand.push_back(dealerDeck.top());
        dealerDeck.pop();
        updateRemainingCards(dealerHand.back());

        cout << "Dealer hits and receives a card." << endl;
        cout << "Dealer's Hand:" << endl;
        for (const auto& card : dealerHand) {
            cout << card.rank << " of " << card.suit << endl;
        }
        cout << endl;

        break;
    }
}

bool Deck::isNumeric(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

int Deck::calculateHandValue(const list<Card>& hand) const {
    int sum = 0;
    int numAces = 0;

    for (const auto& card : hand) {
        if (card.rank == "A") {
            numAces++;
        } else if (card.rank == "J" || card.rank == "Q" || card.rank == "K") {
            sum += 10;
        } else if (isNumeric(card.rank)) {
            sum += stoi(card.rank);
        }
    }

    for (int i = 0; i < numAces; ++i) {
        if (sum + 11 <= 21) {
            sum += 11;
        } else {
            sum += 1;
        }
    }

    return sum;
}

void Deck::determineWinner(const list<Card>& playerHand, const list<Card>& dealerHand) const {
    int playerValue = calculateHandValue(playerHand);
    int dealerValue = calculateHandValue(dealerHand);

    if (playerValue > 21) {
        cout << "Player busts! Dealer wins." << endl;
    } else if (dealerValue > 21) {
        cout << "Dealer busts! Player wins." << endl;
    } else {
        if (playerValue > dealerValue) {
            cout << "Player wins!" << endl;
        } else if (playerValue < dealerValue) {
            cout << "Dealer wins!" << endl;
        } else {
            cout << "It's a push! No one wins." << endl;
        }
    }
}

void Deck::updateScore(int& playerScore, const list<Card>& playerHand, const list<Card>& dealerHand) const {
    int playerValue = calculateHandValue(playerHand);
    int dealerValue = calculateHandValue(dealerHand);

    if (playerValue > 21 || (playerValue < dealerValue && dealerValue <= 21)) {
        cout << "Player loses this round." << endl;
        playerScore--;
    } else if ((dealerValue > 21 || playerValue > dealerValue) && playerValue <= 21) {
        cout << "Player wins this round!" << endl;
        playerScore++;
    } else {
        cout << "It's a tie! No change in score." << endl;
    }

    cout << "Player's score: " << playerScore << endl;
}

bool Deck::compareCards(const Card& card1, const Card& card2) {
    int value1 = card1.rank == "A" ? 1 : (isNumeric(card1.rank) ? stoi(card1.rank) : 10);
    int value2 = card2.rank == "A" ? 1 : (isNumeric(card2.rank) ? stoi(card2.rank) : 10);
    return value1 < value2;
}

void Deck::printDeck() const {
    for (const auto& card : deck) {
        cout << card.rank << " of " << card.suit << endl;
    }
}
