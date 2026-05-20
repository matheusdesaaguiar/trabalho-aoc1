#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINHA 256
#define TAM_MEMORIA 256

unsigned char* memoria[TAM_MEMORIA];
unsigned char memory[TAM_MEMORIA]= {0};

unsigned int mbr;
unsigned short int mar = 0;
unsigned short int pc = 0;
unsigned short int reg[8] = {0};
unsigned short int imm = 0;

unsigned char ir, ro0, ro1;
unsigned char e = 0, l = 0, g = 0;

unsigned char opcode(char *instrucao)
{

    if (strcmp(instrucao, "hlt") == 0) return 0;
    if (strcmp(instrucao, "nop") == 0) return 1;
    if (strcmp(instrucao, "ldr") == 0) return 2;
    if (strcmp(instrucao, "str") == 0) return 3;
    if (strcmp(instrucao, "add") == 0) return 4;
    if (strcmp(instrucao, "sub") == 0) return 5;
    if (strcmp(instrucao, "mul") == 0) return 6;
    if (strcmp(instrucao, "div") == 0) return 7;
    if (strcmp(instrucao, "cmp") == 0) return 8;
    if (strcmp(instrucao, "movr") == 0) return 9;
    if (strcmp(instrucao, "and") == 0) return 10;
    if (strcmp(instrucao, "or") == 0) return 11;
    if (strcmp(instrucao, "xor") == 0) return 12;
    if (strcmp(instrucao, "not") == 0) return 13;
    if (strcmp(instrucao, "je") == 0) return 14;
    if (strcmp(instrucao, "jne") == 0) return 15;
    if (strcmp(instrucao, "jl") == 0) return 16;
    if (strcmp(instrucao, "jle") == 0) return 17;
    if (strcmp(instrucao, "jg") == 0) return 18;
    if (strcmp(instrucao, "jge") == 0) return 19;
    if (strcmp(instrucao, "jmp") == 0) return 20;
    if (strcmp(instrucao, "ld") == 0) return 21;
    if (strcmp(instrucao, "st") == 0) return 22;
    if (strcmp(instrucao, "movi") == 0) return 23;
    if (strcmp(instrucao, "addi") == 0) return 24;
    if (strcmp(instrucao, "subi") == 0) return 25;
    if (strcmp(instrucao, "muli") == 0) return 26;
    if (strcmp(instrucao, "divi") == 0) return 27;
    if (strcmp(instrucao, "lsh") == 0) return 28;
    if (strcmp(instrucao, "rsh") == 0) return 29;

    return 255;
}

unsigned char registrador_codigo(char *regis)
{

    if (strcmp(regis, "r0") == 0) return 0;
    if (strcmp(regis, "r1") == 0) return 1;
    if (strcmp(regis, "r2") == 0) return 2;
    if (strcmp(regis, "r3") == 0) return 3;
    if (strcmp(regis, "r4") == 0) return 4;
    if (strcmp(regis, "r5") == 0) return 5;
    if (strcmp(regis, "r6") == 0) return 6;
    if (strcmp(regis, "r7") == 0) return 7;

    return 255;
}

void preenchendo_memoria(char *nome_arquivo)
{

    FILE *arquivo;
    char linha[TAM_LINHA];

    arquivo = fopen(nome_arquivo, "r");

    if (arquivo == NULL)
    {

        perror("Erro ao abrir arquivo");
        return;
    }

    while (fgets(linha, TAM_LINHA, arquivo))
    {

        linha[strcspn(linha, "\n")] = '\0';

        char *endereco_str = strtok(linha, ";");
        char *tipo = strtok(NULL, ";");
        char *conteudo = strtok(NULL, "\n");

        if (!endereco_str || !tipo || !conteudo)
            continue;

        int endereco = (int)strtol(endereco_str, NULL, 16);

        if (strcmp(tipo, "i") == 0)
        {

            char *instrucao = strtok(conteudo, " ,");
            char *registrador = strtok(NULL, " ,");
            char *valor = strtok(NULL, " ,");

            if (instrucao)
                memoria[endereco] = strdup(instrucao);

            if (registrador)
                memoria[endereco + 1] = strdup(registrador);

            if (valor)
                memoria[endereco + 2] = strdup(valor);
        }

        else if (strcmp(tipo, "d") == 0)
        {

            memoria[endereco] = strdup(conteudo);
        }
    }

    fclose(arquivo);
}

