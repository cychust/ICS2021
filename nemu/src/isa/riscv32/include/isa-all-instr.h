#include <cpu/decode.h>

#include "../local-include/rtl.h"

#define INSTR_LIST(f)                                                        \
  f(lui) f(lw) f(lh) f(lhu) f(sw) f(sh) f(sb) f(lbu) f(inv) f(addi) f(auipc) \
      f(jal) f(jrw) f(add) f(sub) f(andi) f(and) f(or) f(xor) f(mul) f(mulh) \
          f(div) f(rem) f(ori) f(xori) f(sltiu) f(slli) f(sll) f(srl) f(slt) \
              f(sltu) f(sra) f(srli) f(srai) f(beq) f(bne) f(bgeu) f(blt)    \
                  f(bltu) f(bge) f(nemu_trap)

def_all_EXEC_ID();
