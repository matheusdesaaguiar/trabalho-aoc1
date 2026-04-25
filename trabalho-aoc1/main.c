#include <stdio.h>
#include <stdlib.h>

unsigned char memory[256], e, l, g;
unsigned int mbr;
unsigned short int mar, imm, pc = 0, reg[8];
unsigned char ir, ro0, ro1;

void search(){
    memory[0] = 0b00100000;
    memory[1] = 0b01100000;
    memory[2] = 0b00011110;

    mar = pc;
    // 0000 0000 0000 0000 0000 0000 0000 0000
    mbr = memory[mar];
    // 0000 0000 0000 0000 0000 0000 0000 0000

    ir = mbr >> 3;

    // hlt nop not
    if(ir == 0 || ir == 1 || ir == 13) {
        mbr = mbr << 8;
        mbr = mbr << 8;
    } else if(ir >=2 && ir <=12) {
        mar ++;
        mbr = mbr << 8;
        mbr = mbr | memory[mar];
        mbr = mbr <<8;
    } else if(ir >= 14 && ir <=20){
        mar++;
        mbr = mbr << 8;
        mbr = mbr | memory[mar];
        mar++;
        mbr = mbr << 8;
        mbr = mbr | memory[mar];
    } else if(ir >=21 && ir<=29) {
        mbr = mbr << 8;
        mar++;
        mbr = mbr | memory[mar];
        mbr = mbr << 8;
        mar++;
        mbr = mbr | memory[mar];
    }
}

int main()
{
    search();
    return 0;
}


