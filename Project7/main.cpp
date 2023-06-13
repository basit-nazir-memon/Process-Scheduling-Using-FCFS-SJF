#include "FeedbackQueue.h"
int main() {
    cout << "-------------------- Process Scheduling Using FCFS AND SJF -----------------------\n";
    Driver driver;
    cout << "The Processess in Waiting Queue Are: \n";
    driver.getWaitingQueue().print();
    driver.execute();
    return 0;
}
