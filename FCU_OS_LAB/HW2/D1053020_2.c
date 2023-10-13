#include <linux/string.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday
{
	int day;
	int month;
	int year;
	char *name;
	struct list_head list;
};
static LIST_HEAD(birthday_list);

int simple_init(void)
{
	struct birthday *ptr;
	struct birthday *person, *person2, *person3, *person4, *person5;

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 22;
	person->month = 7;
	person->year = 2018;
	person->name = kmalloc(sizeof("Jane"), GFP_KERNEL);
	strcpy(person->name, "Jane");

	person2 = kmalloc(sizeof(*person2), GFP_KERNEL);
	person2->day = 4;
	person2->month = 3;
	person2->year = 2018;
	person2->name = kmalloc(sizeof("Michael"), GFP_KERNEL);
	strcpy(person2->name, "Michael");

	person3 = kmalloc(sizeof(*person3), GFP_KERNEL);
	person3->day = 4;
	person3->month = 3;
	person3->year = 2018;
	person3->name = kmalloc(sizeof("Lacey"), GFP_KERNEL);
	strcpy(person3->name, "Lacey");

	person4 = kmalloc(sizeof(*person2), GFP_KERNEL);
	person4->day = 4;
	person4->month = 3;
	person4->year = 2018;
	person4->name = kmalloc(sizeof("Michael"), GFP_KERNEL);
	strcpy(person4->name, "Michael");

	person5 = kmalloc(sizeof(*person3), GFP_KERNEL);
	person5->day = 4;
	person5->month = 3;
	person5->year = 2018;
	person5->name = kmalloc(sizeof("Lacey"), GFP_KERNEL);
	strcpy(person5->name, "Lacey");

	list_add(&person->list, &birthday_list);
	list_add(&person2->list, &birthday_list);
	list_add(&person3->list, &birthday_list);
	list_add(&person4->list, &birthday_list);
	list_add(&person5->list, &birthday_list);
	return 0;
}

void list_add(*ptr){

}

void list_add_tail(*ptr){

}


void simple_exit(void)
{
	struct birthday *ptr, *next;
	list_for_each_entry_safe(ptr, next, &birthdat_list, list)
	{
		list_del(&ptr->list);
		kfree(ptr);
	}
	printk(KERN_INFO "Removing module\n");
}

module_init(simple_init);
module_exit(simple_exit);
