#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/hashtable.h>

static DEFINE_HASHTABLE(task_table, 16);

static int hello_init(void)
{
  struct task_struct *pp;

  printk(KERN_INFO "Hello World!!!\n");

  // 初始化 hash table
  hash_init(task_table);

  printk(KERN_ALERT "名稱\t\t\tpid\tstate\tprio\tpolicy\n");
  // 遍歷所有 process
  // for_each_process(pp)
  // {
  //     if (pp->mm == NULL)
  //     {
  //         // 創建一個資料結構來儲存您需要的信息
  //         struct task_info
  //         {
  //             char comm[TASK_COMM_LEN];
  //             int state;
  //             pid_t pid;
  //             int prio;
  //             int policy;
  //         };

  //         struct task_info *info = kmalloc(sizeof(struct task_info), GFP_KERNEL);
  //         if (!info)
  //         {
  //             printk(KERN_ERR "Failed to allocate memory\n");
  //             return -ENOMEM;
  //         }

  //         // Fill in the data structure
  //         strncpy(info->comm, pp->comm, TASK_COMM_LEN);
  //         info->state = pp->state;
  //         info->pid = pp->pid;
  //         info->prio = pp->prio;
  //         info->policy = pp->policy;

  //         // Add the data structure to the hash table
  //        // hash_add(task_table, &info->node, info->pid);
  //     }
  // }
  for_each_process(pp)
  {
    if (pp->prio < 70)
    {
      printk(KERN_ALERT "%s\t\t%d\t%d\t%d\t%d\n", pp->comm, pp->pid, pp->state, pp->prio, pp->policy);
    }
    else
    {
      printk(KERN_INFO "%s\t\t%d\t%d\t%d\t%d\n", pp->comm, pp->pid, pp->state, pp->prio, pp->policy);
    }
  }
  return 0;
}

static void hello_exit(void)
{
  struct task_info *info;

  printk(KERN_INFO "Goodbye World!!!\n");

  // // Traverse the hash table and print the information
  // hash_for_each(task_table, i, info, node)
  // {
  //     printk(KERN_ALERT "\t\t%d\t%d\t%d\t%d\n", info->pid,info->state ,info->prio,info->policy);
  // }

  // // Delete the hash table
  // hash_for_each(task_table, i, info, node)
  // {
  //     hash_del(&info->node);
  //     kfree(info);
  // }
}

// //sample code

// // 進入 kernel module
// static int hello_init(void)
// {
// // 宣告兩個 task_struct 指標
// struct task_struct *pp;
// struct task_struct *p;

// // 輸出 Hello, world!
// printk("Hello, world!\n");

// // 顯示上方欄位
// printk(KERN_ALERT"\t\tpid\t\t\tpolicy\n");

// // 遍歷所有 process
// for_each_process(pp) {
//   // 如果該 process 的 mm 為 NULL，則跳過
//   if (pp->mm == NULL) {
//     continue;
//   }

//   // 輸出 process 的名稱和 pid
//   printk(KERN_ALERT "%s\t%d\t\t%d\n", pp->comm, pp->pid, pp->policy);
// }

// // task_struct 的 ptd 欄位是 process 的 td 指標
// for_each_process(p) {
//   printk(KERN_INFO "this is %d", p->pid);
// }

//   return 0;
// }

// // 離開 kernel module
// static void hello_exit(void)
// {
//   printk(KERN_INFO "Good-bye!\n");
// }

// 定義 module 進入、離開
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
