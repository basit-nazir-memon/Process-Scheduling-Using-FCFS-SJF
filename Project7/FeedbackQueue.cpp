#include "FeedbackQueue.h"

ExecutionOrder::ExecutionOrder() {
	order = "";
	next = NULL;
}


instruction::instruction() {
	name = "";
	next = NULL;
	time = 0;
}
instruction::instruction(string n, int t) {
	name = n;
	time = t;
	next = NULL;
}
bool instruction::isDevice() {
	if (name == "WRITEMEMORY" || name == "READMEMORY" || name == "COMPUTE" || name == "BREAK") {
		return false;
	}
	return true;
}

bool processNode::isEmpty() {
	return (instruct == NULL);
}

processNode::processNode() {
	id = 0;
	instruct = NULL;
	instructTail = NULL;
	arrivalTime = 0;
	burstTime = 0;
	completionTime = 0;
	waitingTime = 0;
	status = "";
	type = ""; // IO-Bound / CPU-Bound
	next = NULL;
	enqueueOrder("Waiting Queue");
}

processNode::processNode(int ID, int at, string Type, string Status, string code) {
	id = ID;
	instruct = NULL;
	instructTail = NULL;
	arrivalTime = at;
	status = Status;
	type = Type;
	divideInstructions(code);
	burstTime = calculateBurstTime();
	completionTime = 0;
	waitingTime = 0;
	next = NULL;
	enqueueOrder("Waiting Queue");
}

string processNode::peekType() {
	if (instruct->name == "WRITESCREEN" || instruct->name == "READSCREEN") {
		return "SCREEN";
	}
	if (instruct->name == "WRITEKEYBOARD" || instruct->name == "READKEYBOARD") {
		return "KEYBOARD";
	}
	if (instruct->name == "WRITEPRINTER" || instruct->name == "READPRINTER") {
		return "PRINTER";
	}
	if (instruct->name == "WRITEDISK" || instruct->name == "READDISK") {
		return "DISK";
	}
	if (instruct->name == "WRITEFILE" || instruct->name == "READFILE") {
		return "FILE";
	}
	if (instruct->name == "BREAK") {
		return "BREAK";
	}
	return "NODEVICE";
}

int processNode::calculateBurstTime() {
	int time = 0;
	instruction* ptr = instruct;
	while (ptr) {
		time += ptr->time;
		ptr = ptr->next;
	}
	return time;
}

processNode* processNode::setStatus(string st) {
	status = st;
	return this;
}

int processNode::calculateWaitingTime() {
	return 0;
}

void processNode::divideInstructions(string input) {
	int i = 0;
	string subStr;
	while (input.length() > 5) {
		for (i = 1; input[i] != '\n'; i++) {}
		subStr = input.substr(1, i - 1);
		input.erase(0, i);
		enqueue(subStr);
	}
}

void processNode::enqueueOrder(string input) {
	if (executionOrderTail == NULL) {
		executionOrderTail = new ExecutionOrder;
		executionOrderHead = executionOrderTail;
	}
	else {
		executionOrderTail->next = new ExecutionOrder;
		executionOrderTail = executionOrderTail->next;
	}
	executionOrderTail->order = input;
	executionOrderTail->next = NULL;
}

void processNode::enqueueExecuted(instruction* input) {
	if (executedCodeTail == NULL) {
		executedCodeTail = input;
		executedCodeHead = executedCodeTail;
	}
	else {
		executedCodeTail->next = input;
		executedCodeTail = executedCodeTail->next;
	}
	executedCodeTail->next = NULL;
}

void processNode::enqueue(instruction* input) {
	if (instruct == NULL) {
		instruct = input;
		instructTail = instruct;
	}
	else {
		instructTail->next = input;
		instructTail = instructTail->next;
	}
	instructTail->next = NULL;
}

