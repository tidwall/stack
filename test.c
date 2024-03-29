#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "stack.h"

int main(void) {
    // Get 1000 stacks. Write data, read data, and put them back.
    const int N = 1000;
    const size_t STACKSZ = 262144;
    struct stack_mgr mgr;
    struct stack_opts opts = { .stacksz = STACKSZ };
    stack_mgr_init(&mgr, &opts);
    struct stack stacks[N];
    for (int i = 0 ; i < N; i++) {
        
        assert(stack_get(&mgr, &stacks[i]) == 0);
    }
    for (int i = 0 ; i < N; i++) {
        char *addr = stack_addr(&stacks[i]);
        size_t size = stack_size(&stacks[i]);
        assert(addr);
        assert(size == STACKSZ);
        memset(addr, 99, size);
        for (size_t j = 0; j < 5000; j++) {
            assert(addr[j] == 99);
        }
        for (size_t j = size-1; j > size-5000; j--) {
            assert(addr[j] == 99);
        }
        stack_put(&mgr, &stacks[i]);
    }
    stack_mgr_destroy(&mgr);
    printf("PASSED\n");
    return 0;
}
