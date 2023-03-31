#pragma once

#include <ultra64.h>

#include "types.h"


#define INSN_ID_1(opcode, rs, rt, rd, shift, function, paramType, name) \
    {{  opcode,      rs,      rt,      rd,   shift, function}, paramType, name     },

#ifdef DISASM_INCLUDE_ALL_INSTRUCTIONS
    #define INSN_ID_0(opcode, rs, rt, rd, shift, function, paramType, name) INSN_ID_1(opcode, rs, rt, rd, shift, function, paramType, name)
#else
    #define INSN_ID_0(opcode, rs, rt, rd, shift, function, paramType, name)
#endif

// Special opcodes
enum SpecialOpcodes {
    OPC_SPEC = 0b000000,
    OPC_REGI = 0b000001,
    OPC_COP0 = 0b010000,
    OPC_COP1 = 0b010001,
    OPC_COP2 = 0b010010,
    OPC_COP3 = 0b010011,
};

enum InsnParamTypes {
    PARAM_NOP, // NOP
    PARAM_N,   //
    PARAM_SYS, //
    PARAM_SYN, //
    PARAM_S,   // rs
    PARAM_T,   // rt
    PARAM_D,   // rd
    PARAM_ST,  // rs, rt
    PARAM_ST2, // rs, rt
    PARAM_DS,  // rd, rs
    PARAM_TD,  // rt, rd
    PARAM_SD,  // rs, rd
    PARAM_STD, // rs, rt, rd
    PARAM_SDT, // rs, rd, rt
    PARAM_DST, // rd, rs, rt
    PARAM_DTS, // rd, rt, rs
    PARAM_DTA, // rd, rt, shift
    PARAM_SI,  // rs, 0xI
    PARAM_TI,  // rt, 0xI
    PARAM_STI, // rs, rt, 0xI
    PARAM_TSI, // rt, rs, 0xI
    PARAM_TOS, // rt, 0xI(rs)
    PARAM_SO,  // rs, offset
    PARAM_STO, // rs, rt, offset
    PARAM_O,   // offset
    PARAM_J,   // func
    PARAM_FOS, // ft, 0xI(rs)
    PARAM_TFS, // rt, fs
    PARAM_FF,  // fd, fs
    PARAM_FFF, // fd, fs, ft
    PARAM_CON, // fs, ft
    PARAM_BC1, // offset
    PARAM_UNK, // unimpl
};

// Instruction data
typedef union {
    struct PACKED { // Instruction struct
        /*0x00*/ union { // Upper 16 bits of instruction
                    struct PACKED {
                        /*0x00*/ u16 opcode : 6;
                        /*0x00*/ u16 rs     : 5;
                        /*0x01*/ u16 rt     : 5;
                    };
                    struct PACKED { // Floating point operations
                        /*0x00*/ u16 COP1   : 6;
                        /*0x00*/ u16 fmt    : 5;
                        /*0x01*/ u16 ft     : 5;
                    };
                };
        /*0x02*/ union { // Lower 16 bits of instruction
                    struct PACKED {
                        /*0x00*/ u16 rd       : 5;
                        /*0x00*/ u16 sa       : 5;
                        /*0x01*/ u16 function : 6;
                    }; /*0x02*/
                    struct PACKED { // Floating point operations
                        /*0x00*/ u16 fs     : 5;
                        /*0x00*/ u16 fd     : 5;
                        /*0x01*/ u16 MOV    : 6;
                    }; /*0x02*/
                    u16 immediate;
                    u16 offset;
                };
    }; /*0x04*/
    u32 raw;
} InsnData; /*0x04*/

// Instruction database format
typedef struct PACKED {
    /*0x00*/ InsnData i;
    /*0x04*/ s32 paramType;
    /*0x08*/ char name[8];
} InsnTemplate; /*0x10*/

typedef struct PACKED {
    /*0x00*/ InsnData mask;
    /*0x04*/ const char *paramStr;
} InsnParamType; /*0x08*/

// sDisasmColors
enum DisasmColors {
    DISASM_COLOR_NOP,
    DISASM_COLOR_INSN_NAME,
    DISASM_COLOR_REG,
    DISASM_COLOR_REG_2,
    DISASM_COLOR_IMMEDIATE,
    DISASM_COLOR_ADDRESS,
    DISASM_COLOR_OFFSET,
};

// fmt_to_char
enum FormatCodes {
    FMT_SINGLE     = 16,
    FMT_DOUBLEWORD = 17,
    FMT_WORD       = 20,
    FMT_LONG       = 21,
};


#define DISASM_STEP (s32)sizeof(InsnData)

#define INSN_OFFSET(addr, offset) ((addr) + (sizeof(InsnData) * (s16)(offset)))


extern const RGBA32 sDisasmColors[];


s16 get_branch_offset(InsnData insn);
uintptr_t get_branch_target_from_addr(uintptr_t addr);
char *insn_disasm(InsnData insn, const char **fname, _Bool showDestNames);
