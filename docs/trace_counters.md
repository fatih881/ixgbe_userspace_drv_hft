# Summary 
Some functions have counters in enter-happy exit path, since the processes can exit without implemented returns. (e.g, kpanic)
Within this traces, this possibility can be traced on hexdump.  
Since increasing counters is a atomic operation, this implementation found logical especially on init functions. In functions which will be going to run on data path, it can be optional / never implemented.
# Usage 
Even if the program usually doesn't write anything to terminal, this counters can be observed with GNU debugging tool(gdb). See more about GNU Debugger on [official website][gdb]. 
This usage example will be hands on with gdb, with a run attempt on unvalid parameter. 
use gdb ./binary for entering mode.
(gdb) break unbind
Breakpoint 1 at 0x400862: file pci.c, line 10.
(gdb) run 0
Starting program: /path/to/binary/driver 0 **Driver is binary name, and 0 is parameter.**

This GDB supports auto-downloading debuginfo from the following URLs:
  <ima:enforcing>
  <https://debuginfod.fedoraproject.org/>
  <ima:ignore>
Enable debuginfod for this session? (y or [n]) y
Debuginfod has been enabled.
To make this setting permanent, add 'set debuginfod enabled on' to .gdbinit.
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".

Breakpoint 1, unbind (pci=0xc00000 <error: Cannot access memory at address 0xc00000>,
    target_drv=0x40 <error: Cannot access memory at address 0x40>,
    trace=0x10 <error: Cannot access memory at address 0x10>) at pci.c:10
10    {
(gdb) p/x debug_trace **With this command, we can trace all the structure from memory itself.**
**Also display debug_trace can be used to do it automaticly after every `next`.**
$1 = {unbind_counter = 0x0, alloc_hugepage_counter = 0x0, virt2phy_counter = 0x0}
(gdb) next **By next, we can run the codes one by one.**
11        if (likely(trace)) {
(gdb) next
12            (*trace)++;
(gdb) p/x debug_trace
$2 = {unbind_counter = 0x0, alloc_hugepage_counter = 0x0, virt2phy_counter = 0x0} 
**As we implemented, 0x0 means it doesn't entered to function, and exiting in 0x1 means an err, while 0x2 is happy path.**
(gdb) next
16        snprintf(path, sizeof(path),
(gdb) next
19        fd = open(path, O_WRONLY);
(gdb) next
20        if (unlikely(fd < 0)) return -1;
(gdb) next
43    }
(gdb) next
main (argc=2, argv=0x7fffffffd8e8) at main.c:18
18        if (unlikely(err != 0)){
(gdb) next
19            return -err;
(gdb) next
29    };
(gdb) p/x debug_trace 
$4 = {unbind_counter = 0x1, alloc_hugepage_counter = 0x0, virt2phy_counter = 0x0}
(gdb) next
nDownloading 2.61 K source file /usr/src/debug/glibc-2.42-9.fc43.x86_64/csu/../sysdeps/nptl/libc_start_call_main.h
__libc_start_call_main (main=main@entry=0x4004f6 <main>, argc=argc@entry=2,
    argv=argv@entry=0x7fffffffd8e8) at ../sysdeps/nptl/libc_start_call_main.h:74
74      exit (result);
(gdb) p/x debug_trace
$5 = {unbind_counter = 0x1, alloc_hugepage_counter = 0x0, virt2phy_counter = 0x0}
(gdb) next
[Inferior 1 (process 55552) exited with code 01]
(gdb) **After exiting the program, we can't see debug_trace since it will not be here.**
> Also keep in mind that `echo $?` will return the latest exit code.
[gdb]:gnu.org/software/gdb/