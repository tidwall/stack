# stack

A C library for managing stacks for coroutines and threads.

It's designed for safety and efficency.

## Example

```c
#include "stack.h"

// Create a manager for creating stacks.
struct stack_mgr mgr;
stack_mgr_init(&mgr, 0);

// Get a stack.
struct stack stack;
int ret = stack_get(&mgr, &stack);
if (ret == -1) {
    // Error getting a stack, check errno.
}

// Get the stack base address and stack size.
void *addr = stack_addr(&stack);
size_t size = stack_size(&stack);

// Give the stack back when you are done with it.
stack_put(&mgr, &stack);

// Release the manager.
stack_mgr_destroy(&mgr);

```

## Options

```c
struct stack_opts {
    size_t stacksz;       // Stack size (default 8388608)
    size_t defcap;        // Default stack_group capacity (default 4)
    size_t maxcap;        // Max stack_group capacity (default 8192)
    size_t gapsz;         // Size of gap (guard) pages (default 1048576)
    bool useguards;       // Use mprotect'ed guard pages (default false)
    bool nostackfreelist; // Do not use a stack free list (default false)
    bool nopagerelease;   // Do not early release mmapped pages (default false)
};
```

## Details

Stacks are stored in groups. The `stack_mgr` manages the groups in a linked
list that dynamically grows and shrinks as more stacks are added and removed.

A group allocation contains the group struct and all its stacks. 
Each stack is separated by an optional gap page, which can also act as
the guard page. There will be one more gap pages than stack to ensure
that each stack is sandwiched by gaps or guards, allowing for overflow
padding or segv detection for stacks that grow up or down.
For example, let's say the group has a capacity of 3. The memory layout
will end up look something like this

```
[ GUARD ][ Stack1 ][ GUARD ][ Stack2 ][ GUARD ][ Stack3 ][ GUARD ]
```

where the entire group is a single mmap and each stack is sandwiched by
guard pages.

## License

Source code is available under the MIT License.
