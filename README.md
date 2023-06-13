# Process Scheduling using FCFS and SJF

This is a C++ program that implements process scheduling algorithms - First-Come, First-Served (FCFS) and Shortest Job First (SJF). The program uses the RapidXML library to read process data from an XML file.

## Program Structure

The program consists of the following files:

1. `FeedbackQueue.cpp`: This file contains the implementation of the FeedbackQueue class, which represents the process scheduling queue.
2. `FeedbackQueue.h`: This header file defines the FeedbackQueue class and its member functions.
3. `XMLFile.xml`: This XML file stores the process data.
4. `file.XML`: This is an example XML file containing sample process data.
5. `main.cpp`: This file contains the `main()` function and provides a user interface to interact with the process scheduling algorithms.
6. `rapidxml.hpp`: This is the RapidXML library header file.

## Compilation

To compile the program, follow these steps:

1. Ensure that you have a C++ compiler installed on your system (e.g., g++ for Linux or MinGW for Windows).
2. Open a terminal or command prompt and navigate to the directory containing the program files.
3. Run the following command to compile the program:

```shell
g++ main.cpp FeedbackQueue.cpp -o process_scheduling -std=c++11
```

This command compiles both `main.cpp` and `FeedbackQueue.cpp` files and generates an executable named `process_scheduling`.

## Usage

To run the program, execute the following command in the terminal or command prompt:

```shell
./process_scheduling
```

The program will read the process data from the `XMLFile.xml` file and display a menu of options for selecting the scheduling algorithm (FCFS or SJF). Follow the on-screen instructions to proceed with the selected algorithm.

## XML Data Format

The program reads the process data from an XML file in the following format:

```xml
<?xml version="1.0" encoding="utf-8"?>
<AssignmentInput>
  <QueueSet>
    <SetName>Background Queues</SetName>
    <UpgradePolicy>time</UpgradePolicy>
    <DowngradePolicy> time </DowngradePolicy>
    <QAllocPolicy>2</QAllocPolicy>
    <Queue number="1" >
      <SchedulingPolicy> FCFS </SchedulingPolicy>
    </Queue>
    <Queue number="2">
      <SchedulingPolicy> SJF</SchedulingPolicy>
    </Queue>
  </QueueSet>
  <Configuration>
    <Processes number="2" >
      <Process ID="1">
        <ArrivalTime>0</ArrivalTime>
        <Type>IO_Bound</Type>
        <code>
          WRITESCREEN
          READKEYBOARD
          COMPUTE
          READMEMORY
          COMPUTE
          READKEYBOARD
          WRITEMEMORY
          WRITEPRINTER
        </code>
      </Process>
      <Process ID="2">
        <ArrivalTime>1</ArrivalTime>
        <Type>CPU_Bound</Type>
        <code>
          WRITEMEMORY
          READMEMORY
          COMPUTE
        </code>
      </Process>
     </Processes>
  </Configuration>
</AssignmentInput>
```

You can modify the `XMLFile.xml` file or create your own XML file with process data to test different scenarios.

## Contributing

If you would like to contribute to this program, please follow these steps:

1. Fork the repository.
2. Create a new branch for your changes.
3. Make the necessary modifications and improvements.
4. Test your changes thoroughly.
5. Commit and push your changes to your forked repository.
6. Open a pull request, describing the changes you have made.

Your contributions are greatly appreciated!

## License

This program is licensed under the [MIT License](LICENSE). Feel free to modify and distribute it according to the terms of the license.

## Contact

If you have any questions or suggestions regarding this program, please contact basitnazir585@gmail.com.

Thank you for using our Process Scheduling program!
