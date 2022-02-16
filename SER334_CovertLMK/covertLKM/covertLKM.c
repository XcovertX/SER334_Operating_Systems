/**
 * covertLKM.c
 *
 * A kernal module that describes all processes above a given PID number.
 * Default PID is 0.
 *
 * To compile, run makefile by entering "make"
 * To insert, enter "sudo insmod ./covertLKM.o inp_pid=<desired starting pid>"
 * To print, enter "dmesg"
 * To remove, enter "sudo rmmod covertLKM"
 *
 * Completion time: 5h
 *
 * @author James Covert
 * @version 1.0
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/list.h>

// Globals
static int inp_pid = 0;
struct task_struct* task;
struct list_head* head;

/* Macros for getting information from the user */
module_param(inp_pid, int, 0);
MODULE_PARM_DESC(inp_pid, "The strating PID in the list");

void print_processes(void) {

    // Loop over scheduled task
    for_each_process(task) {
    
        if(task->pid >= inp_pid) {
        
            // Process Information
            printk("=======================================================\n");
            printk(KERN_INFO "PROCESS NAME: %s\n", task->comm);
            printk(KERN_INFO "PID: %d\n", task->pid);
            printk(KERN_INFO "STATE: %ld\n", task->state);
            printk(KERN_INFO "PRIORITY: %d\n", task->prio);
            printk(KERN_INFO "STATIC PRIORITY: %d\n", task->static_prio);
            printk(KERN_INFO "NORMAL PRIORITY: %d\n", task->normal_prio);
            
            // Parent task check
            if(task->parent != NULL) {
                // Parent Task
                struct task_struct* parent = task->parent;
                
              	 // Parent Information
             	 printk(KERN_INFO "PARENT ------------------------------------------------------");
           	 printk(KERN_INFO "PROCESS NAME: %s\n", parent->comm);
           	 printk(KERN_INFO "PID: %d\n", parent->pid);
            	 printk(KERN_INFO "STATE: %ld\n", parent->state);
           	 printk(KERN_INFO "PRIORITY: %d\n", parent->prio);
           	 printk(KERN_INFO "STATIC PRIORITY: %d\n", parent->static_prio);
           	 printk(KERN_INFO "NORMAL PRIORITY: %d\n", parent->normal_prio);
            }

            // Loop over all children tasks
            list_for_each(head, &task->children) {

                // Point to next child in the list
                struct task_struct* child = list_entry(head, struct task_struct, sibling);
                
                // Child Information
                printk(KERN_INFO "CHILD -------------------------------------------------------");
                printk(KERN_INFO "PROCESS NAME: %s\n", child->comm);
                printk(KERN_INFO "PID: %d\n", child->pid);
                printk(KERN_INFO "STATE: %ld\n", child->state);
                printk(KERN_INFO "PRIORITY: %d\n", child->prio);
                printk(KERN_INFO "STATIC PRIORITY: %d\n", child->static_prio);
                printk(KERN_INFO "NORMAL PRIORITY: %d\n", child->normal_prio);
            }
        }
    }
}

/* This function is called when the module is loaded. */
int covertLKM_init(void) {

    printk(KERN_INFO "CovertLKM loaded\n");
    printk(KERN_INFO "Starting PID: %d\n", inp_pid);
    print_processes();
    return 0;
}

/* This function is called when the module is removed. */
void covertLKM_exit(void) {

    printk(KERN_INFO "Removing CovertLKM\n");
}

/* Macros for registering module entry and exit points. */
module_init( covertLKM_init );
module_exit( covertLKM_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Covert Loadable Kernel Module");
MODULE_AUTHOR("James Covert");