void processNode::enqueue(string input) {
	if (instruct == NULL) {
		instruct = new instruction;
		instructTail = instruct;
	}
	else {
		instructTail->next = new instruction;
		instructTail = instructTail->next;
	}
	instructTail->name = input;
	instructTail->next = NULL;
	if (input == "WRITEMEMORY" || input == "READMEMORY" || input == "COMPUTE" || input == "BREAK") {
		instructTail->time = 1;
	}
	else {
		instructTail->time = 3;
	}
}

instruction* processNode::dequeue() {
	if (!instruct) {
		cout << "No Instructions In Process\n";
		return NULL;
	}
	instruction* temp = instruct;
	instruct = instruct->next;
	temp->next = NULL;
	enqueueExecuted(temp);
	return temp;
}

void processNode::print() {
	if (!instruct) {
		cout << "No Instructions\n";
		return;
	}
	instruction* temp = instruct;
	while (temp) {
		cout << temp->name << endl;
		temp = temp->next;
	}
}

void processNode::printOrder() {
	if (!executionOrderHead) { cout << "No Execution Held\n"; return; }
	ExecutionOrder* temp = executionOrderHead;
	cout << "Execution Order: \n";
	while (temp) {
		cout << temp->order;
		temp = temp->next;
		if (temp) {
			cout << "  ->  ";
		}
	}
	cout << endl;
	return;
}

void processNode::printExecutedCode() {
	if (!executedCodeHead) { cout << "No Code Executed\n"; return; }
	instruction* temp = executedCodeHead;
	cout << "Executed Code: \n";
	while (temp) {
		cout << temp->name << endl;
		temp = temp->next;
	}
	return;
}


Queue::Queue() {
	type = "";
	process = NULL;
	lastProcess = NULL;
}

void Queue::incrementWaitingTime() {
	if (!process) { return; }
	processNode* temp = process;
	while (temp) {
		temp->waitingTime++;
		temp = temp->next;
	}
}

int Queue::findShortBurstTime() {
	if (!process) { return 100; }
	int time = process->burstTime;
	processNode* temp = process;
	while (temp) {
		if (temp->burstTime > time) {
			time = temp->burstTime;
		}
		temp = temp->next;
	}
	return time;
}

processNode* Queue::dequeue(int num) {
	if (!process) {
		cout << "Queue Does Not Have Process\n";
		return NULL;
	}
	processNode* temp = process;
	processNode* shortProcess = NULL;
	if (num == 0) {
		return dequeue();
	}
	for (int i = 0; i < num - 1 && temp; i++) {
		temp = temp->next;
	}
	shortProcess = temp->next;
	temp->next = shortProcess->next;
	shortProcess->next = NULL;
	return shortProcess;
}

processNode* Queue::removeShortProcess() {
	if (!process) { return NULL; }
	int time = findShortBurstTime(), i = 0;
	processNode* temp = process;
	while (temp && temp->burstTime != time) {
		temp = temp->next;
		i++;
	}
	return dequeue(i);
}
void Queue::setType(string input) { type = input; }

void Queue::enqueue(processNode* node, int time) {
	if (process == NULL) {
		process = node;
		lastProcess = node;
		if (lastProcess)
			lastProcess->completionTime = time;
	}
	else {
		lastProcess->next = node;
		lastProcess = lastProcess->next;
		if (lastProcess)
			lastProcess->completionTime = time;
	}

}

processNode* Queue::dequeue() {
	if (process == NULL) {
		//cout << "The queue is Empty\n";
		return NULL;
	}
	else {
		processNode* temp = process;
		process = process->next;
		temp->next = NULL;
		return temp;
	}
}

int Queue::noOfProcess() {
	int count = 0;
	processNode* temp = process;
	while (temp) {
		count++;
		temp = temp->next;
	}
	return count;
}

bool Queue::isEmpty() {
	return process == NULL;
}

processNode* Queue::peek() {
	return process;
}

