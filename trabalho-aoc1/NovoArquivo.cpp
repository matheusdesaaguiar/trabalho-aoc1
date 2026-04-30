#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINHA 256
#define TAM_MEMORIA 256

unsigned char* memoria[TAM_MEMORIA];
unsigned char memory[TAM_MEMORIA];

unsigned int mbr;
unsigned short int mar, pc = 0;
unsigned char ir;


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
    if (strcmp(instrucao, "not") == 0) return 13;i
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

unsigned char registrador_codigo(char *reg) {
    if (strcmp(reg, "r0") == 0) return 0;
    if (strcmp(reg, "r1") == 0) return 1;
    if (strcmp(reg, "r2") == 0) return 2;
    if (strcmp(reg, "r3") == 0) return 3;
    if (strcmp(reg, "r4") == 0) return 4;
    if (strcmp(reg, "r5") == 0) return 5;
    if (strcmp(reg, "r6") == 0) return 6;
    if (strcmp(reg, "r7") == 0) return 7;

    return 255;
}

void preenchendo_memoria(char *nome_arquivo) {
    FILE *arquivo;
    char linha[TAM_LINHA];

    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    while (fgets(linha, TAM_LINHA, arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';

        char *endereco_str = strtok(linha, ";");
        char *tipo = strtok(NULL, ";");
        char *conteudo = strtok(NULL, "\n");

        if (!endereco_str || !tipo || !conteudo)
            continue;

        int endereco = atoi(endereco_str);

        if (strcmp(tipo, "i") == 0) {
            char *instrucao = strtok(conteudo, " ,");
            char *registrador = strtok(NULL, " ,");
            char *valor = strtok(NULL, " ,");

            if (instrucao)
                memoria[endereco] = (unsigned char*)strdup(instrucao);

            if (registrador)
                memoria[endereco + 1] = (unsigned char*)strdup(registrador);

            if (valor)
                memoria[endereco + 2] = (unsigned char*)strdup(valor);

            if (instrucao && strcmp(instrucao, "hlt") == 0) {
                break;
            }

        } else if (strcmp(tipo, "d") == 0) {
            memoria[endereco] = (unsigned char*)strdup(conteudo);
        }
    }

    fclose(arquivo);
}


void converter_memoria() {
    for (int i = 0; i < TAM_MEMORIA; i++) {

        if (memoria[i] == NULL) continue;

        char *valor = (char*)memoria[i];

        unsigned char op = opcode(valor);
        if (op != 255) {
            memory[i] = op;
            continue;
        }

        unsigned char reg = registrador_codigo(valor);
        if (reg != 255) {
            memory[i] = reg;
            continue;
        }

        memory[i] = (unsigned char)atoi(valor);
    }
}



void print_bin(unsigned char valor) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (valor >> i) & 1);
    }
}


void exibir_memoria() {
    for (int i = 0; i < TAM_MEMORIA; i++) {
        if (memoria[i] != NULL) {
            printf("memoria[%d] = %s\n", i, memoria[i]);
        }
    }
}

void exibir_memory_completo() {
    printf("\nMemoria (DEC | HEX | BIN):\n");

    for (int i = 0; i < 30; i++) {
        printf("memory[%d] = %3d | %02X | ", i, memory[i], memory[i]);
        print_bin(memory[i]);
        printf("\n");
    }
}



void search() {

    mar = pc;
    mbr = memory[mar];

    ir = mbr;

    if(ir == 0 || ir == 1 || ir == 13) {
        mbr = mbr << 16;
    } 
    else if(ir >= 2 && ir <= 29) {
        mar++;
        mbr = (mbr << 8) | memory[mar];
        mar++;
        mbr = (mbr << 8) | memory[mar];
    }

    printf("\nMBR: %08X\n", mbr);
}


int main() {

    preenchendo_memoria("testee.txt");

    exibir_memoria();

    converter_memoria();

    exibir_memory_completo();

    search();

    return 0;
}