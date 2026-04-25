#include <stdio.h>
#include <stdlib.h>

unsigned char memory[256], e, l, g;
unsigned int mbr;
unsigned short int mar, imm, pc = 0, reg[8];
unsigned char ir, ro0, ro1;

void search(){
    //memory[0] = 0b01110;
    memory[0]= 0b01110;
    memory[1]= 0b;
    mar = pc;
    // 0000 0000 0000 0000 0000 0000 0000 0000
    mbr = memory[mar];
    // 0000 0000 0000 0000 0000 0000 0000 0000
    printf("%024b\n", mbr);

    ir = mbr;
    printf("%i\n", ir);

    // hlt nop not
    if(ir == 0 || ir == 1 || ir == 13) {
        mbr = mbr << 8;
        mbr = mbr << 8;
    } else if(ir >=2 && ir <=12) {
        mbr = mbr << 8;
        mar ++;
        mbr = mbr | memory[mar];
        mbr = mbr <<8;
        mbr = mbr | memory[mar];
        mbr = mbr <<3;
    } else if(ir >= 14 && ir <=20){
        mbr = mbr << 11;
        mar++;
        mbr = mbr | memory[mar];
        mar++;
        mbr = mbr | memory[mar];
        mbr = mbr << 8;
    } else if(ir >=21 && ir<=29) {

    }
    printf("%024b\n",mbr);

}

int main()
{
    search();

    // para o commit
    return 0;
}


