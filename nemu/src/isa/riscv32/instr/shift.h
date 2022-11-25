def_EHelper(sltiu) {
  DbgPrintf("op sltiu. ddest = %d src1 = %u imm = %u\n", *ddest, *dsrc1,
         id_src2->simm);
  rtl_sltiu(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(slli) {
  DbgPrintf("op slli. ddest = %d src1 = %u imm = %u\n", *ddest, *dsrc1,
         id_src2->simm);
  if ((id_src2->simm & 0x20) != 0) {
    DbgPrintf("slli return\n");
    return;
  }
  rtl_slli(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(sll) {
  DbgPrintf("op sll. ddest = %d src1 = %u imm = %u\n", *ddest, *dsrc1,
         id_src2->simm);
  if ((id_src2->simm & 0x20) != 0) {
    DbgPrintf("slli return\n");
    return;
  }
  rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srli) {
  DbgPrintf("op srli. ddest = %d src1 = %u imm = %u\n", *ddest, *dsrc1,
         id_src2->simm);
  if ((id_src2->simm & 0x20) != 0) {
    DbgPrintf("srli return\n");
    return;
  }
  rtl_srli(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(srai) {
  DbgPrintf("op srai. ddest = %d src1 = %u imm = %u\n", *ddest, *dsrc1,
         id_src2->simm);
  if ((id_src2->simm & 0x20) != 0) {
    DbgPrintf("srli return\n");
    return;
  }
  rtl_srai(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(srl) {
  DbgPrintf("op srl. ddest = %d src1 = %u\n", *ddest, *dsrc1);
  rtl_srl(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sra) {
  DbgPrintf("op sra. ddest = %d src1 = %u\n", *ddest, *dsrc1);
  rtl_sra(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slt) {
  DbgPrintf("op slt. ddest = %d src1 = %u src2 = %u\n", *ddest, *dsrc1, *dsrc2);
  rtl_slt(s, ddest, dsrc1, dsrc2);
  DbgPrintf("op end: slt. ddest = %d\n", *ddest);
}

def_EHelper(sltu) {
  DbgPrintf("op sltu. ddest = %d src1 = %u src2 = %u\n", *ddest, *dsrc1, *dsrc2);
  rtl_sltu(s, ddest, dsrc1, dsrc2);
  DbgPrintf("op end: sltu. ddest = %d\n", *ddest);
}
