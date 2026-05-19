#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINHA 256
#define TAM_MEMORIA 256

unsigned char* memoria[TAM_MEMORIA];
unsigned char memory[TAM_MEMORIA]={0};

unsigned int mbr;
unsigned short int mar = 0;
unsigned short int pc = 0;
unsigned short int reg[8] = {0};
unsigned short int imm = 0;

unsigned char ir, ro0, ro1;
unsigned char e = 0, l = 0, g = 0;

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

        if (strcmp(tipo, "i") == 0) {

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

        else if (strcmp(tipo, "d") == 0) {

            memoria[endereco] = strdup(conteudo);
        }
    }

    fclose(arquivo);
}

void converter_memoria() {

    int i;

    for (i = 0; i < TAM_MEMORIA; i++) {

        if (memoria[i] == NULL)
            continue;

        char *valor = memoria[i];

        unsigned char op = opcode(valor);

        if (op != 255) {

            memory[i] = op;
            continue;
        }

        unsigned char regis = registrador_codigo(valor);

        if (regis != 255) {

            memory[i] = regis;
            continue;
        }

        memory[i] = (unsigned char)strtol(valor, NULL, 16);
    }
}

void exibirCPU() {

    int i;

    printf("\n================ CPU ================\n");

    printf("PC : %02X\n", pc);
    printf("IR : %02X\n", ir);
    printf("MAR: %02X\n", mar);
    printf("MBR: %02X\n", mbr);
    printf("IMM: %02X\n", imm);

    printf("\nREGISTRADORES\n");

    for(i = 0; i < 8; i++) {

        printf("R%d: %04X\n", i, reg[i]);
    }

    printf("\nFLAGS\n");

    printf("E: %d\n", e);
    printf("L: %d\n", l);
    printf("G: %d\n", g);

    printf("\nMEMORIA\n");

    for(i = 0; i < 256; i++) {

        printf("%02X: %02X  ", i, memory[i]);

        if((i + 1) % 8 == 0)
            printf("\n");
    }
}

void search() {

    mar = pc;

    ir = memory[pc];

    printf("\nFETCH\n");

    printf("Opcode carregado: %02X\n", ir);
}

