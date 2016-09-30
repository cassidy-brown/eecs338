####Cassidy Brown - cmb195
#### Homework 3

###Problem 1: shell
Basic implementation  


###Problem 2: compute
Used loop to fork a new process for each computation to be made. The children called a helper method to make the computation, while the parent itself did not do the computation.

```
cassidy@ubuntu:~/os/hw3$ ./compute
Number of processes? 1
Process 1: 14.904915 seconds. x = 2000000000.

cassidy@ubuntu:~/os/hw3$ ./compute
Number of processes? 2
Process 2: 14.801942 seconds. x = 1000000000.
Process 1: 14.815269 seconds. x = 1000000000.

cassidy@ubuntu:~/os/hw3$ ./compute
Number of processes? 3
Process 2: 13.229601 seconds. x = 666666666.
Process 3: 13.240655 seconds. x = 666666666.
Process 1: 13.290159 seconds. x = 666666666.

cassidy@ubuntu:~/os/hw3$ ./compute
Number of processes? 4
Process 3: 14.133034 seconds. x = 500000000.
Process 4: 14.240959 seconds. x = 500000000.
Process 1: 14.262185 seconds. x = 500000000.
Process 2: 14.311838 seconds. x = 500000000.
```

The amount of time each process took really didn't change, which may be related to the fact that I used a virtual machine with only one core. Without multiple cores, the CPU has to multitask and the time it takes to execute the multiple processes in rotating increments is about the same as if all the computation had been in one process.


###Problem 3: memory
Basic implementation  
Prints "Sharing is caring"