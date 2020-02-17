

#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

MODULE_LICENSE("GPL");


#define BUFFER_SIZE 1024
#define DEVNAME "device"
#define MAJOR 245
/* Define device_buffer and other global data structures you will need here */


int open_c = 0;
int close_c = 0;
char* device_buffer;

ssize_t pa2_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	int max_bytes = BUFFER_SIZE - *offset; /*max bytes can be read from offset to buff size*/
	int bytesRead, bytesToRead; /*bytesRead: bytes actually read; bytesToRead: bytes to read*/
	
	/*check if the maximum to be read is in size of the buffer*/
	if (max_bytes > length){
		bytesToRead = length;
	}	
	else{
		bytesToRead = max_bytes;
	}

	if (bytesToRead == 0){
		printk(KERN_INFO "Reach the end of the device.\n"); 
	}
	bytesRead = bytesToRead - copy_to_user(buffer, device_buffer + *offset, bytesToRead); 
	/*get bytes actually read by subtraction*/
	printk(KERN_ALERT "%d bytes have been read from the file\n", bytesRead);
	*offset += bytesRead;
	/*updating the current position*/
	return bytesRead;
}



ssize_t pa2_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
	int max_bytes = BUFFER_SIZE - *offset; /*max bytes can be write from offset to buff size*/
	int bytesWrite, bytesToWrite; /*bytesWrite: bytes actually write; bytesToWrite: bytes to write*/
	
	/*check if the maximum to be written is in size of the buffer*/
	if (max_bytes > length){
		bytesToWrite = length;
	}	
	else{
		bytesToWrite = max_bytes;
	}
	
	bytesWrite = bytesToWrite - copy_from_user(device_buffer + *offset, buffer, bytesToWrite); 
	/*get bytes actually wrote by subtraction*/
	printk(KERN_ALERT "%d bytes have been written to the file\n", bytesWrite);
	*offset += bytesWrite;
	/*updating the current position*/
	return bytesWrite;
}


int pa2_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	open_c++;
	printk(KERN_ALERT "Opening file <simple_char_driver>. %d times opened\n", open_c);
	return 0;
}

int pa2_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	close_c++;
	printk(KERN_ALERT "Closing file <simple_char_driver>. %d times closed\n", close_c);
	return 0;
}

loff_t pa2_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	loff_t position = 0;
	
	switch(whence){
		case 0: /*SEEK_SET, current position is the value of offset*/
			position = offset;
			break;
		case 1: /*SEEK_CUR, current position is incrementing with bytes of the offset*/
			position = pfile->f_pos + offset;
			break;
		case 2: /*SEEK_END, set to offset bytes before ends*/
			position = BUFFER_SIZE - offset;		
		default: /*error mesg*/
			printk(KERN_ALERT "ERROR");
			return -EINVAL;
	}
	
	/*seek before the beginning of the file*/
	if (position > BUFFER_SIZE){
		position = BUFFER_SIZE;
	}
	else if (position < 0){
		position = 0;
	}
	pfile->f_pos = position;
	return 0;
}

struct file_operations pa2_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
	.open    = pa2_char_driver_open,
	.release = pa2_char_driver_close,
	.llseek  = pa2_char_driver_seek, 
	.read    = pa2_char_driver_read,
	.write   = pa2_char_driver_write
};

static int pa2_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
		
	printk(KERN_ALERT "inside %s function\n", __FUNCTION__);
	/*allocate the memory for initialization*/	
	device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
	/* register the device */
	register_chrdev(MAJOR, DEVNAME, &pa2_char_driver_file_operations);
	return 0;
}

static void pa2_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	printk(KERN_ALERT "inside %s function\n", __FUNCTION__);
	/* free the memory before exiting*/
	kfree(device_buffer);
	/* unregister  the device using the register_chrdev() function. */
	unregister_chrdev(MAJOR, DEVNAME);
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(pa2_char_driver_init);
module_exit(pa2_char_driver_exit);
