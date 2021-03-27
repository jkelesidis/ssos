# Super Simple Operating System

A super simple operating system written in C and x86 assembly devloped as a pet-project for Creative Day on March 26th 2021.


### Features
* x86 Bootloader
* 32bit Protected mode switch
* Keyboard and VGA textmode drivers
* Interrupt handling
* Memory allocator
* Simple Shell

---
### Frequently Asked Questions

#### Why?

Why not?

#### But why?

I always wanted to write a bootloader, so after I wrote it I took it one step further with a very simple OS kernel.

#### What can I do with it?

Not much besides learn how a system boots.

#### Can I write software for the OS?

Sure, read **Super Simple OS Developer's Manual** (not yet written).

#### How to run it?

Assuming you have a x86 Qemu installed, download `os-image.bin` and run `qemu os-image.bin`. It should throw you into the simple shell.


---
### Code structure

The `boot` folder contains the bootsector, functionality to load the kernel from the disk and the switch to protected mode.

The `cpu` folder contains the Interrupt Descriptor Tables and Interrupt Service Routines implementation, a timer and some glue code.

The `drivers` folder contains the keyboard and text mode VGA drivers.

The `kernel` folder contains the main kernel method (`kernel_main`) and some test routines for the timer and the shell.

There's no standard library for the OS, so the `libc` folder contains an implementation of the following along with other helper functions:
* memset
* memcpy
* malloc
* strlen
* strcmp

---
### Resources

* Writing a simple operating system from scratch ([link](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf))
* OSDev wiki ([link](https://wiki.osdev.org/Main_Page))
* The little book about OS development ([link](https://littleosbook.github.io/))
* OS tutorial on GH ([link](https://github.com/cfenollosa/os-tutorial))