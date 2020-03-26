#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
//#include <fcntl.h>
//#include <unistd.h>

#include <linux/slab.h>
#include <asm/uaccess.h>
#define MAX 1024


int  stack[MAX];
int stsize = 0;



bool push(int* stack, int elem)
{
   bool succeed = false;
   if( stsize < MAX)
   {
	stack[stsize] = elem;
        ++stsize;
        succeed = true;	
   }	   
   return succeed;
}


int* pop(int* stack)
{
   int *elem = NULL;
   if (stsize > 0)
   {
     elem = &stack[--stsize];
   }
   return elem;
}


