#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Rostyslav Kunchiy <roctik4x@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int counter = 1;
module_param(counter, uint, 0644);
MODULE_PARM_DESC(counter, "How many times 'Hello, world!' will be displayed");

struct hello_data {
	struct list_head list;
	ktime_t timestamp;
};

static LIST_HEAD(hello_data_head);

static int __init hello_init(void)
{
	int i;

	BUG_ON(counter > 10);

	if (counter == 0 || (counter >= 5 && counter <= 10)) {
		pr_warn("Bad parameter. Continuing...\n");
	}

	for (i = 0; i < counter; i++) {
		struct hello_data *ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);

		if (i == 5) {
			ptr = 0;
		}

		ptr->timestamp = ktime_get();
		list_add(&ptr->list, &hello_data_head);

		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_data *ptr, *tmp;

	list_for_each_entry_safe(ptr, tmp, &hello_data_head, list) {
		pr_info("Event time: %lld ns\n", ktime_to_ns(ptr->timestamp));
		list_del(&ptr->list);
		kfree(ptr);
	}
}

module_init(hello_init);
module_exit(hello_exit);
