- Name: Vincent Wang
- CSCI3753-103
yiwa3841
108316003

Contents of submission: 
  1. Makefile: command for the obj for pa2_char.o module
  2. Driver.c: the driver file for the functions: seek, write, read, close, open, and initiations.
  3. Test.c: command interface for user to operate the module created
  
Instructions:
  - r, w, s stand for reading, writing and running the program
  - Check each file are at the proper location and modules are installed properly
  - Using the following instructions:
    "sudo make -C /lib/modules/$(uname -r)/build M=$PWD modules" to compile module files
    "sudo insmod pa2_char_driver.ko" to load the module to kernel
    "sudo mknod -m 777 /dev/pa2_char_device c 245 0" to create device file in /dev
    "sudo gcc pa2_char_test.c" to compile the test app
  - Using dmesg to check the output of the modules
  - To remove the module, use: 
    "rmmod pa2_char_driver"
