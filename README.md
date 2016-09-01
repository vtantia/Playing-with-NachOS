GetReg - In exception.cc, read register number from register 4, read contents of that register and wrote to register 2. Change program counters.

GetPID - In order to assign pid, we create a static variable prpid which is a member of the NachOSThread class, initialize it to -1 in Initialize(). When a new thread is created, the prpid is incremented and assigned to pid. We have defined a getPID function in NachOSThread class. In exception.cc, we get the pid number using this function, place it in register 2 to return and then change program counters.

GetPA - In exception.cc, we read virtual memory address from register 4, translate it to physical address using our getPhysAddr function, put it in register 2, and change program counters. We have implemented getPhysAddr in exception.cc, modelling it on the Translate function in machine/translate.cc. 
