#include "types.h"
#include "defs.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"

int getgpid(void){

	struct proc* current_proc = myproc();
	struct proc* parent_proc= current_proc->parent;
	struct proc* grandparent_proc= parent_proc->parent;

	int gpid = grandparent_proc->pid;
	return gpid;

}

//wrapper function?

int sys_getgpid(void){
	return getgpid();
}






