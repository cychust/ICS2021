def_EHelper(jal) {
  DbgPrintf("op: jal. ddest = %u simm = %d\n", *ddest, id_src1->simm);
  rtl_jal(s, ddest, id_src1->simm);
}

def_EHelper(jrw) {
  DbgPrintf("op: jrw. ddest = %u dsrc1 = %u simm = %d\n", *ddest, *dsrc1,
         id_src2->simm);
  rtl_jalr(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(beq) {
  DbgPrintf("op: beq. dsrc1 = %d dsrc2 = %d simm = %d\n", *dsrc1, *dsrc2,
         id_dest->simm);
  rtl_bran(s, RELOP_EQ, dsrc1, dsrc2, id_dest->simm);
}

def_EHelper(bne) {
  DbgPrintf("op: bne. dsrc1 = %d dsrc2 = %d simm = %d\n", *dsrc1, *dsrc2,
         id_dest->simm);
  rtl_bran(s, RELOP_NE, dsrc1, dsrc2, id_dest->simm);
}


def_EHelper(blt) {
  DbgPrintf("op: blt. dsrc1 = %d dsrc2 = %d simm = %d\n", *dsrc1, *dsrc2,
         id_dest->simm);
  rtl_bran(s, RELOP_LT, dsrc1, dsrc2, id_dest->simm);
}

def_EHelper(bltu) {
  DbgPrintf("op: bltu. dsrc1 = %d dsrc2 = %d simm = %d\n", *dsrc1, *dsrc2,
         id_dest->simm);
  rtl_bran(s, RELOP_LTU, dsrc1, dsrc2, id_dest->simm);
}


def_EHelper(bge) {
  DbgPrintf("op: bge. dsrc1 = %d dsrc2 = %d simm = %d\n", *dsrc1, *dsrc2,
         id_dest->simm);
  rtl_bran(s, RELOP_GE, dsrc1, dsrc2, id_dest->simm);
}

def_EHelper(bgeu) {
  DbgPrintf("op: bgeu. dsrc1 = %d dsrc2 = %d simm = %d\n", *dsrc1, *dsrc2,
         id_dest->simm);
  rtl_bran(s, RELOP_GEU, dsrc1, dsrc2, id_dest->simm);
}

