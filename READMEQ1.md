# Q1 Readme

# Why Deadlocks Can Happen:

Picture five friends sitting at a round table, each with a fork on their left and right. To eat, they need both forks. If everyone tries to pick up their left fork and then waits for the right one, it's trouble. They might end up stuck, each holding one fork and waiting forever for the person on their right to hand over the other fork. This creates a loop of waiting, and no one gets to eat.

# How the Solution Helps Avoid Deadlock:

To avoid this mess, the solution lets philosophers pick up both forks, but with a twist. Before they start eating, they grab a lock associated with the bowl they're using. This ensures they can't get stuck holding one fork and waiting forever for the other.

There's also a check to see if it's okay to start eating. It looks at whether the neighbors are eating or not. This check ensures that a philosopher only eats when the folks on the left and right aren't busy with their food.

When a philosopher finishes eating, they put down both forks and let their neighbors know they're done. This helps to keep things moving and ensures that everyone gets a fair shot at eating.

# How Fair Is the Solution:

The solution aims to be fair, meaning all philosophers should eventually get a chance to eat. The way the philosophers are set up in a circle guarantees that everyone has neighbors on both sides. The special locks for each bowl make sure a philosopher doesn't hog a fork forever.

But, the actual fairness can depend on how quickly philosophers share the forks. If one philosopher takes a long time to finish, it might delay others. Also, the system's timing and how the computer handles these philosophers can affect fairness. Adjusting how long philosophers think and eat can also change how often they get to eat, making things more or less fair.
