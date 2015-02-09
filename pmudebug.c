#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include<linux/debugfs.h>
#include <linux/pmudebug.h>

MODULE_AUTHOR("Priya Sundaresan");
MODULE_DESCRIPTION("debugfs - pmu");
MODULE_LICENSE("Dual BSD/GPL");

int filevalue;
struct dentry *dirret,*fileret;
extern char ker_buf[PMU_BUFFER_MAX];
extern int buffer_index;

static ssize_t pmu_reader(struct file *fp, char __user *user_buffer,size_t count, loff_t *position)
{
	printk(KERN_INFO "\nIn reader%s",ker_buf);
     return simple_read_from_buffer(user_buffer, count, position, ker_buf, PMU_BUFFER_MAX);
} 

static ssize_t pmu_writer(struct file *fp, const char __user *user_buffer,size_t count, loff_t *position)
{
        if(count > PMU_BUFFER_MAX )
                return -EINVAL;
	buffer_index=count;
  	printk(KERN_INFO "\nIn writer %d",buffer_index);
	
        return simple_write_to_buffer(ker_buf, PMU_BUFFER_MAX, position, user_buffer, count);
} 

static const struct file_operations fops_debug ={
	.read=pmu_reader,
	.write=pmu_writer,
};

static int pmudebug_init(void) {
	//creating file in debugfs
	dirret = debugfs_create_dir("pmu",NULL);
	fileret = debugfs_create_file("trace",0644,dirret, &filevalue, &fops_debug);
	return 0;
}

static void pmudebug_exit(void)
{
	debugfs_remove_recursive(dirret);
}


module_init(pmudebug_init);
module_exit(pmudebug_exit);
//early_intcall(pmudebug_init);
