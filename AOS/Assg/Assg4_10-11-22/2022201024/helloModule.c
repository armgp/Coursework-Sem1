#include<linux/module.h>

// The following includes are not initially required, but have been included for Assignment #3 - Sp 2020
#include<linux/sched/signal.h>
#include<linux/pid_namespace.h>
#include<linux/cdev.h>
#include<linux/proc_fs.h>
#include<linux/slab.h>

void count_running_processes(void){
	int count = 0, countRunning = 0, countUninterruptible = 0, countInterruptible = 0;	
	struct task_struct *child;
	for_each_process(child) {
		if(child->state == TASK_RUNNING) countRunning++;
		else if(child->state == TASK_UNINTERRUPTIBLE) countUninterruptible++;
		else if(child->state == TASK_INTERRUPTIBLE) countInterruptible++;
		count++;
	}
	printk(KERN_INFO "No of RUNNING processes = %d\n", countRunning);
	printk(KERN_INFO "No of UNINTERRUPTIBLE processes = %d\n", countUninterruptible);
	printk(KERN_INFO "No of INTERRUPTIBLE processes = %d\n", countInterruptible);
  	printk(KERN_INFO "Total No of processes = %d\n", count);
}

int proc_init (void) {
  	printk(KERN_INFO "helloModule: kernel module initialized\n");
  	count_running_processes();
  	return 0;
}

void proc_cleanup(void) {
  printk(KERN_INFO "helloModule: performing cleanup of module\n");
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);

