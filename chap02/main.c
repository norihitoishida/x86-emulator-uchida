#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Memory = 1MB */
#define MEMORY_SIZE (1024 * 1024)

/* General Purpose Registers */
enum Register { 
    EAX, // 0, Accumulator
    ECX, // 1, Count registor
    EDX, // 2, Data registor
    EBX, // 3, Base registor

    ESP, // 4, Stack Pointer
    EBP, // 5, Base Pointer
    ESI, // 6, Source Index
    EDI, // 7, Destination Index
    
    REGISTERS_COUNT, // 8
    };

char* registers_name[] = {
    "EAX", "ECX", "EDX", "EBX", 
    "ESP", "EBP", "ESI", "EDI"
    };

typedef struct {
    uint32_t registers[REGISTERS_COUNT]; // 汎用レジスタ群
    uint32_t eflags; // EFLAGSレジスタ
    uint8_t* memory; // メモリ(バイト列)
    uint32_t eip; // プログラムカウンタ(Instruction Pointer)
} Emulator;

static Emulator* create_emu(size_t size, uint32_t eip, uint32_t esp)
{
    Emulator* emu = malloc(sizeof(Emulator)); // メモリ確保
    emu->memory = malloc(size); // メモリ確保

    memset(emu->registers, 0, sizeof(emu->registers)); // 汎用レジスタ群初期化(全て0)
    
    emu->eip = eip; // プログラムカウンタ初期化
    emu->registers[ESP] = esp; // スタックポインタ初期化
    return emu;
}

static void destroy_emu(Emulator* emu)
{
    free(emu->memory);
    free(emu);
}

/* 汎用レジスタとプログラムカウンタの値を標準出力に出力する */
static void dump_registers(Emulator* emu)
{
    int i;

    for (i = 0; i < REGISTERS_COUNT; i++) {
        printf("%s = %08x\n", registers_name[i], emu->registers[i]);
    }

    printf("EIP = %08x\n", emu->eip);
}

uint32_t get_code8(Emulator* emu, int index)
{
    return emu->memory[emu->eip + index];
}

int32_t get_sign_code8(Emulator* emu, int index)
{
    return (int8_t)emu->memory[emu->eip + index];
}

uint32_t get_code32(Emulator* emu, int index)
{
    int i;
    uint32_t ret = 0;

    /* リトルエンディアンでメモリの値を取得する */
    for (i = 0; i < 4; i++) {
        ret |= get_code8(emu, index + i) << (i * 8);
    }

    return ret;
}

/* 
* 汎用レジスタに即値をコピーするMOV命令
* オペランドが汎用レジスタ指定を含む
*/
void mov_r32_imm32(Emulator* emu)
{
    uint8_t reg = get_code8(emu, 0) - 0xB8;
    uint32_t value = get_code32(emu, 1);
    emu->registers[reg] = value;
    emu->eip += 5;
}

/* 
* 1Byteのメモリ番地を取るJMP命令
* オペランドは1Byteの符号付き整数(-128 ~ 127)
*/
void short_jump(Emulator* emu)
{
    int8_t diff = get_sign_code8(emu, 1);
    emu->eip += (diff + 2);
}

typedef void instruction_func_t(Emulator*);
instruction_func_t* instructions[256];
void init_instructions(void)
{
    int i;
    memset(instructions, 0, sizeof(instructions));
    for (i = 0; i < 8; i++) {
        instructions[0xB8 + i] = mov_r32_imm32;
    }
    instructions[0xEB] = short_jump;
}



int main(int argc, char* argv[]) {
    /* 引数が1コでない場合はエラー */
    if (argc != 2) {
        printf("usage: px86 filename\n"); 
        return 1;
    }

    /*
    * Emulator構造体を作成
    * メモリサイズ=1MB, EIP=0, ESP=0x7C00
    * EIP : プログラムカウンタ, 命令実行毎に確認し0ならばmainループ終了
    * ESP : スタックポインタ
    */ 
    Emulator* emu; 
    emu = create_emu(MEMORY_SIZE, 0x0000, 0x7c00); 

    /*
    * ファイル構造体のポインタを作成
    * 機械語ファイルを読み込む(最大512バイト)
    */
    FILE* binary;
    binary = fopen(argv[1], "rb");
    if (binary == NULL) {
        printf("%sファイルが開けません\n", argv[1]);
        return 1;
    }
    fread(emu->memory, 1, 0x200, binary);
    fclose(binary);

    init_instructions();

    while (emu->eip < MEMORY_SIZE) {
        uint8_t code = get_code8(emu, 0);
        /* 現在のプログラムカウンタと実行されるバイナリを出力する */
        printf("EIP = %X, Code = %02X\n", emu->eip, code);

        if (instructions[code] == NULL) {
            /* 実装されてない命令が来たらVMを終了する */
            printf("\n\nNot Implemented: %x\n", code);
            break;
        }

        /* 命令の実行 */
        instructions[code](emu);

        /* EIPが0になったらプログラム終了 */
        if (emu->eip == 0x00) {
            printf("\n\nend of program.\n\n");
            break;
        }
    }

    dump_registers(emu);
    destroy_emu(emu);
    return 0;
}

