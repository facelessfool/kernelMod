#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#define  DEVICE_NAME "process_list"
MODULE_LICENSE("Dual BSD/GPL");

static int my_open(struct inode *, struct file *);

static ssize_t dev_read(struct file* filep, char* buffer, size_t len, loff_t* offset) ;
struct task_struct *current_p,*next_p=NULL;
static char* output;
int buffer_size = 256;
int process_traverse = 0;


#define BUFFER_LEN 100

static struct file_operations fops =
{
           .owner = THIS_MODULE,
           .read = dev_read,
           .open = my_open
};



static char* getstate(int state){
        switch(state){
        case TASK_RUNNING:
        {
                return "TASK_RUNNING";
        }
        case TASK_INTERRUPTIBLE:
        {
            return "TASK_INTERRUPTIBLE";
        }
        case TASK_UNINTERRUPTIBLE:
        {
            return "TASK_UNINTERRUPTIBLE";
        }
        case TASK_STOPPED:
        {
            return "TASK_STOPPED";
        }
        case TASK_TRACED:{
            return "TASK_TRACED";
        }
        case TASK_DEAD:{
            return "TASK_DEAD";
        }
        case TASK_WAKEKILL:{
            return "TASK_WAKEKILL";
        }
        }
    return "IDLE";
}


struct task_struct *task;

static struct miscdevice my_misc_device =
{
       .minor = MISC_DYNAMIC_MINOR,
       .name = DEVICE_NAME ,
       .fops = &fops
};

struct display_data *displays;
static int i = 0;

int __init init_module()
{
        printk(KERN_ALERT "init module from assignment 2!");
        int ret;
        output = kmalloc(buffer_size, GFP_KERNEL);

        ret = misc_register(&my_misc_device);
        if(ret)
        {
          printk("\nRegistering the character device failed with error number: %d", ret);
          return ret;
        }
        printk(KERN_ALERT "device registered!");
        return 0;
}

static int my_open(struct inode *inode, struct file *file)
{
        printk(KERN_ALERT "Character opened!");
        return 0;
}

static ssize_t dev_read(struct file* filep, char* buffer, size_t len, loff_t* offset) {
    unsigned int cpu;
    int status = 0;
    int length=0;
    struct task_struct *p;
    char *State;    

    if(process_traverse==0)
    {
        p = &init_task;
    }
    else
    {
        p = current_p;
    }
    printk("Kajal fingers crossed\n");
    State = getstate(p->state);
    printk("p");
    cpu = task_cpu(p);
    //Process List Over
    if(process_traverse == -1)
    {
        return -1;
    }
    //Iterate over each task 
    length = sprintf(output,"PID=%d \t PPID=%d \t CPU=%d \t STATE=%s"
,p->pid,p->real_parent->pid,cpu,State);
    pr_info("Output buffer details %s \n",output);
    status = copy_to_user(buffer,output,buffer_size);
    if(status !=0)
    {
        pr_err("Failed to copy to user \n");
        return -EINVAL;
    }
    
    process_traverse++;
    current_p = next_task(p);
    if (current_p == &init_task)
    {
        process_traverse =-1;
        next_p = &init_task;
    }
    return buffer_size;
        
}


void __exit cleanup_module()
{
        misc_deregister(&my_misc_device);
        printk(KERN_ALERT "Assignment 2 Done!\n");
 }