void converter_memoria()
{
    int i;

    for(i = 0; i < TAM_MEMORIA; i++)
    {
        if(memoria[i] == NULL)
            continue;

        char *token1 = memoria[i];

        unsigned char op = opcode(token1);

        // =========================
        // DADOS
        // =========================
        if(op == 255)
        {
            memory[i] = (unsigned char)strtol(token1, NULL, 16);
            continue;
        }

        // =========================
        // HLT / NOP
        // 8 bits
        // =========================
        if(op == 0 || op == 1)
        {
            memory[i] = op << 3;
        }

        // =========================
        // NOT
        // opcode(5) reg0(3)
        // =========================
        else if(op == 13)
        {
            unsigned char r0 =
            registrador_codigo(memoria[i + 1]);

            unsigned char instrucao =
            (op << 3) | r0;

            memory[i] = instrucao;

            i += 1;
        }

        // =========================
        // instruções 16 bits
        // opcode(5) r0(3) r1(3) 00000
        // =========================
        else if(op >= 2 && op <= 12)
        {
            unsigned char r0 =
            registrador_codigo(memoria[i + 1]);

            unsigned char r1 =
            registrador_codigo(memoria[i + 2]);

            unsigned short instrucao =
            (op << 11) |
            (r0 << 8) |
            (r1 << 5);

            memory[i]     = (instrucao >> 8) & 0xFF;
            memory[i + 1] = instrucao & 0xFF;

            i += 2;
        }

        // =========================
        // jumps
        // opcode(5) 000 endereco(16)
        // =========================
        else if(op >= 14 && op <= 20)
        {
            unsigned short endereco =
            (unsigned short)strtol(memoria[i + 1], NULL, 16);

            unsigned int instrucao =
            (op << 19) |
            endereco;

            memory[i]     = (instrucao >> 16) & 0xFF;
            memory[i + 1] = (instrucao >> 8) & 0xFF;
            memory[i + 2] = instrucao & 0xFF;

            i += 2;
        }

        // =========================
        // ld / st
        // opcode(5) r0(3) endereco(16)
        // =========================
        else if(op == 21 || op == 22)
        {
            unsigned char r0 =
            registrador_codigo(memoria[i + 1]);

            unsigned short endereco =
            (unsigned short)strtol(memoria[i + 2], NULL, 16);

            unsigned int instrucao =
            (op << 19) |
            (r0 << 16) |
            endereco;

            memory[i]     = (instrucao >> 16) & 0xFF;
            memory[i + 1] = (instrucao >> 8) & 0xFF;
            memory[i + 2] = instrucao & 0xFF;

            i += 2;
        }

        // =========================
        // imediatas
        // opcode(5) r0(3) imm(16)
        // =========================
        else if(op >= 23 && op <= 29)
        {
            unsigned char r0 =
            registrador_codigo(memoria[i + 1]);

            unsigned short imediato =
            (unsigned short)strtol(memoria[i + 2], NULL, 16);

            unsigned int instrucao =
            (op << 19) |
            (r0 << 16) |
            imediato;

            memory[i]     = (instrucao >> 16) & 0xFF;
            memory[i + 1] = (instrucao >> 8) & 0xFF;
            memory[i + 2] = instrucao & 0xFF;

            i += 2;
        }
    }
}