void Queue::printQueue() {
	processNode* temp = process;
	if (!temp) { return; }
	while (temp) {
		cout << "_____________________________________________\n";
		cout << "ID: " << temp->id << endl << endl;
		cout << "Status: " << temp->status << endl << endl;
		cout << "Completion Time: " << temp->completionTime << "\n\n";
		cout << "Arrival Time: " << temp->arrivalTime << "\n\n";
		cout << "Burst Time: " << temp->burstTime << "\n\n";
		cout << "Waiting Time: " << temp->waitingTime << "\n\n";
		temp->printExecutedCode();
		cout << "\n";
		temp->printOrder();
		cout << "_____________________________________________\n";
		temp = temp->next;
	}
}

void Queue::printContent() {
	processNode* temp = process;
	if (!temp) { cout << "No Processess\n"; return; }
	while (temp) {
		cout << "Process ID : " << temp->id << endl;
		temp = temp->next;
	}
}

void Queue::print() {
	cout << "\t----- Queue DATA ------\n";
	processNode* temp = process;
	instruction* ptr;

	if (!temp) { cout << "No Data in Queue\n"; return; }
	while (temp) {
		cout << "\nProcess " << temp->id << endl;
		cout << "Arrival Time: " << temp->arrivalTime << endl;
		cout << "Burst Time: " << temp->burstTime << endl;
		cout << "Status: " << temp->status << endl;
		cout << "Type: " << temp->type << endl;
		cout << "Instructions:\n";
		ptr = temp->instruct;
		if (ptr) {
			cout << "Name\t\tTime\n";
			while (ptr) {
				cout << ptr->name << '\t';
				cout << ptr->time << endl;
				ptr = ptr->next;
			}
		}
		else {
			cout << "No Instructions\n";
		}
		temp = temp->next;
	}
}

void MultiLevelQueue::setName(string input) { name = input; }
string MultiLevelQueue::getName() { return name; }
Queue& MultiLevelQueue::getHigh() { return high; }
Queue& MultiLevelQueue::getReady() { return ready; }
Queue& MultiLevelQueue::getLow() { return low; }
bool MultiLevelQueue::isEmpty() {
	return high.isEmpty() && low.isEmpty() && ready.isEmpty();
}

