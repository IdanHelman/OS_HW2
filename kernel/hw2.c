#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/list.h>

asmlinkage long sys_hello(void) {
    printk("Hello, World!\n");
    return 0;
}

asmlinkage long sys_set_weight(int weight){
    if(weight < 0){
        return -EINVAL;
    }
    current->weight = weight;
    return 0;
}

asmlinkage long sys_get_weight(void){
    return current->weight;
}

asmlinkage long sys_get_path_sum(pid_t target){
    struct task_struct *task;
    long sum = current->weight;
    task = pid_task(find_vpid(target), PIDTYPE_PID);
    if (task == NULL) {
        return -ECHILD;
    }

    while (task->pid != 0 && task->pid != current->pid) {
        sum += task->weight;
        task = task->parent;
    }

    if (task->pid == current->pid) {
        return sum;
    } else {
        return -ECHILD;
    }
}

asmlinkage long sys_get_heaviest_sibling(void){
    long maxWeight = 0;
    long heaviest = current->pid;
    struct task_struct *temp;

    if(list_empty(&current->sibling)){
        return current->pid;
    }

    list_for_each_entry(temp, &current->sibling, sibling) {
        if(temp->weight > maxWeight){
            maxWeight = temp->weight;
            heaviest = temp->pid;
        }
        else if(temp->weight == maxWeight){
            heaviest = heaviest < temp->pid ? heaviest : temp->pid;
        }
    }
    return heaviest;
}