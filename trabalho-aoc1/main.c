#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINHA 256
#define TAM_MEMORIA 256

unsigned char* memoria[TAM_MEMORIA];
unsigned char memory[TAM_MEMORIA];

unsigned int mbr;
unsigned short int mar, pc = 0, reg[8], imm;
unsigned char ir, ro0, ro1, e, l, g;


// mapeando os opcodes e designindo valores em decimal
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

// Mapeando registradores e designando seus valores em decimal
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
// Lendo o arquivo texto e separando as partes em tokens
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
		// Separa o primeiro token opara o enderco da memoria
		// O segundo para o tipo. (instrucao ou dado)
		// O terceiro para o conteudo.
        char *endereco_str = strtok(linha, ";");
        char *tipo = strtok(NULL, ";");
        char *conteudo = strtok(NULL, "\n");

        if (!endereco_str || !tipo || !conteudo)
            continue;
		// Endereco ja sera armazenado em decimal
        int endereco = (int)strtol(endereco_str, NULL, 16);

        if (strcmp(tipo, "i") == 0) {
            char *instrucao = strtok(conteudo, " ,");
            char *registrador = strtok(NULL, " ,");
            char *valor = strtok(NULL, " ,");
            // Armazena na memoria global cada dado a partir do endereco coletado ate ocupatr todos os lugaeres da memoria possiveis.

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


// Converte os dados que estao armazendaos ainda no tipo char em dados decimais.
void converter_memoria() {
	int i;
    for ( i = 0; i < TAM_MEMORIA; i++) {

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
        memory[i] = (unsigned char)strtol(valor, NULL, 16);
    }
}

// Aqui mostra a memoria global com os dados retirados do arquivo texto. ex: add, r0, etc

void exibir_memoria() {
    int i;
	for (i = 0; i < TAM_MEMORIA; i++) {
        if (memoria[i] != NULL) {
            printf("memoria[%d] = %s\n", i, memoria[i]);
        }
    }
}
// Esta funcao copia os dados da memoria global para a MEMORY usada na cpu
void exibir_memory_hex() {
    printf("\nMemoria em HEX:\n");
    int i;
    for (i = 0; i < 30; i++) {
        printf("memory[%d] = %08b\n", i, memory[i]); // Aqui deve ser apresentado os dados em binario mas so conseguir ser  mostrado em hexa
    }
}


void exibirCPU() {
    printf("CPU:\n");
    printf("R0: %x\t R1: %x\t R2: %x\t R3: %x\n", reg[0], reg[1], reg[2], reg[3]);
    printf("R4: %d\t R5: %x\t R6: %x\t R7: %x\n", reg[4], reg[5], reg[6], reg[7]);
    printf("MBR: %x\t MAR: %x\t IMM: %x\t PC: %x\n", mbr, mar, imm, pc);
    printf("IR: %x\t RO0: %u\t RO1: %u\n", ir, ro0, ro1);
    printf("E: %u\t L: %u\t G: %u\n", e, l, g);
    printf("Memoria:\n");

    for (int i = 0; i < 256; i++){
        printf("%02d: %0x%02d ",i, memory[i]);
        if ((i + 1) % 5 == 0 && i != 256)
        {
            printf("\n");
        }
    }
    printf("\n\nPressione Enter para iniciar o proximo ciclo de maquina ou aperte CTRL+C para finalizar a execucao do trabalho");
}

void search(){
    mar = pc;
    // 0000 0000 0000 0000 0000 0000 0000 0000
    mbr = memory[mar];
    // 0000 0000 0000 0000 0000 0000 0000 0000
    printf("%024b\n", mbr);

    ir = mbr;
    printf("%i\n", ir);
    ir = mbr >> 3;

    // hlt nop not
    if(ir == 0 || ir == 1 || ir == 13) {
        mbr = mbr << 8;
        mbr = mbr << 8;
    } else if(ir >=2 && ir <=12) {
        mbr = mbr << 8;
        mar ++;
        mbr = mbr << 8;
        mbr = mbr | memory[mar];
        mbr = mbr <<8;
        mbr = mbr | memory[mar];
        mbr = mbr <<3;
    } else if(ir >= 14 && ir <=20){
        mbr = mbr << 11;
        mar++;
        mbr = mbr << 8;
        mbr = mbr | memory[mar];
        mar++;
        mbr = mbr | memory[mar];
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
    printf("%024b\n",mbr);

}

void decode()   {
    if (ir == 0) return;
    if (ir == 1)return pc++;
    if (ir == 13) {
        ro0 = mbr >>11;
        ro0 = ro0 >>5;
        return;
    }
    if(ir >=2 && ir <=12) {
        ro0 = mbr >>11;
        ro0 = ro0 >>5;
        ro1 = mbr >>8;
        ro1 = ro1 >>5;
        return;
    }
    if (ir >= 14 && ir <=20) {
        mar = mbr;
        return;
    }
    if (ir >=21 && ir<=29) {
        ro0 = mbr >>11;
        ro0 = ro0 >>5;
        if(ir == 21 || ir == 22) {
           mar = mbr;
           return;
        }
        if(ir >=23 && ir <=29) {
            imm = mbr;
            return;
        }

    }

}

void execute() {
    if(ir == 0) return;
    if(ir == 1) return pc++;
    if(ir == 2){ // ld
        reg[ro0] = memory[ro1];
        reg[ro0] = reg[ro0] <<8;
        reg[ro0] = memory[ro1+1];
        return;
    }
    if( ir == 3 ) {
        memory[ro1] = reg[ro0] >> 8;
        memory[ro1+1] = reg[ro0];
        return;
    }
    if( ir == 4) { //add
        reg[ro0] = reg[ro0] +reg[ro1];
        return;
    }
    if( ir == 5) {
        reg[ro0] = reg[ro0] - reg[ro1];
        return;
    }
    if( ir == 6) {
        reg[ro0] = reg[ro0] *reg[ro1];
        return;
    }
    if( ir == 7 ) {
        reg[ro0] = reg[ro0] / reg[ro1];
        return;
    }
    if( ir == 8) {
        if(reg[ro0] > reg[ro1]) return g = 1;
        if(reg[ro0] < reg[ro1]) return l = 1;
        if(reg[ro0] == reg[ro1]) return e = 1;
    }
    if( ir == 9) { //movr
        reg[ro0] = reg[ro1];
        return;
    }
    if( ir == 10) { // and
        reg[ro0] = reg[ro0] && reg[ro1];
        return;
    }
    if( ir == 11) { // or
        reg[ro0] = reg[ro0] || reg[ro1];
        return;
    }

    if( ir == 12){ // xor
        reg[ro0] = reg[ro0]^ reg[ro1];
    }
    if(ir == 13) { //not
        reg[ro0] = ~reg[ro0];
        return;
    }
    if( ir == 14 ){ // je
        if(e == "1"){
            pc = mar;
        }
        return;
    }
    if (ir == 15) { //jne
        if( e =="0") {
            pc = mar;
        }
        return;
    }
    if(ir == 16) { // jl
        if( l == "1") {
            pc = mar;
        }
        return;
    }
    if( ir == 17) { // jle
        if(l == "1" || e == "1") {
            pc = mar;
        }
        return;
    }
    if(ir == 18) { //jg
        if(g == "1") pc = mar;
        return;
    }
    if( ir == 19) { // jge
        if( g == "1" || e == "1") pc = mar;
        return;
    }
    if ( ir == 20){ // jmp
        pc = mar;
        return;
    }
    if( ir == 21) { // ld
        reg[ro0] = memory[mar];
        reg[ro0] = reg[ro0] << 8;
        reg[ro0] = memory[mar++];
        return;
    }
    if( ir == 22) { // st
        memory[mar] = reg[ro0]>>8;
        memory[mar++] = reg[ro0];
        return;
    }
    if( ir == 23 ){ //movi
        reg[ro0] = imm;
        return;
    }
    if( ir == 24) { //addi
        reg[ro0] += imm;
        return;
    }
    if( ir == 25) { //subi
        reg[ro0] -= imm;
        return;
    }
    if( ir == 26) { //muli
        reg[ro0] *= imm;
        return;
    }
    if( ir == 27){ //divi
        reg[ro0] /= imm;
    }
    if( ir == 28) {// lsh
        reg[ro0] = reg[ro0] << imm;
        return;
    }
    if(ir == 29) {//rsh
        reg[ro0] = reg[ro0] >> imm;
        return;
    }
}


int main() {
    preenchendo_memoria("programa.txt");
    converter_memoria();
    fflush(stdout);
    exibirCPU();
    getchar();
    while(1) {
        search();
        decode();
        execute();
        exibirCPU();
        getchar();
    }

    return 0;
}
