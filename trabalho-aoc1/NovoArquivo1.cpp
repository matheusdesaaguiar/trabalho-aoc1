#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MEMORIA 256
#define TAM_LINHA 256

/* =========================================================
   MEMORIA
========================================================= */

char *memoria[TAM_MEMORIA];
unsigned char memory[TAM_MEMORIA];

/* =========================================================
   CPU
========================================================= */

unsigned short reg[8];

unsigned short pc = 0;
unsigned short mar = 0;
unsigned short imm = 0;

unsigned short mbr = 0;

unsigned char ir = 0;
unsigned char ro0 = 0;
unsigned char ro1 = 0;

unsigned char e = 0;
unsigned char l = 0;
unsigned char g = 0;

/* =========================================================
   OPCODES
========================================================= */

unsigned char opcode(char *instrucao) {

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

/* =========================================================
   REGISTRADORES
========================================================= */

unsigned char registrador_codigo(char *regis) {

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

/* =========================================================
   BINARIO
========================================================= */

void binario(unsigned char valor) {

    int i;

    for(i = 7; i >= 0; i--) {

        printf("%d", (valor >> i) & 1);
    }
}

/* =========================================================
   LEITURA DO ARQUIVO
========================================================= */

void preenchendo_memoria(char *nome_arquivo) {

    FILE *arquivo;
    char linha[TAM_LINHA];

    arquivo = fopen(nome_arquivo, "r");

    if (arquivo == NULL) {

        perror("Erro ao abrir arquivo");
        return;
    }

    while (fgets(linha, TAM_LINHA, arquivo)) {

        linha[strcspn(linha, "\n")] = '\0';

        char *endereco_str = strtok(linha, ";");
        char *tipo = strtok(NULL, ";");
        char *conteudo = strtok(NULL, "\n");

        if (!endereco_str || !tipo || !conteudo)
            continue;

        int endereco = (int)strtol(endereco_str, NULL, 16);

        /* ==========================
           INSTRUCOES
        ========================== */

        if (strcmp(tipo, "i") == 0) {

            char *instrucao = strtok(conteudo, " ,");
            char *op1 = strtok(NULL, " ,");
            char *op2 = strtok(NULL, " ,");

            if (instrucao)
                memoria[endereco] = strdup(instrucao);

            if (op1)
                memoria[endereco + 1] = strdup(op1);

            if (op2)
                memoria[endereco + 2] = strdup(op2);
        }

        /* ==========================
           DADOS
        ========================== */

        else if (strcmp(tipo, "d") == 0) {

            memoria[endereco] = strdup(conteudo);
        }
    }

    fclose(arquivo);
}

/* =========================================================
   CONVERTER MEMORIA
========================================================= */

void converter_memoria() {

    int i;

    for(i = 0; i < TAM_MEMORIA; i++) {

        if(memoria[i] == NULL)
            continue;

        char *valor = memoria[i];

        unsigned char op = opcode(valor);

        if(op != 255) {

            memory[i] = op;
            continue;
        }

        unsigned char regis = registrador_codigo(valor);

        if(regis != 255) {

            memory[i] = regis;
            continue;
        }

        memory[i] = (unsigned char)strtol(valor, NULL, 16);
    }
}

/* =========================================================
   EXIBIR CPU
========================================================= */

void exibirCPU() {

    int i;

    printf("\n================ CPU ================\n");

    printf("PC : %u\n", pc);
    printf("IR : %u\n", ir);
    printf("MAR: %u\n", mar);
    printf("IMM: %u\n", imm);

    printf("\nREGISTRADORES\n");

    for(i = 0; i < 8; i++) {

        printf("R%d = %02X\n", i, reg[i]);
    }

    printf("\nFLAGS\n");

    printf("E = %u\n", e);
    printf("L = %u\n", l);
    printf("G = %u\n", g);

    printf("\nMEMORIA\n");

    for(i = 0; i < 64; i++) {

        printf("%02X : %02X  ", i, memory[i]);

       

        printf("\n");
    }

    printf("=====================================\n");
}

/* =========================================================
   FETCH
========================================================= */

void search() {

    mar = pc;

    ir = memory[pc];

    printf("\nFETCH\n");

    printf("PC = %u\n", pc);
    printf("IR = %u\n", ir);
}

/* =========================================================
   DECODE
========================================================= */

void decode() {

    ro0 = 0;
    ro1 = 0;
    imm = 0;

    printf("\nDECODE\n");

    /* ==========================
       REG REG
    ========================== */

    if(ir >= 2 && ir <= 12) {

        ro0 = memory[pc + 1];
        ro1 = memory[pc + 2];
    }

    /* ==========================
       JUMPS
    ========================== */

    else if(ir >= 14 && ir <= 20) {

        mar = memory[pc + 1];
    }

    /* ==========================
       LD ST
    ========================== */

    else if(ir == 21 || ir == 22) {

        ro0 = memory[pc + 1];
        mar = memory[pc + 2];
    }

    /* ==========================
       IMMEDIATE
    ========================== */

    else if(ir >= 23 && ir <= 29) {

        ro0 = memory[pc + 1];
        imm = memory[pc + 2];
    }
}

/* =========================================================
   EXECUTE
========================================================= */

void execute() {

    printf("\nEXECUTE\n");

    switch(ir) {

        case 0:

            printf("HLT\n");
            break;

        case 1:

            pc += 1;
            break;

        /* ==========================
           LDR
        ========================== */

        case 2:

            reg[ro0] = memory[reg[ro1]];
            reg[ro0] <<= 8;
            reg[ro0] |= memory[reg[ro1] + 1];

            pc += 3;
            break;

        /* ==========================
           STR
        ========================== */

        case 3:

            memory[reg[ro1]] = (reg[ro0] >> 8) & 0xFF;
            memory[reg[ro1] + 1] = reg[ro0] & 0xFF;

            pc += 3;
            break;

        case 4:

            reg[ro0] += reg[ro1];

            pc += 3;
            break;

        case 5:

            reg[ro0] -= reg[ro1];

            pc += 3;
            break;

        case 6:

            reg[ro0] *= reg[ro1];

            pc += 3;
            break;

        case 7:

            if(reg[ro1] != 0)
                reg[ro0] /= reg[ro1];

            pc += 3;
            break;

        /* ==========================
           CMP
        ========================== */

        case 8:

            e = 0;
            l = 0;
            g = 0;

            if(reg[ro0] == reg[ro1])
                e = 1;

            else if(reg[ro0] < reg[ro1])
                l = 1;

            else
                g = 1;

            pc += 3;
            break;

        case 9:

            reg[ro0] = reg[ro1];

            pc += 3;
            break;

        case 10:

            reg[ro0] &= reg[ro1];

            pc += 3;
            break;

        case 11:

            reg[ro0] |= reg[ro1];

            pc += 3;
            break;

        case 12:

            reg[ro0] ^= reg[ro1];

            pc += 3;
            break;

        case 13:

            reg[ro0] = ~reg[ro0];

            pc += 2;
            break;

        /* ==========================
           JE
        ========================== */

        case 14:

            if(e == 1)
                pc = mar;
            else
                pc += 2;

            break;

        case 15:

            if(e == 0)
                pc = mar;
            else
                pc += 2;

            break;

        case 16:

            if(l == 1)
                pc = mar;
            else
                pc += 2;

            break;

        case 17:

            if(l == 1 || e == 1)
                pc = mar;
            else
                pc += 2;

            break;

        case 18:

            if(g == 1)
                pc = mar;
            else
                pc += 2;

            break;

        case 19:

            if(g == 1 || e == 1)
                pc = mar;
            else
                pc += 2;

            break;

        case 20:

            pc = mar;
            break;

        /* ==========================
           LD
        ========================== */

        case 21:

            reg[ro0] = memory[mar];
            reg[ro0] <<= 8;
            reg[ro0] |= memory[mar + 1];

            pc += 3;
            break;

        /* ==========================
           ST
        ========================== */

        case 22:

            memory[mar] = (reg[ro0] >> 8) & 0xFF;
            memory[mar + 1] = reg[ro0] & 0xFF;

            pc += 3;
            break;

        case 23:

            reg[ro0] = imm;

            pc += 3;
            break;

        case 24:

            reg[ro0] += imm;

            pc += 3;
            break;

        case 25:

            reg[ro0] -= imm;

            pc += 3;
            break;

        case 26:

            reg[ro0] *= imm;

            pc += 3;
            break;

        case 27:

            if(imm != 0)
                reg[ro0] /= imm;

            pc += 3;
            break;

        case 28:

            reg[ro0] <<= imm;

            pc += 3;
            break;

        case 29:

            reg[ro0] >>= imm;

            pc += 3;
            break;
    }
}

/* =========================================================
   CLOCK
========================================================= */

void clock_cpu() {

    while(1) {

        printf("\nPRESSIONE ENTER PARA FETCH");
        getchar();

        search();

        exibirCPU();

        printf("\nPRESSIONE ENTER PARA DECODE");
        getchar();

        decode();

        exibirCPU();

        printf("\nPRESSIONE ENTER PARA EXECUTE");
        getchar();

        execute();

        exibirCPU();

        if(ir == 0) {

            printf("\nPROGRAMA FINALIZADO\n");
            break;
        }
    }
}

/* =========================================================
   MAIN
========================================================= */

int main() {

    preenchendo_memoria("programa.txt");

    converter_memoria();

    clock_cpu();

    return 0;
}