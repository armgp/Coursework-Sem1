#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/uaccess.h>

asmlinkage long abhiramprocess(void){
	printk ("Current Process Id => %d\n", current->pid);
	printk ("Parent Process Id => %d\n", current->parent->pid);
	return 0;
}
