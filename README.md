# BankTellerSimulator Report For Operating Systems COMP2006
Serena McPherson 19765644	

# How to install via GitHub:
git clone https://github.com/Deltane/Bank-Teller-Simulator
cd BankTellerSimulator
make

# Usage:
* ./banksimulator queue_length customer_interval withdrawal_duration deposit_duration information_duration
* Enter five command line arguments in the form of positive integers where the above represent their values. 
* For example:./cq 8 3 6 2 5

# Function Overviews
Main Function:
My main function creates a Queue struct named c_queue using the createQueue function, initialises a TellerTotals struct named totals using the initTellerTotals function, and opens a log file named "r_log" using openLogFile function.

The function then creates a pthread_t variable named customer_th and an array of 4 pthread_t variables named teller_th, and initialises a loop variable i.

The function prints a message indicating that it is starting the threads, creates a new thread for the customer function using pthread_create, and creates 4 new threads for the teller function using a for loop and pthread_create.

The function then waits for the threads to finish executing using pthread_join, and prints a message indicating that the threads have been joined.

Finally, the function frees the memory allocated for c_queue using freeQueue function, totals using freeTellerTotals function, and the logfile using closeLogFile function, and returns 0 to indicate successful completion of the program.

The Customer function takes a void pointer arg as a parameter, representing a pointer to a Parameters structure. It reads customer information from a file, simulates customer arrival with sleep intervals, and adds customers to the queue. The function uses synchronization mechanisms such as locks and condition variables to ensure thread safety.

    Helper Functions:

* create_customer(): Allocates memory for a new customer_t structure, initializes its fields, and returns a pointer to the created structure.
* print_customer(): Prints the customer's number (n) and type (type) received as a void pointer.
Conclusion:
The customer() function effectively processes a customer file, simulates customer arrivals, and adds customers to a queue using synchronization mechanisms. It ensures thread safety and provides essential functionality for a customer queuing system.

Synchronisation through implementation of locks/mutexs


# Customer Threads
The `Customer` function begins by casting the input argument, which is initially of type `void*`, to a `Parameters*` pointer. It also declares and initializes several variables for later use. Then, it opens the file "c_file.txt" to read customer information.

Inside a while loop, the function reads each line of the file. For each line, it creates a customer struct and reads the customer's number and type using `fscanf`. Afterward, it sleeps for a specified duration (`params->tc`) using `sleep`.

In regards to synchronisation methods used in the simulator, the function first acquires a lock on the queue using `pthread_mutex_lock(&queue->lock)` to push a customer into the queue. If the queue is full, it blocks and waits for the queue to become not full using `pthread_cond_wait(&queue->not_full, &queue->lock)`. This condition variable is signaled by teller threads when they successfully remove a customer from the queue. If there is room in the queue, the customer is inserted at the back of the queue using the `insertStart` function.

The function then sets the customer's arrival time using `localtime_r` and logs the arrival of the customer to the log file using the `LOG` variadic macro.

After pushing the customer to the queue, the function unlocks the queue using `pthread_mutex_unlock(&queue->lock)` and signals the `new_customer` condition variable to unblock a waiting teller thread. It then proceeds to the next iteration of the while loop to read the next customer, unless it has reached the end of the file. In that case, it sets the `queue->incoming` boolean to `FALSE`, closes the file, and exits the function.

Note: The code references additional functions and macros (e.g., `insertStart`, `LOG`) defined elsewhere. The appendix references provide further information on those specific parts.