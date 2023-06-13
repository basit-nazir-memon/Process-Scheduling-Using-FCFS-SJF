#pragma once
#include <iostream>
#include <string>
#include "rapidxml.hpp"
#include <fstream>
#include <vector>

using namespace rapidxml;
using namespace std;

class ExecutionOrder {
public:
	string order;
	ExecutionOrder* next;
	ExecutionOrder();
};

class instruction {
public:
	string name;
	int time;
	instruction* next;
	instruction();
	instruction(string n, int t);
	bool isDevice();
};

class processNode {
public:
	int id;
	instruction* instruct;
	instruction* instructTail;
	int arrivalTime;
	int completionTime;
	int burstTime;
	int waitingTime;
	string type;
	string status;
	processNode* next;
	instruction* executedCodeHead;
	instruction* executedCodeTail;
	ExecutionOrder* executionOrderHead;
	ExecutionOrder* executionOrderTail;

	bool isEmpty();
	processNode();
	processNode(int ID, int at, string Type, string Status, string code);
	string peekType();
	int calculateBurstTime();
	processNode* setStatus(string st);
	int calculateWaitingTime();
	void divideInstructions(string input);
	void enqueueOrder(string input);
	void enqueueExecuted(instruction* input);
	void enqueue(instruction* input);
	void enqueue(string input);
	instruction* dequeue();
	void print();
	void printOrder();
	void printExecutedCode();

};

class Queue {
	string type;
	processNode* process;
	processNode* lastProcess;
public:
	Queue();
	void incrementWaitingTime();
	int findShortBurstTime();
	processNode* dequeue(int num);
	processNode* removeShortProcess();
	void setType(string input);
	void enqueue(processNode* node, int time);
	processNode* dequeue();
	int noOfProcess();
	bool isEmpty();
	processNode* peek();
	void printQueue();
	void printContent();
	void print();
};

class MultiLevelQueue {
	string name;
	Queue high;
	Queue ready;
	Queue low;
public:
	void setName(string input);
	string getName();
	Queue& getHigh();
	Queue& getReady();
	Queue& getLow();
	bool isEmpty();
};

class DeviceQueue {
	Queue disk;
	Queue screen;
	Queue keyboard;
	Queue file;
	Queue printer;
public:
	void addProcess(processNode* process, int time);
	Queue& getDisk();
	Queue& getScreen();
	Queue& getKeyboard();
	Queue& getFile();
	Queue& getPrinter();
	void execute(processNode& executed, Queue& lowPr, Queue& Completion, int time);
	bool isEmpty();
};

class Driver {
	Queue Completion;
	Queue waitingQueue;
	MultiLevelQueue multiQueue;
	DeviceQueue devQueue;
	processNode executed;

	void readXML(Queue& waitingQueue, MultiLevelQueue& multiQueue);
public:
	Driver();
	Queue getWaitingQueue();
	void printBreakpoint();
	void execute();
};
