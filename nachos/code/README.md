###GetPA
 In exception.cc, 

 * Read virtual memory address from register 4
 * Implement getPhysAddr in exception.cc, modelling it on the Translate function in machine/translate.cc
 * Translate it to physical address using our getPhysAddr function and put it in register 2 
 * Advance program counters

### GetReg
 In exception.cc, 

 * Read register number from register 4
 * Read contents of that register and wrote to register 2
 * Advance program counters

### GetPID and GetPPID

 * Create a static variable prpid which is a member of the NachOSThread class, initialize it to -1 in Initialize()
 * Increment prpid and assign to pid in constructor of NachOSThread
 * Create various get and set functions in NachOSThread class
 * Call getPID() and getPPID() functions respectively and put the return value in register 2.
 * Advance program counters

### GetTime
 In exception.cc, 

 * Read ticks from TotalTicks variable of stats object and put it in register 2
 * Increase program counters

### Sleep

 * Create a new SleepThreadList in scheduler.h
 * Create threadIsReadyToSleep function which does a sortedInsert on SleepThreadList
 * In timer interrupt handler, keep removing threads, whose time to wake up has been crossed by looking at that field in SleepThreadList, and call ThreadIsReadyToRun for it
 * Get the time to sleep from register 4 and call function threadIsReadyToSleep

### Yield

 * Advance program counters
 * Call YieldCPU()

### Fork
 
 * Set ppid and parentThreadPointer (will be used later in join). They are members of NachOSThread class.
 * Create parentPID array in machine, which stores ppid of each process, indexed by PID. To determine PID of parent when child exits
 * Create a list childList storing pointers of child processes in NachOSThread class
 * Put value 0 in register 2 (return value for child)
 * Advance program counters
 * New ProcessAddrSpace() constructor for the new thread
   * Create new constructor for address space in addrspace.cc
   * Copy everything from parent to child, ensuring unused physical page for children
 * Save user state to new thread
 * Create numRunningProcesses in machine.h and increment it in ProcessAddrSpace() constructors
 * Call threadfork and pass the function pointers of function func() (containing everything after SWITCH)
 * Write new thread PID in register 2

### Join

 * Read child pid from register 4
 * Advance program counters
 * Return -1 if current process is not parent of the child pid by checking parentPID array
 * Return exit code stored in killStatus array (defined in machine.h, initialized to -1500 in constructor, indexed by PID) if child has exited  (ie killStatus is not 1500)
 * If child is alive, in callJoin array (defined in machine.h, initialized to False in constructor, indexed by PID), change callJoin[PID] to true
 * Swich off interrupt, put thread to sleep, switch on interrupt
 * Write exit code in register 2

### Exec 
 * Create a char array and read variable name from register 4, one character at a time
 * Delete childList of currentThread, and contents
 * Call startUserProcess(executable) in exception.cc
 * In ProcessAddrSpace(executable) constructor
   * Ensure unused pages are assigned to new process using validPage array (define in machine.h)
   * For code and initData, copy to a temporary array temp, and then copy to the correct physical address corresponding to respective virtAddr (get this using GetPhysAddr function)

### Exit
 * Get exit code from register 4
 * Set killStatus[PID] to exit code
 * Delete childList and for every child, update parentPID to -1
 * Decrement number of running processes
 * If callJoin[parent PID] is TRUE, switch off interrupts, put parent thread in ready queue, switch on interrupt and make callJoin[parent PID] false
 * If this is the last running process delte thread and cleanup, otherwise call FinishThread()

### NumInstr 
 * Create a numInstr private variable in Thread class and get and increase methods for it
 * In OneInstruction, increase the thread's numInstr
 * Put numInstr in register 2
