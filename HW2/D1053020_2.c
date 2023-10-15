#include <linux/string.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

struct student
{
   char *SID;
   char *name;
   char *class;
   char *gender;
   char *birthday;
   struct list_head list;
};
static LIST_HEAD(student_list);

void insertData(struct student *ptr, const char *SID, const char *name, const char *class, const char *gender, const char *birthday)
{
   // ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
   //  if (ptr == NULL) {
   //   return NULL;
   //  }

   ptr->SID = kmalloc(strlen(SID), GFP_KERNEL);
   strcpy(ptr->SID, SID);

   ptr->name = kmalloc(strlen(name), GFP_KERNEL);
   strcpy(ptr->name, name);

   ptr->class = kmalloc(strlen(class), GFP_KERNEL);
   strcpy(ptr->class, class);

   ptr->gender = kmalloc(strlen(gender), GFP_KERNEL);
   strcpy(ptr->gender, gender);

   ptr->birthday = kmalloc(strlen(birthday), GFP_KERNEL);
   strcpy(ptr->birthday, birthday);
}

int simple_init(void)
{
   struct student *ptr;
   struct student *person, *person2, *person3, *person4, *person5;
   struct student *firstp, *lastp;

	firstp = kmalloc(sizeof(*firstp), GFP_KERNEL);
	lastp = kmalloc(sizeof(*lastp), GFP_KERNEL);
	insertData(firstp, "D1051111", "Leonardo", "資訊三丁", "男", "92/10/10");
   	insertData(lastp, "D1051122", "ben", "資訊三丁", "女", "92/05/04");
	INIT_LIST_HEAD(&firstp->list);
	list_add(&firstp->list, &student_list);
	list_add_tail(&lastp->list, &student_list);

   person = kmalloc(sizeof(*person), GFP_KERNEL);
   person2 = kmalloc(sizeof(*person2), GFP_KERNEL);
   person3 = kmalloc(sizeof(*person3), GFP_KERNEL);
   person4 = kmalloc(sizeof(*person4), GFP_KERNEL);
   person5 = kmalloc(sizeof(*person5), GFP_KERNEL);

   insertData(person, "D1053020", "PAO-HUA-HSU", "資訊三丁", "男", "92/08/28");
   insertData(person2, "D1053021", "PAO-HUA-HSU1", "資訊三丁", "女", "92/08/29");
   insertData(person3, "D1053022", "PAO-HUA-HSU2", "資訊三丁", "男", "92/08/30");
   insertData(person4, "D1053023", "PAO-HUA-HSU3", "資訊三丁", "女", "92/08/31");
   insertData(person5, "D1053024", "PAO-HUA-HSU4", "資訊三丁", "男", "92/09/01");

   list_add_tail(&person->list, &student_list);
   list_add_tail(&person2->list, &student_list);
   list_add_tail(&person3->list, &student_list);
   list_add_tail(&person4->list, &student_list);
   list_add_tail(&person5->list, &student_list);

   printk(KERN_INFO "loading moudle\n");
   list_for_each_entry(ptr, &student_list, list)
   {
      printk(KERN_ALERT "SID: %s, Name: %s, Class: %s, Gender: %s, Birthday: %s\n", ptr->SID, ptr->name, ptr->class, ptr->gender, ptr->birthday);
   }
   return 0;
}

void simple_exit(void)
{
   struct student *ptr, *next;
   list_for_each_entry_safe(ptr, next, &student_list, list)
   {
      list_del(&ptr->list);
      kfree(ptr);
   }
   printk(KERN_INFO "Removing module\n");
}

module_init(simple_init);
module_exit(simple_exit);