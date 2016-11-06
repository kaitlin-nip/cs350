/*
 * "Hello, world!" kernel module - /dev version
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched.h>

#include <asm/uaccess.h>


struct task_struct *task_p = NULL;
char m_buf [48]; // 10+10+10+10+4+padding
int bytes_left = 0;
int total_written = 0;
int written;
int cp_success; // 0 == success
/*
 
*/

static ssize_t my_write_to_user(char* buf, size_t count){
	// write everything left
	if(bytes_left <= count){
		cp_success = copy_to_user(buf, m_buf + total_written, bytes_left);
		written = bytes_left;
		bytes_left = 0;
		total_written += bytes_left;
		printk("written in if=%d\n", bytes_left);
		return written;
	}
	// write "count" bytes
	cp_success = copy_to_user(buf, m_buf + total_written, count);
	bytes_left -= count;
	total_written += count;
	printk("written in else=%d\n", (int)count);
	return count;
}

static ssize_t process_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos){
	printk("in process_read\n");
	if(bytes_left > 0){
		return my_write_to_user(buf, count);
		
	}else{

		if(task_p == NULL){
			task_p = &init_task;
		}else{
			task_p = next_task(task_p);
			if (task_p == &init_task){
				return 0;
			}
		}
		
		bytes_left = sprintf(m_buf, "%d %d %d %d \n", 
			(int)task_p->pid, 
			(int)task_p->real_parent->pid, 
			(int)task_p->state,
			(int)task_thread_info(task_p)->cpu);
		total_written = 0;
		printk("bytes_left=%d\n", bytes_left);
		return my_write_to_user(buf, count);		
	}


	
	// write the buffer to client in bits and pieces
	/*int byte_copied;
	int byte_left;
	

	if(m_size == 0){
		m_size = get_all_pid(&m_buf);
		*ppos = 0;
	}

	if(*ppos >= m_size){
		return 0;
	}

	byte_left = m_size-*ppos;
	if( byte_left <= count){
		byte_copied = copy_to_user(buf, m_buf+*ppos, byte_left);
		printk("byte_copied=%d\n", byte_copied);
		if (byte_copied)
			return -EINVAL;
		*ppos += byte_left;
		return byte_left;
	}else{
		byte_copied = copy_to_user(buf, m_buf+*ppos, count);
		printk("byte_copied=%d\n", byte_copied);
		if (byte_copied)
			return -EINVAL;
		*ppos += count;
	}*/
	
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations process_fops = {
	.owner		= THIS_MODULE,
	.read		= process_read,
};

static struct miscdevice process_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/hello.
	 */
	"process_list",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&process_fops
};

static int __init
process_init(void)
{
	int ret;

	/*
	 * Create the "hello" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/hello device using
	 * the default rules.
	 */
	ret = misc_register(&process_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"process_list\" misc device\n");

	return ret;
}

module_init(process_init);

static void __exit
process_exit(void)
{
	misc_deregister(&process_dev);
}

module_exit(process_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kaiyik Nip <knip1 AT binghamton.edu>");
MODULE_DESCRIPTION("A kernel module to print out all current process pids, ppids, state, and assigned cpu.");
MODULE_VERSION("dev");