/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   bst.h
 * Author: ajporter
 *
 * Created on May 27, 2024, 3:17 PM
 */

// bst.h
#ifndef BST_H
#define BST_H

#include "card.h"
#include <iostream>

struct Node {
    Card card;
    Node* left;
    Node* right;
    Node(const Card& card) : card(card), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    Node* insert(Node* node, const Card& card) {
        if (node == nullptr) return new Node(card);
        if (card.value < node->card.value) node->left = insert(node->left, card);
        else node->right = insert(node->right, card);
        return node;
    }

    void inorder(Node* node) const {
        if (node == nullptr) return;
        inorder(node->left);
        cout << node->card.rank << " of " << node->card.suit << endl;
        inorder(node->right);
    }

public:
    BST() : root(nullptr) {}

    void insert(const Card& card) {
        root = insert(root, card);
    }

    void inorderTraversal() const {
        inorder(root);
    }
};

#endif // BST_H
