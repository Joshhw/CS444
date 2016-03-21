The first task, for part two of homework one, was to replace the existing code for the ring buffer and utilize the provided queue code.  This was quite straightforward as 
each ring buffer action corresponded to an equivalent queue function.
 
After replacing the ring buffer we needed to allow for the queues to be modified by the interrupt service routine.  We determined that this would be done by utilizing the 
RDI and THRI bits on the line status register.  We implemented what we thought to be functional code but when testing on the VM found that it was only printing the first 
letter of our desired output and then getting hung up.  We attempted to troubleshoot this issue using a variety of methods and at the end of the day determined we would need 
to consult with Professor Cheung.
 
 In class Professor Cheung explained that there was an issue with the way the VM's were handling interrupts and that he had yet to find a way to resolve it.  He said that 
the program should be able to run on the SAPC's.  We had yet to test this, as we were told to work exclusively on the VM's but once run on the SAPC our code worked 
successfully.
 
Once Professor Cheung determined the cause of the issue, the virtualized VM THRI bit needed to be turned off within the ISR, he proposed one method for solving the issue and 
our team sought to find another.  In Professor Cheung's solution he said that we should have a for loop that went over the entire write queue, output everything, and then go 
back into ttywrite.  This went against what we had been taught previously, that interrupt service routines should be as short as possible, so our team developed an 
implementation that took the VM's issue into account but would still not have a loop within the ISR.
 
We tested our code on both the VM and SAPC to ensure that our solution would function properly in both environments.
 
Below is a breakdown on how each part of the program functions:
 
 
ttyread:
Function is passed a number of characters to read.
Function loops until it has read that many characters
Function de-queues items from the read queue which is being filled up by the ISR
 
ttywrite:
Function is passed a number of characters to write out
Function loops until it is able to enqueue all of the characters into the write queue
Function then loops until the queue is empty (Allows code to function normally on the VM and built in delay to allow interrupts to complete)

 
ISR: (Has two main paths of execution)
If there is data to be read
-read in a byte and store it into the read queue
-if echo is enabled store it into the echo queue as well
 
If there is data to send out 
-send data out from the echo queue if there is any 
-send data out from the write queue 
-turn off THRI bit
