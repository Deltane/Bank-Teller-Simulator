Operating Systems BankTellerSimulator Report
Serena McPherson 19765644	

How to install via GitHub:
git clone https://github.com/Deltane/Bank-Teller-Simulator
cd BankTellerSimulator
make

Usage:
If given permission denied error run the command below:
chmod +x banksimulator

./banksimulator queue_length customer_interval withdrawal_duration deposit_duration information_duration

Enter five command line arguments in the form of positive integers where the above represent their values. For example:
./banksimulator 20 3 6 2 5

Function Breakdowns

Main Function:
My main function creates a Queue struct named c_queue using the createQueue function, initialises a TellerTotals struct named totals using the initTellerTotals function, and opens a log file named "r_log" using openLogFile function.

The function then creates a pthread_t variable named customer_th and an array of 4 pthread_t variables named teller_th, and initialises a loop variable i.

The function prints a message indicating that it is starting the threads, creates a new thread for the customer function using pthread_create, and creates 4 new threads for the teller function using a for loop and pthread_create.

The function then waits for the threads to finish executing using pthread_join, and prints a message indicating that the threads have been joined.

Finally, the function frees the memory allocated for c_queue using freeQueue function, totals using freeTellerTotals function, and the logfile using closeLogFile function, and returns 0 to indicate successful completion of the program.


Title: Function Analysis: customer() - Adding Customers to Queue

Introduction:
The customer() function is a thread routine responsible for adding customers to a queue. This analysis aims to provide a concise breakdown of its functionality.

Function Overview:
The customer() function takes a void pointer arg as a parameter, representing a pointer to a Parameters structure. It reads customer information from a file, simulates customer arrival with sleep intervals, and adds customers to the queue. The function uses synchronization mechanisms such as locks and condition variables to ensure thread safety.

Detailed Analysis:

Initialization:

Cast the arg pointer to Parameters* and assign it to the parameters variable.
Retrieve the queue pointer from the parameters structure.
Initialize variables, including customer, push_customer, linebreak, and ltime.
File Handling:

Open a customer file for reading.
If the file pointer is NULL, print an error message.
If the file is successfully opened, continue.
Customer Processing Loop:

Read lines from the file until the end is reached.
Create a new customer_t structure using the create_customer() function.
Read integer n and character type from the file and assign them to the corresponding fields of the customer structure.
Simulate customer arrival by sleeping for a specified time interval (parameters->tc).
Queue Management:

Acquire the queue's lock using pthread_mutex_lock.
Enter a while loop to attempt pushing the customer into the queue.
Check if the queue has room for one more customer.
If there is room, insert the customer at the start of the queue.
Assign the current time to the customer's arrival field.
Signal successful push by setting push_customer to TRUE and logging arrival time to a logfile.
If the queue is full, wait on the not_full condition variable until space becomes available.
Unlock the queue's mutex using pthread_mutex_unlock.
Signaling and Termination:

Signal the new_customer condition variable to unblock a teller thread and serve the customer.
Repeat the loop until all customers are processed.
Lock the queue's mutex.
Set the incoming flag of the queue to FALSE, indicating no more customers will arrive.
Unlock the queue's mutex.
Cleanup:

Close the file.
Helper Functions:

create_customer(): Allocates memory for a new customer_t structure, initializes its fields, and returns a pointer to the created structure.
print_customer(): Prints the customer's number (n) and type (type) received as a void pointer.
Conclusion:
The customer() function effectively processes a customer file, simulates customer arrivals, and adds customers to a queue using synchronization mechanisms. It ensures thread safety and provides essential functionality for a customer queuing system.

Synchronisation through implementation of locks/mutexs

