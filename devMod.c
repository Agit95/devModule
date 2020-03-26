#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include "devMod.h"
//#include <fcntl.h>
//#include <unistd.h>

#include <linux/slab.h>
#include <asm/uaccess.h>
#define BUFSIZE  1000
 
 
MODULE_DESCRIPTION("Module which uses functions from other module to demonstrate symbol exporting");
MODULE_AUTHOR("Anna Ayvazyan");
MODULE_LICENSE("GPL v2");

static struct proc_dir_entry *ent;
/* 
static ssize_t mywrite(struct file *file, const char __user *ubuf,size_t count, loff_t *ppos) 
{
	printk( KERN_DEBUG "write handler\n");
	return -1;
}
*/

char* devName = "lla345";
const int major = 57;
//int globalVar = 31;
//static int staticVar = 61;

//int exportedVar = 91;
//EXPORT_SYMBOL(exportedVar);

static int irq=20;
module_param(irq,int,0660);
 
static int mode=1;
module_param(mode,int,0660);
 

int exportedFunc(int param);
//{
////	printk(KERN_DEBUG "we are in exportedFuc and the param is %d\n", param);
//	return 3;
//}


//EXPORT_SYMBOL(exportedFunc);



static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) 
{
	printk( KERN_DEBUG "1 write handler\n");
	int num, elem,c,i,m;
	char buf[BUFSIZE];
	printk( KERN_DEBUG "2 write succeed\n");
	if(*ppos > 0 || count > BUFSIZE)
		return -EFAULT;
	
	printk( KERN_DEBUG "3 write succeed\n");
	if(raw_copy_from_user(buf,ubuf,count))
		return -EFAULT;

	printk( KERN_DEBUG "4 write succeed\n");
        num = sscanf(buf,"%d", &elem);

	printk( KERN_DEBUG "5 write succeed\n");
	if(num != 1)
		return -EFAULT;
	
	push(stack, elem);
	
	c = strlen(buf);
	*ppos = c;

	return c;
}

static ssize_t myread(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) 
{
	char buf[BUFSIZE];
	int len=0;
	printk( KERN_DEBUG "1 read handler\n");
	if(*ppos > 0 || count < BUFSIZE)
		return 0;
	printk( KERN_DEBUG "2 read handler\n");
	int* ptrElem = pop(stack);
        if (!ptrElem)
           return -EFAULT;

	printk( KERN_DEBUG "3 read handler  sz%d\n", *ptrElem);
	printk( KERN_DEBUG "3 read handler sz+1%d\n", *ptrElem);
	printk( KERN_DEBUG "3 read handler sz-1%d\n", *ptrElem);
      	len += sprintf(buf,"%d\n", *ptrElem);

	
	printk( KERN_DEBUG "4 read handler\n");

	
	if(raw_copy_to_user(ubuf,buf,len))
		return -EFAULT;
	
	printk( KERN_DEBUG "5 read handler\n");
	*ppos = len;

	return len;
}


/*
static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	printk( KERN_DEBUG "read handler\n");
	return 0;
}
 */

static struct file_operations myops = 
{
	.owner = THIS_MODULE,
	.read = myread,
	.write = mywrite,
};
 
static int simple_init(void)
{
        printk(KERN_INFO "Hello, It is procFileMod init!\n");
 
        int retVal = register_chrdev(major, devName, &myops);
 	
	if (retVal)
	{
           printk(KERN_INFO "Something is wrong %d\n", retVal);
	}

	return 0;
}
 
static void simple_cleanup(void)
{
        printk(KERN_INFO "Bye, It was procFileMod exit!\n");
	proc_remove(ent);
}
 
module_init(simple_init);
module_exit(simple_cleanup)






// source:  https://devarea.com/linux-kernel-development-creating-a-proc-file-and-interfacing-with-user-space/#.XkziL2gza70	