void decode() {

    printf("\nDECODE\n");

    ro0 = 0;
    ro1 = 0;
    imm = 0;

    // instrucoes com 2 registradores
    if((ir >= 2 && ir <= 12) && ir != 13) {

        ro0 = memory[pc + 1];
        ro1 = memory[pc + 2];
    }

    // NOT
    else if(ir == 13) {

        ro0 = memory[pc + 1];
    }

    // jumps
    else if(ir >= 14 && ir <= 20) {

        mar = memory[pc + 1];
    }

    // LD e ST
    else if(ir == 21 || ir == 22) {

        ro0 = memory[pc + 1];
        mar = memory[pc + 2];
    }

    // imediatas
    else if(ir >= 23 && ir <= 29) {

        ro0 = memory[pc + 1];
        imm = memory[pc + 2];
    }

    printf("RO0: %u\n", ro0);
    printf("RO1: %u\n", ro1);
    printf("MAR: %02X\n", mar);
    printf("IMM: %02X\n", imm);
}
void execute() {

    printf("\nEXECUTE\n");

    // HLT
    if(ir == 0) {

        printf("HLT\n");
        return;
    }

    // NOP
    if(ir == 1) {

        pc += 1;
        return;
    }

    // LDR
    if(ir == 2) {

        reg[ro0] = memory[reg[ro1]];

        pc += 3;
        return;
    }

    // STR
    if(ir == 3) {

        memory[reg[ro1]] = reg[ro0];

        pc += 3;
        return;
    }

    // ADD
    if(ir == 4) {

        reg[ro0] += reg[ro1];

        pc += 3;
        return;
    }

    // SUB
    if(ir == 5) {

        reg[ro0] -= reg[ro1];

        pc += 3;
        return;
    }

    // MUL
    if(ir == 6) {

        reg[ro0] *= reg[ro1];

        pc += 3;
        return;
    }

    // DIV
    if(ir == 7) {

        if(reg[ro1] == 0) {

            printf("ERRO DIVISAO POR ZERO\n");
            return;
        }

        reg[ro0] /= reg[ro1];

        pc += 3;
        return;
    }

    // CMP
    if(ir == 8) {

        e = 0;
        l = 0;
        g = 0;

        if(reg[ro0] > reg[ro1])
            g = 1;

        if(reg[ro0] < reg[ro1])
            l = 1;

        if(reg[ro0] == reg[ro1])
            e = 1;

        pc += 2;
        return;
    }

    // MOVR
    if(ir == 9) {

        reg[ro0] = reg[ro1];

        pc += 3;
        return;
    }

    // AND
    if(ir == 10) {

        reg[ro0] &= reg[ro1];

        pc += 3;
        return;
    }

    // OR
    if(ir == 11) {

        reg[ro0] |= reg[ro1];

        pc += 3;
        return;
    }

    // XOR
    if(ir == 12) {

        reg[ro0] ^= reg[ro1];

        pc += 3;
        return;
    }

    // NOT
    if(ir == 13) {

        reg[ro0] = ~reg[ro0];

        pc += 2;
        return;
    }

    // JE
    if(ir == 14) {

        if(e)
            pc = mar;
        else
            pc += 2;

        return;
    }

    // JNE
    if(ir == 15) {

        if(!e)
            pc = mar;
        else
            pc += 2;

        return;
    }

    // JL
    if(ir == 16) {

        if(l)
            pc = mar;
        else
            pc += 2;

        return;
    }

    // JLE
    if(ir == 17) {

        if(l || e)
            pc = mar;
        else
            pc += 2;

        return;
    }

    // JG
    if(ir == 18) {

        if(g)
            pc = mar;
        else
            pc += 2;

        return;
    }

    // JGE
    if(ir == 19) {

        if(g || e)
            pc = mar;
        else
            pc += 2;

        return;
    }

    // JMP
    if(ir == 20) {

        pc = mar;
        return;
    }

    // LD
    if(ir == 21) {

        reg[ro0] = memory[mar];

        pc += 3;
        return;
    }

    // ST
    if(ir == 22) {

        memory[mar] = reg[ro0];

        pc += 3;
        return;
    }

    // MOVI
    if(ir == 23) {

        reg[ro0] = imm;

        pc += 3;
        return;
    }

    // ADDI
    if(ir == 24) {

        reg[ro0] += imm;

        pc += 3;
        return;
    }

    // SUBI
    if(ir == 25) {

        reg[ro0] -= imm;

        pc += 3;
        return;
    }

    // MULI
    if(ir == 26) {

        reg[ro0] *= imm;

        pc += 3;
        return;
    }

    // DIVI
    if(ir == 27) {

        if(imm == 0) {

            printf("ERRO DIVISAO POR ZERO\n");
            return;
        }

        reg[ro0] /= imm;

        pc += 3;
        return;
    }

    // LSH
    if(ir == 28) {

        reg[ro0] <<= imm;

        pc += 3;
        return;
    }

    // RSH
    if(ir == 29) {

        reg[ro0] >>= imm;

        pc += 3;
        return;
    }

    printf("OPCODE INVALIDO\n");
}
int main() {

    preenchendo_memoria("programa.txt");

    converter_memoria();

    ir = 255;

    while(ir != 0) {

        exibirCPU();

        printf("\nPRESSIONE ENTER PARA FETCH");
        getchar();

        search();

        printf("\nPRESSIONE ENTER PARA DECODE");
        getchar();

        decode();

        printf("\nPRESSIONE ENTER PARA EXECUTE");
        getchar();

        execute();
    }

    exibirCPU();

    printf("\nPROGRAMA FINALIZADO\n");

    return 0;
}