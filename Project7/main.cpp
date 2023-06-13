#include "Maze.h"
#include "FeedbackQueue.h"
int main() {
    cout << "-------------------- Question : 01 -----------------------\n";

    Driver driver;
    cout << "The Processess in Waiting Queue Are: \n";
    driver.getWaitingQueue().print();
    driver.execute();

    /*cout << "Multi Level Queue Name: " << d.getName() << endl;
    cout << "\n\n\n\n\nAfter Execution, The Processess in Waiting Queue Are: \n";
    driver.getWaitingQueue().print();*/

    cout << "\n\n\n\n\n\n\n\n\n\n\n\n-------------------- Question : 02 -----------------------\n";

   
	MazeSolver maze;
	maze.solve();
	return 0;
}