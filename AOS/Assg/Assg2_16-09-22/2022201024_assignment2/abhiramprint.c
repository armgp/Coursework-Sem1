#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/uaccess.h>

/* prints input string alog with added message */
/* syscall number 549 */

SYSCALL_DEFINE1(abhiramprint, char __user *, source){
        char buf[256];     

	if( copy_from_user(buf, source, sizeof(buf)) ){
		return -EFAULT;
	}
    
        printk("If the 2nd Question is working -> %s\n", buf);
    
        return 0;
}
