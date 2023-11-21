This code simulates cars crossing a bridge, with a limit of 5 cars allowed on the bridge simultaneously. The program 
uses pthreads for concurrent execution and semaphores for synchronization.

Here's an explanation of the code logic and how it avoids concurrency bugs:

# Initialization:
        The program starts by taking the number of left and right cars as input.
        Two semaphores, car_semaphore (to control the number of cars on the bridge) and allSameCars (currently commented out), are initialized.

# Passing Function:
        The passing function represents a car crossing the bridge.
        It checks the direction and the number of cars on the bridge.
        If the bridge is full, it waits until it's clear and then updates the current direction.

# Left and Right Functions:
        left and right functions represent left and right cars attempting to cross the bridge.
        They call the passing function with their respective direction.

# Minimum and Maximum Functions:
        The min and max functions return the minimum and maximum of two numbers, respectively.

# Main Function:
        The main function takes the number of left and right cars as input and initializes semaphores.
        It creates threads for left and right cars based on the minimum and maximum counts.
        The program ensures that if one side has more cars, additional threads are created only for that side.
        Finally, it waits for all threads to finish.

# Concurrency Bug Avoidance:
        Semaphores (car_semaphore and allSameCars) are used to control access to the critical section, ensuring that only a limited number of cars can be on the bridge simultaneously.
        The curr_direction variable helps avoid unnecessary context switches when cars of the same direction are crossing the bridge.
        The program uses synchronization mechanisms to wait until the bridge is clear if the direction changes.
        Proper flushing of output buffers (fflush(stdout)) is added to ensure accurate printing in a multithreaded environment.

In summary, the code uses semaphores and direction checks to control the number of cars on the bridge concurrently, preventing race conditions and ensuring a smooth simulation of cars crossing the bridge.
