
#include "include/head.h"

void Main(void){
    int i; 
    for (i = 0xa0000; i <= 0xaffff; i++) {
        write_mem8(i, 12); /* MOV BYTE [i],15 */
    }
    for (;;) {
        io_hlt();
    }
}