void DeviceQueue::addProcess(processNode* process, int time) {
	if (process->peekType() == "DISK") {
		process->enqueueOrder("Device Disk Queue");
		disk.enqueue(process, time);
	}
	if (process->peekType() == "SCREEN") {
		process->enqueueOrder("Device Screen Queue");
		screen.enqueue(process, time);
	}
	if (process->peekType() == "KEYBOARD") {
		process->enqueueOrder("Device Keyboard Queue");
		keyboard.enqueue(process, time);
	}
	if (process->peekType() == "FILE") {
		process->enqueueOrder("Device File Queue");
		file.enqueue(process, time);
	}
	if (process->peekType() == "PRINTER") {
		process->enqueueOrder("Device Printer Queue");
		printer.enqueue(process, time);
	}
}
Queue& DeviceQueue::getDisk() { return disk; }
Queue& DeviceQueue::getScreen() { return screen; }
Queue& DeviceQueue::getKeyboard() { return keyboard; }
Queue& DeviceQueue::getFile() { return file; }
Queue& DeviceQueue::getPrinter() { return printer; }
void DeviceQueue::execute(processNode& executed, Queue& lowPr, Queue& Completion, int time) {
	if (!disk.isEmpty()) {
		if ((disk.peek()->instruct->time--) == 0) {
			instruction* ptr = disk.peek()->dequeue();
			//executed.enqueue(ptr);
			//disk.peek()->enqueueExecuted(ptr);
			disk.peek()->burstTime = disk.peek()->calculateBurstTime();

			if (!disk.peek()->isEmpty()) {
				processNode* processTemp = disk.dequeue();
				processTemp->status = "Waiting in Low Priority";
				lowPr.enqueue(processTemp, time);

			}
			else {
				processNode* temp = disk.dequeue()->setStatus("Executed");
				Completion.enqueue(temp, time);
			}
		}
	}
	if (!screen.isEmpty()) {
		if ((screen.peek()->instruct->time--) == 0) {
			instruction* ptr = screen.peek()->dequeue();
			screen.peek()->burstTime = screen.peek()->calculateBurstTime();
			//executed.enqueue(ptr);
			//screen.peek()->enqueueExecuted(ptr);
			if (!screen.peek()->isEmpty()) {
				processNode* processTemp = screen.dequeue();
				processTemp->status = "Waiting in Low Priority";
				lowPr.enqueue(processTemp, time);
			}
			else {
				processNode* temp = screen.dequeue()->setStatus("Executed");
				Completion.enqueue(temp, time);
			}
		}
	}
	if (!keyboard.isEmpty()) {
		if ((keyboard.peek()->instruct->time--) == 0) {
			instruction* ptr = keyboard.peek()->dequeue();
			keyboard.peek()->burstTime = keyboard.peek()->calculateBurstTime();
			//executed.enqueue(ptr);
			//keyboard.peek()->enqueueExecuted(ptr);
			if (!keyboard.peek()->isEmpty()) {
				processNode* processTemp = keyboard.dequeue();

				processTemp->status = "Waiting in Low Priority";
				lowPr.enqueue(processTemp, time);
			}
			else {
				processNode* temp = keyboard.dequeue()->setStatus("Executed");
				Completion.enqueue(temp, time);
			}
		}
	}
	if (!file.isEmpty()) {
		if ((file.peek()->instruct->time--) == 0) {
			instruction* ptr = file.peek()->dequeue();
			file.peek()->burstTime = file.peek()->calculateBurstTime();
			//executed.enqueue(ptr);
			//file.peek()->enqueueExecuted(ptr);
			if (!file.peek()->isEmpty()) {
				processNode* processTemp = file.dequeue();
				processTemp->status = "Waiting in Low Priority";
				lowPr.enqueue(processTemp, time);
			}
			else {
				processNode* temp = file.dequeue()->setStatus("Executed");
				Completion.enqueue(temp, time);
			}
		}
	}
	if (!printer.isEmpty()) {
		if ((printer.peek()->instruct->time--) == 0) {
			instruction* ptr = printer.peek()->dequeue();
			printer.peek()->burstTime = printer.peek()->calculateBurstTime();
			//executed.enqueue(ptr);
			//printer.peek()->enqueueExecuted(ptr);
			if (!printer.peek()->isEmpty()) {
				processNode* processTemp = printer.dequeue();
				processTemp->status = "Waiting in Low Priority";
				lowPr.enqueue(processTemp, time);
			}
			else {
				processNode* temp = printer.dequeue()->setStatus("Executed");
				Completion.enqueue(temp, time);
			}
		}
	}
}
bool DeviceQueue::isEmpty() { return disk.isEmpty() && screen.isEmpty() && keyboard.isEmpty() && file.isEmpty() && printer.isEmpty(); }

void Driver::readXML(Queue& waitingQueue, MultiLevelQueue& multiQueue) {
	ofstream file2;
	ifstream file1;
	string temp;
	file1.open("XMLFile.xml");
	file2.open("file.xml", ios::trunc);

	while (getline(file1, temp)) {
		if (temp == "<breakpoint/>") {
			file2 << "BREAK" << endl;
		}
		else {
			file2 << temp << endl;
		}
	}
	file1.close();
	file2.close();

	xml_document<> doc;
	xml_node<>* root_node = NULL;
	xml_node<>* queueSet = NULL;
	xml_node<>* configuration = NULL;
	xml_node<>* processes = NULL;
	xml_node<>* process = NULL;
	xml_node<>* child = NULL;
	xml_node<>* operation = NULL;


	ifstream file("file.xml");
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	// Find out the nodes
	root_node = doc.first_node("AssignmentInput");
	queueSet = root_node->first_node("QueueSet");
	configuration = root_node->first_node("Configuration");

	// Assiging the Name to Multilevel Queue
	multiQueue.setName(queueSet->first_node("SetName")->value());

	// Storing Configuration
	waitingQueue.setType("FCFS");
	processNode* p_node;
	int id, arrivalTime;
	string type, code;

	processes = configuration->first_node("Processes");
	process = processes->first_node("Process");

	for (int i = 0; process; process = process->next_sibling(), i++) {
		id = atoi(process->first_attribute("ID")->value());
		arrivalTime = atoi(process->first_node("ArrivalTime")->value());
		type = process->first_node("Type")->value();
		code = process->first_node("code")->value();
		p_node = new processNode(id, arrivalTime, type, "waiting", code);
		waitingQueue.enqueue(p_node, i);
	}
}
Driver::Driver() {
	readXML(waitingQueue, multiQueue);
}
Queue Driver::getWaitingQueue() { return waitingQueue; }

