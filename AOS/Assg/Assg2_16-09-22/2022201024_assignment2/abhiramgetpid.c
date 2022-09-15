#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/uaccess.h>

/* prints input string alog with added message */
/* syscall number 549 */

SYSCALL_DEFINE0(abhiramgetpid){
        return task_tgid_vnr(current);
}