void exibirCPU()
{

    int i;

    printf("\n================ CPU ================\n");

    printf("PC : %04X\n", pc);
    printf("IR : %02X\n", ir);
    printf("MAR: %04X\n", mar);
    printf("MBR: %032b\n", mbr);
    printf("IMM: %04X\n", imm);

    printf("\nREGISTRADORES\n");

    for(i = 0; i < 8; i++)
    {

        printf("R%d: %04X\n", i, reg[i]);
    }

    printf("\nFLAGS\n");

    printf("E: %d\n", e);
    printf("L: %d\n", l);
    printf("G: %d\n", g);

    printf("\nMEMORIA\n");

    for(i = 0; i < 256; i++)
    {

        printf("%02X: %02X  ", i, memory[i]);

        if((i + 1) % 8 == 0)
            printf("\n");
    }
}

void search()
{
    mar = pc;

    unsigned char primeiro_byte = memory[mar];

    // opcode está nos 5 bits mais altos
    ir = (primeiro_byte >> 3) & 0x1F;

    // =========================
    // instruções 8 bits
    // =========================
    if(ir == 0 || ir == 1 || ir == 13)
    {
        mbr = memory[mar];
    }

    // =========================
    // instruções 16 bits
    // =========================
    else if(ir >= 2 && ir <= 12)
    {
        mbr =
        (memory[mar] << 8) |
        memory[mar + 1];
    }

    // =========================
    // instruções 24 bits
    // =========================
    else if(ir >= 14 && ir <= 29)
    {
        mbr =
        (memory[mar] << 16) |
        (memory[mar + 1] << 8) |
        memory[mar + 2];
    }
}

