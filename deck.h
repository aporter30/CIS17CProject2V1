/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   deck.h
 * Author: ajporter
 *
 * Created on May 27, 2024, 3:18 PM
 */

#ifndef DECK_H
#define DECK_H

#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_map>
#include "card.h"
#include "bst.h"

class Deck {
private:
    list<Card> deck;
    set<string> usedCards;
    map<string, int> remainingCards;
    stack<Card> dealerDeck;
    queue<Card> playerDeck;
    unordered_map<string, Card> cardHashTable;

public:
    Deck();
    void shuffleDeck();
    void dealInitialCards(list<Card>& playerHand, list<Card>& dealerHand);
    void updateRemainingCards(const Card& card);
    void displayInitialHands(const list<Card>& playerHand, const list<Card>& dealerHand) const;
    void playerTurn(list<Card>& playerHand);
    void dealerTurn(list<Card>& dealerHand, int (Deck::*calculateHandValue)(const list<Card>&) const);
    static bool isNumeric(const string& str);
    int calculateHandValue(const list<Card>& hand) const;
    void determineWinner(const list<Card>& playerHand, const list<Card>& dealerHand) const;
    void updateScore(int& playerScore, const list<Card>& playerHand, const list<Card>& dealerHand) const;
    void printDeck() const;
    static bool compareCards(const Card& card1, const Card& card2);
    void addCardToHashTable(const Card& card);
};

#endif // DECK_H