void Driver::printBreakpoint() {
	cout << "------------------- BREAK POINT --------------------\n";

	cout << "\t------- Processess -------\n";
	if (!Completion.isEmpty()) {
		Completion.printQueue();
	}

	if (!waitingQueue.isEmpty()) {
		waitingQueue.printQueue();
	}
	if (!multiQueue.getHigh().isEmpty()) {
		multiQueue.getHigh().printQueue();
	}
	if (!multiQueue.getLow().isEmpty()) {
		multiQueue.getLow().printQueue();
	}
	if (!multiQueue.getReady().isEmpty()) {
		multiQueue.getReady().printQueue();
	}
	if (!devQueue.getDisk().isEmpty()) {
		devQueue.getKeyboard().printQueue();
	}
	if (!devQueue.getDisk().isEmpty()) {
		devQueue.getKeyboard().printQueue();
	}
	if (!devQueue.getScreen().isEmpty()) {
		devQueue.getScreen().printQueue();
	}
	if (!devQueue.getPrinter().isEmpty()) {
		devQueue.getPrinter().printQueue();
	}
	if (!devQueue.getFile().isEmpty()) {
		devQueue.getFile().printQueue();
	}
	// The contents of different queues, including ready queue and device queues for each
	// device.

	cout << "\n------ Content / Process in Waiting Queue ------\n";
	waitingQueue.printContent();
	cout << "\n------ Content / Process in Ready Queue ------\n";
	multiQueue.getReady().printContent();
	cout << "\n------ Content / Process in High Priority Queue ------\n";
	multiQueue.getHigh().printContent();
	cout << "\n------ Content / Process in Low Priority Queue ------\n";
	multiQueue.getLow().printContent();
	cout << "\n------ Content / Process in Keyboard Device Queue ------\n";
	devQueue.getKeyboard().printContent();
	cout << "\n------ Content / Process in Screen Device Queue ------\n";
	devQueue.getScreen().printContent();
	cout << "\n------ Content / Process in Disk Device Queue ------\n";
	devQueue.getDisk().printContent();
	cout << "\n------ Content / Process in Printer Device Queue ------\n";
	devQueue.getPrinter().printContent();
	cout << "\n------ Content / Process in File Device Queue ------\n";
	devQueue.getFile().printContent();
	cout << "\n\n";
}

