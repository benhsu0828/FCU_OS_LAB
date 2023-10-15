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
	char *gender;				//0女1男
	char *birthday;
	struct list_head list;
};
static LIST_HEAD(student_list);

void insertData(student *ptr,char *SID, char *name, char *class, char *gender, char *birthday){
	ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
	if (ptr == NULL) {
		return NULL;
	}
	student->SID = strdup(SID);
	student->name = strdup(name);
	student->class = strdup(class);
	student->gender = strdup(gender);
	student->birthday = strdup(birthday);
}

int simple_init(void)
{
	struct student *ptr;
	struct student *person, *person2, *person3, *person4, *person5;

	insertData(person, "D1053020", "PAO-HUA-HSU", "資訊三丁", "男", "92/08/28");
	insertData(person2, "D1053021", "PAO-HUA-HSU1", "資訊三丁", "女", "92/08/29");
	insertData(person3, "D1053022", "PAO-HUA-HSU2", "資訊三丁", "男", "92/08/30");
	insertData(person4, "D1053023", "PAO-HUA-HSU3", "資訊三丁", "女", "92/08/31");
	insertData(person5, "D1053024", "PAO-HUA-HSU4", "資訊三丁", "男", "92/09/01");

	// person = kmalloc(sizeof(*person), GFP_KERNEL);
	// person->SID = 22;
	// person->month = 7;
	// person->year = 2018;
	// person->name = kmalloc(sizeof("Jane"), GFP_KERNEL);
	// strcpy(person->name, "Jane");

	// person2 = kmalloc(sizeof(*person2), GFP_KERNEL);
	// person2->day = 4;
	// person2->month = 3;
	// person2->year = 2018;
	// person2->name = kmalloc(sizeof("Michael"), GFP_KERNEL);
	// strcpy(person2->name, "Michael");

	// person3 = kmalloc(sizeof(*person3), GFP_KERNEL);
	// person3->day = 4;
	// person3->month = 3;
	// person3->year = 2018;
	// person3->name = kmalloc(sizeof("Lacey"), GFP_KERNEL);
	// strcpy(person3->name, "Lacey");

	// person4 = kmalloc(sizeof(*person2), GFP_KERNEL);
	// person4->day = 4;
	// person4->month = 3;
	// person4->year = 2018;
	// person4->name = kmalloc(sizeof("Michael"), GFP_KERNEL);
	// strcpy(person4->name, "Michael");

	// person5 = kmalloc(sizeof(*person3), GFP_KERNEL);
	// person5->day = 4;
	// person5->month = 3;
	// person5->year = 2018;
	// person5->name = kmalloc(sizeof("Lacey"), GFP_KERNEL);
	// strcpy(person5->name, "Lacey");

	list_add(&person->list, &student_list);
	list_add(&person2->list, &student_list);
	list_add(&person3->list, &student_list);
	list_add(&person4->list, &student_list);
	list_add(&person5->list, &student_list);
	printk(KERN_INFO "loading moudle\n");
	list_for_each_entry(ptr, &student_list, list){
		printk(KERN_ALERT"SID: %s, Name: %s, Class: %s, Gender: %s, Birthday: %s\n",ptr->SID, ptr->name, ptr->class, ptr->gender, ptr->birthday);
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