void decode()
{
    ro0 = 0;
    ro1 = 0;
    imm = 0;

    // hlt / nop
    if(ir == 0 || ir == 1)
        return;

    // =========================
    // NOT
    // opcode = bits 7-3
    // ro0    = bits 2-0
    // =========================
    if(ir == 13)
    {
        ro0 = mbr & 0x07;
        return;
    }

    // =========================
    // instruções 16 bits
    // opcode = 15-11
    // ro0    = 10-8
    // ro1    = 7-5
    // =========================
    if(ir >= 2 && ir <= 12)
    {
        ro0 = (mbr >> 8) & 0x07;
        ro1 = (mbr >> 5) & 0x07;

        return;
    }

    // =========================
    // jumps
    // opcode = 23-19
    // endereço = 15-0
    // =========================
    if(ir >= 14 && ir <= 20)
    {
        mar = mbr & 0xFFFF;

        return;
    }

    // =========================
    // ld/st
    // opcode = 23-19
    // ro0    = 18-16
    // endereço = 15-0
    // =========================
    if(ir == 21 || ir == 22)
    {
        ro0 = (mbr >> 16) & 0x07;

        mar = mbr & 0xFFFF;

        return;
    }

    // =========================
    // imediatas
    // opcode = 23-19
    // ro0    = 18-16
    // imm    = 15-0
    // =========================
    if(ir >= 23 && ir <= 29)
    {
        ro0 = (mbr >> 16) & 0x07;

        imm = mbr & 0xFFFF;

        return;
    }
}
void execute()
{
    printf("Chegou na execucao\n");
    printf("%032b\n", mbr);
    if(ir == 0b00000) return; // hlt
    if(ir == 0b00001){
        pc+=1;
        return;
    } //nop
    if(ir == 0b00010)  // ldr
    {
        reg[ro0] = memory[reg[ro1]] | (memory[reg[ro1] + 1] << 8);

        pc += 2;
    }
    if( ir == 0b00011 )   // str
    {
        memory[mar]     = reg[ro0] & 0xFF;
        memory[mar + 1] = reg[ro0] >> 8;
        pc += 2;
        return;
    }
    if( ir == 0b00100)   //add
    {
        reg[ro0] = reg[ro0] +reg[ro1];
        pc += 2;
        return;
    }
    if( ir == 0b00101)   // sub
    {
        reg[ro0] = reg[ro0] - reg[ro1];
        pc += 2;
        return;
    }
    if( ir == 0b00110)   // mul
    {
        reg[ro0] = reg[ro0] *reg[ro1];
        pc += 2;
        return;
    }
    if( ir == 0b00111 )   // div
    {
        if(reg[ro1] == 0)
        {

            printf("ERRO DIVISAO POR ZERO\n");
            pc+=2;
            return;
        }
        reg[ro0] = reg[ro0] / reg[ro1];
        pc += 2;
        return;
    }
    if( ir == 0b01000)   // cmp
    {
        e = 0;
        l = 0;
        g = 0;
        if(reg[ro0] > reg[ro1]) g = 1;
        if(reg[ro0] < reg[ro1]) l = 1;
        if(reg[ro0] == reg[ro1]) e = 1;
        pc+= 2;
        return;
    }
    if( ir == 0b01001)   //movr
    {
        reg[ro0] = reg[ro1];
        pc+= 2;
        return;
    }
    if( ir == 0b01010)   // and
    {
        reg[ro0] = reg[ro0] & reg[ro1];
        pc+=2;

        return;
    }
    if( ir == 0b01011)   // or
    {
        reg[ro0] = reg[ro0] | reg[ro1];
        pc+= 2;
        return;
    }

    if( ir == 0b01100)  // xor
    {
        reg[ro0] = reg[ro0]^ reg[ro1];
        pc+=2;
        return;
    }
    if(ir == 0b01101)   //not
    {
        reg[ro0] = ~reg[ro0];
        pc +=1;
        return;
    }
    if( ir == 0b01110)  // je
    {
        if(e)
        {
            pc = mar;
        }
        else
        {
            pc+=3;
        }
        return;
    }
    if (ir == 0b01111)   //jne
    {
        if(!e)
        {
            pc = mar;
        }
        else
        {
            pc+=3;
        }
        return;
    }
    if(ir == 0b10000)   // jl
    {
        if(l)
        {
            pc = mar;
        }
        else
        {
            pc+=3;
        }
        return;
    }
    if( ir == 0b10001)   // jle
    {
        if(l || e)
        {
            pc = mar;
        }
        else
        {
            pc+=3;
        }
        return;
    }
    if(ir == 0b10010)   //jg
    {
        if(g)
        {
            pc = mar;
        }
        else
        {
            pc+=3;
        }
        return;
    }
    if( ir == 0b10011)   // jge
    {
        if( g || e)
        {
            pc = mar;
        }
        else
        {
            pc+=3;
        }
        return;
    }
    if ( ir == 0b10100)  // jmp
    {
        pc = mar;
        return;
    }

    if( ir == 0b10101)   // ld
    {
        reg[ro0] = memory[mar] | (memory[mar + 1] << 8);

        pc += 3;;
    }
    if( ir == 0b10110)   // st
    {
        memory[reg[ro1]]     = reg[ro0] & 0xFF;
        memory[reg[ro1] + 1] = reg[ro0] >> 8;
        pc += 3;
    }
    if( ir == 0b10111)  //movi
    {
        reg[ro0] = imm;
        pc += 3;
        return;
    }
    if( ir == 0b11000)   //addi
    {
        reg[ro0] += imm;
        pc+= 3;
        return;void converter_memoria()
{
    int i;

    for(i = 0; i < TAM_MEMORIA; i++)
    {
        if(memoria[i] == NULL)
            continue;

        char *token1 = memoria[i];

        unsigned char op = opcode(token1);

        // =========================
        // DADOS
        // =========================
        if(op == 255)
        {
            memory[i] = (unsigned char)strtol(token1, NULL, 16);
            continue;
        }

        // =========================
        // HLT / NOP
        // 8 bits
        // =========================
        if(op == 0 || op == 1)
        {
            memory[i] = op << 3;
        }

        // =========================
        // NOT
        // opcode(5) reg0(3)
        // =========================
        else if(op == 13)
        {
            unsigned char r0 =
            registrador_codigo(memoria[i + 1]);

            unsigned char instrucao =
            (op << 3) | r0;

            memory[i] = instrucao;

            i += 1;
        }

        // =========================
        // instruções 16 bits
        // opcode(5) r0(3) r1(3) 00000
        // =========================
        else if(op >= 2 && op <= 12)
        {
            unsigned char r0 =
            registrador_codigo(memoria[i + 1]);

            unsigned char r1 =
            registrador_codigo(memoria[i + 2]);

            unsigned short instrucao =
            (op << 11) |
            (r0 << 8) |
            (r1 << 5);

            memory[i]     = (instrucao >> 8) & 0xFF;
            memory[i + 1] = instrucao & 0xFF;

            i += 2;
        }

        // =========================
        // jumps
        // opcode(5) 000 endereco(16)
        // =========================
        else if(op >= 14 && op <= 20)
        {
            unsigned short endereco =
            (unsigned short)strtol(memoria[i + 1], NULL, 16);

            unsigned int instrucao =
            (op << 19) |
            endereco;

            memory[i]     = (instrucao >> 16) & 0xFF;
            memory[i + 1] = (instrucao >> 8) & 0xFF;
            memory[i + 2] = instrucao & 0xFF;

            i += 2;
        }

        // =========================
        // ld / st
        // opcode(5) r0(3) endereco(16)
        // =========================
        else if(op == 21 || op == 22)
        {
            unsigned char r0 =
            registrador_codigo(memoria[i + 1]);

            unsigned short endereco =
            (unsigned short)strtol(memoria[i + 2], NULL, 16);

            unsigned int instrucao =
            (op << 19) |
            (r0 << 16) |
            endereco;

            memory[i]     = (instrucao >> 16) & 0xFF;
            memory[i + 1] = (instrucao >> 8) & 0xFF;
            memory[i + 2] = instrucao & 0xFF;

            i += 2;
        }

        // =========================
        // imediatas
        // opcode(5) r0(3) imm(16)
        // =========================
        else if(op >= 23 && op <= 29)
        {
            unsigned char r0 =
            registrador_codigo(memoria[i + 1]);

            unsigned short imediato =
            (unsigned short)strtol(memoria[i + 2], NULL, 16);

            unsigned int instrucao =
            (op << 19) |
            (r0 << 16) |
            imediato;

            memory[i]     = (instrucao >> 16) & 0xFF;
            memory[i + 1] = (instrucao >> 8) & 0xFF;
            memory[i + 2] = instrucao & 0xFF;

            i += 2;
        }
    }
}
    }
    if( ir == 0b11001)   //subi
    {
        reg[ro0] -= imm;
        pc+=3;
        return;
    }
    if( ir == 0b11010)   //muli
    {
        reg[ro0] *= imm;
        pc+=3;
        return;
    }
    if( ir == 0b11011)  //divi
    {
        if(imm == 0)
        {
            printf("ERRO DIVISAO POR ZERO\n");
            pc += 3;
            return;
        }
        reg[ro0] /= imm;
        pc+=3;
        return;
    }
    if( ir == 0b11100)  // lsh
    {
        reg[ro0] = reg[ro0] << imm;
        pc+=3;
        return;
    }
    if(ir == 0b11101)  //rsh
    {
        reg[ro0] = reg[ro0] >> imm;
        pc+=3;
        return;
    }
}

int main()
{

    preenchendo_memoria("programa.txt");

    converter_memoria();

    ir = 255;

    while(ir != 0)
    {

        exibirCPU();

        printf("\nPRESSIONE ENTER PARA FETCH");
        getchar();

        search();

        //printf("\nPRESSIONE ENTER PARA DECODE");
        //getchar();

        decode();

        //printf("\nPRESSIONE ENTER PARA EXECUTE");
        //getchar();

        execute();

    }

    exibirCPU();

    printf("\nPROGRAMA FINALIZADO\n");

    return 0;
}
