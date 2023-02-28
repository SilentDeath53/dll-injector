# dll-injector
A .dll injector. 

ALL RIGHTS RESERVED BY Silent Death_53

To learn the process ID of a running process, you can follow these steps:

1- Open Task Manager on Windows or System Monitor on Linux.
2- Locate the process you want to know the ID of in the list of running processes.
3- In Windows Task Manager, right-click the process and select "Go to details." In Linux System Monitor, click on the process to highlight it, then select "Process" -> "PID Properties."
4- The PID of the selected process should now be displayed in the appropriate column in the task manager or system monitor.

Or use it for Linux:
"ps -eo pid,ppid,cmd,%mem,%cpu --sort=-%mem | head" (useful one)
