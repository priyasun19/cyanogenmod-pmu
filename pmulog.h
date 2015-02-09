#include<linux/string.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pmudebug.h>

static char ker_buf[PMU_BUFFER_MAX]; 
static int buffer_index=0;
EXPORT_SYMBOL(ker_buf);
EXPORT_SYMBOL(buffer_index);

static void pmu_log(char *data)
{
	//printk(KERN_INFO "\nIn pmu_log %s",data);
	int index = (buffer_index + PMU_LOGMSG_DELTA) > PMU_BUFFER_MAX ? 0 : buffer_index;
	memset(ker_buf+index,'-',PMU_LOGMSG_DELTA);
	strcpy(ker_buf+index,data);
	buffer_index = index + PMU_LOGMSG_DELTA;
}