void Driver::execute() {
	processNode* processTemp = NULL;
	for (int i = 0; !waitingQueue.isEmpty() || !multiQueue.isEmpty() || !devQueue.isEmpty(); i++) {
		//cout << "TIME: " << i << endl;

		// Step 01:      Transfer from Waiting to Ready
		if (waitingQueue.isEmpty()) {
			//cout << "The Waiting Queue is Empty\n";
		}
		else {
			if (waitingQueue.peek()->arrivalTime == i) {
				processTemp = waitingQueue.dequeue()->setStatus("Waiting in Ready Queue");
				processTemp->enqueueOrder("Ready Queue");
				//processTemp->status = "Waiting in Ready Queue";
				multiQueue.getReady().enqueue(processTemp, i);
			}

		}
		// Step 02:      Compare the processes of ready queue to find shortest BT process 
		// AND
		// Step 03:      Compare the shortest BT process of waiting queue to high priority process.
		// AND
		// Step 04:      shortest BT process of waiting queue is smaller than send the process to waiting queue and send the process of HP queue to LP queue.

		if (multiQueue.getReady().findShortBurstTime() < multiQueue.getHigh().findShortBurstTime()) {
			processTemp = multiQueue.getHigh().dequeue();

			if (processTemp) {
				processTemp->status = "Waiting in Low Queue";
				processTemp->enqueueOrder("Low Priority Queue");
			}

			multiQueue.getLow().enqueue(processTemp, i);


			processTemp = multiQueue.getReady().dequeue()->setStatus("Executing in High Queue");
			processTemp->enqueueOrder("High Priority Queue");
			//processTemp->status = "Executing in High Queue";
			multiQueue.getHigh().enqueue(processTemp, i);

		}
		// Step 05:		 Tranfer Low Priority Process to High Priority if HighPriority and Ready are Empty
		if (multiQueue.getHigh().isEmpty() && multiQueue.getReady().isEmpty() && (!multiQueue.getLow().isEmpty())) {
			processTemp = multiQueue.getLow().dequeue();
			processTemp->enqueueOrder("High Priority Queue");
			processTemp->status = "Executing in High Queue";
			multiQueue.getHigh().enqueue(processTemp, i);

		}
		// Step 06:		 Execute Process Instructions

		// Executing High Priority Instruction
		if (!multiQueue.getHigh().isEmpty()) {
			if (multiQueue.getHigh().peek()->instruct->isDevice()) {
				processTemp = multiQueue.getHigh().dequeue();
				processTemp->status = "Executing in Device Queue";
				devQueue.addProcess(processTemp, i);

			}
			else {
				if (multiQueue.getHigh().peek()->peekType() == "BREAK") {
					printBreakpoint();
					system("pause");
				}
				instruction* ptr = multiQueue.getHigh().peek()->dequeue();
				multiQueue.getHigh().peek()->burstTime = multiQueue.getHigh().peek()->calculateBurstTime();
				executed.enqueue(ptr);
				multiQueue.getHigh().peek()->enqueueExecuted(ptr);
				if (multiQueue.getHigh().peek()->isEmpty()) {
					processTemp = multiQueue.getHigh().dequeue()->setStatus("Executed");
					Completion.enqueue(processTemp, i);
				}
			}
		}
		// Executing Device Instructions
		devQueue.execute(executed, multiQueue.getLow(), Completion, i);

		waitingQueue.incrementWaitingTime();
		multiQueue.getReady().incrementWaitingTime();
		multiQueue.getLow().incrementWaitingTime();
		system("cls");
		cout << "--------------- Waiting Queue ----------------\n";
		waitingQueue.print();
		cout << "--------------- Ready Queue ----------------\n";
		multiQueue.getReady().print();
		cout << "--------------- High Priority Queue ----------------\n";
		multiQueue.getHigh().print();
		cout << "--------------- Low Priority Queue ----------------\n";
		multiQueue.getLow().print();
		cout << "--------------- DISK Queue ----------------\n";
		devQueue.getDisk().print();
		cout << "--------------- SCREEN Queue ----------------\n";
		devQueue.getScreen().print();
		cout << "--------------- PRINTER Queue ----------------\n";
		devQueue.getPrinter().print();
		cout << "--------------- KEYBOARD Queue ----------------\n";
		devQueue.getKeyboard().print();
		cout << "--------------- FILE Queue ----------------\n";
		devQueue.getFile().print();
		cout << "--------------- EXECUTED INSTRUCTIONS ----------------\n";
		executed.print();
		cout << "--------------- Waiting Queue ----------------\n";
		waitingQueue.print();
		system("pause");

	}
}
