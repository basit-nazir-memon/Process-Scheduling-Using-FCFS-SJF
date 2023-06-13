#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
using namespace std;

class node {
public:
	int x;
	int y;
	node* next;
	node();
};

class Stack {
	node* head;
public:
	Stack();
	void push(int x, int y);
	node* pop();
	node* peek();
	void clear();
	bool isEmpty();
	void print();
	~Stack();
};

class MazeSolver {
	int row;
	int col;
	int hurdles;
	int** maze;
	Stack route;
	void readFile();
public:
	MazeSolver();
	void print();
	void solve();
};