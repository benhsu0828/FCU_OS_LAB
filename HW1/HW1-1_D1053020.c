#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
 
// static int num = 248;
static char *practice1 = "D1053020 iecs3D PAO-HUA-HSU";
// module_param(num, int, 0);
module_param(practice1, charp, 0);
 
// 進入 kernal module
static int __init hello_init(void)
{
    printk(KERN_INFO "welcom to %s!\n", practice1);
    // printk(KERN_EMERG "KERN_EMERG %s!\n", practice1);
    // printk(KERN_ALERT "KERN_ALERT %s!\n", practice1);
    // printk(KERN_WARNING "KERN_WARNING %s!\n", practice1);
    // printk(KERN_INFO "KERN_INFO %s!\n", practice1);
    return 0;
}
 
// 離開 kernal module
static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye! %s\n", practice1);
    // printk(KERN_EMERG "KERN_EMERG %s!\n", practice1);
    // printk(KERN_ALERT "KERN_ALERT %s!\n", practice1);
    // printk(KERN_WARNING "KERN_WARNING %s!\n", practice1);
    // printk(KERN_INFO "KERN_INFO %s!\n", practice1);
}
 
// 定義 module 進入, 離開
module_init(hello_init);
module_exit(hello_exit);
 
// moudule 描述
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("PAO-HUA-HSU");