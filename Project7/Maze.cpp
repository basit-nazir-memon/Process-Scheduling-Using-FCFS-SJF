#include "Maze.h"

node::node() {
	x = 0;
	y = 0;
	next = NULL;
}
Stack::Stack() {
	head = NULL;
}
void Stack::push(int x, int y) {
	if (head == NULL) {
		head = new node;
		head->next = NULL;
	}
	else {
		node* temp = new node;
		temp->next = head;
		head = temp;
	}
	head->x = x;
	head->y = y;
}
node* Stack::pop() {
	if (head == NULL) {
		cout << "The stack is Empty\n";
		return NULL;
	}
	else {
		node* temp = head;
		head = head->next;
		return temp;
	}
}
node* Stack::peek() {
	if (head == NULL) {
		cout << "The stack is Empty\n";
		return NULL;
	}
	return head;
}
void Stack::clear() {
	node* temp = head;
	while (head) {
		temp = head;
		head = head->next;
		delete temp;
	}
}
bool Stack::isEmpty() {
	return head == NULL;
}
void Stack::print() {
		if (!head) {
			cout << "Stack Empty\n";
			return;
		}
	int i = 1;
	node* temp = head;
	cout << "-------- Stack Entries -------\n";
	while (temp) {
		cout << "Item " << i++ << " X : " << temp->x << "\t Y : " << temp->y << endl;
		temp = temp->next;
	}
}
Stack::~Stack() {
	node* temp = head;
	while (head) {
		temp = head;
		head = head->next;
		delete temp;
	}
}

void MazeSolver::readFile() {
	ifstream file;
	file.open("./Maze.txt", ios::in);
	while (file.get() != ':') {

	}
	file.get();
	string abc = " ";
	abc[0] = file.get();
	row = stoi(abc);
	while (file.get() != ':') {

	}
	file.get();
	abc[0] = file.get();
	col = stoi(abc);
	while (file.get() != ':') {

	}
	file.get();
	abc[0] = file.get();
	hurdles = stoi(abc);
	file.get();

	maze = new int* [row];
	for (int i = 0; i < row; i++) {
		maze[i] = new int[col];
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			maze[i][j] = 0;
		}
	}

	int i = 0, j = 0;
	while (!file.eof()) {
		abc[0] = file.get();
		i = stoi(abc);
		file.get();
		abc[0] = file.get();
		j = stoi(abc);
		file.get();
		maze[i][j] = -1;
	}
	file.close();
}
MazeSolver::MazeSolver() {
	readFile();
}
void MazeSolver::print() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout << maze[i][j] << '\t';
		}
		cout << endl;
	}
}
void MazeSolver::solve() {
int r = 0, c = 0;
route.push(r, c);
maze[r][c] = -5;
int blocked = 0;
bool found = true;
while (r != row - 1 || c != col - 1) {
	blocked = 0;
	if (c + 1 < col && maze[r][c + 1] != -1 && maze[r][c + 1] != -5) {
		route.push(r, ++c);
		maze[r][c] = -5;
	}
	else if (r + 1 < row && maze[r + 1][c] != -1 && maze[r + 1][c] != -5) {
		route.push(++r, c);
		maze[r][c] = -5;
	}
	else if (r - 1 >= 0 && maze[r - 1][c] != -1 && maze[r - 1][c] != -5) {
		route.push(--r, c);
		maze[r][c] = -5;
	}
	else if (c - 1 >= 0 && maze[r][c - 1] != -1 && maze[r][c - 1] != -5) {
		route.push(r, --c);
		maze[r][c] = -5;
	}
	else {
		route.pop();
		node* temp = route.peek();
		maze[r][c] = -1;
		r = temp->x;
		c = temp->y;
		//delete temp;
	}
	if (c - 1 >= 0 && maze[r][c - 1] == -1) {
		blocked++;
	}
	if (c + 1 < col && maze[r][c + 1] == -1) {
		blocked++;
	}
	if (r - 1 >= 0 && maze[r - 1][c] == -1) {
		blocked++;
	}
	if (r + 1 < row && maze[r + 1][c] == -1) {
		blocked++;
	}
	if ((r == 0 && c == 0) || (r == 0 && c == col - 1) || (r == row - 1 && c == 0) || (r == row - 1 && c == col - 1)) {
		blocked += 2;
	}
	if (blocked > 3) {
		found = false;
		break;
	}
}
if (found) {
	cout << "---------- ROUTE FOUND ---------\n";
	route.print();
	cout << "SEE IN 2D FORM\n";
	print();
}
else {
	cout << "---------- NO ROUTE FOUND ---------\n";
	route.print();
	cout << "SEE IN 2D FORM\n";
	print();
}
}
