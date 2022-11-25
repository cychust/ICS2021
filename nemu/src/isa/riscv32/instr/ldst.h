def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->simm, 4);
  DbgPrintf("op: lw ddest = %d\n", *ddest);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4); // todo
  DbgPrintf("op end: sw ddest = %d\n", *ddest);
}

def_EHelper(sh) {
  DbgPrintf("op: sh\n");
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 2);
}

def_EHelper(sb) {
  DbgPrintf("op: sb\n");
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 1);
}


def_EHelper(lh) {
  DbgPrintf("op: lh\n");
  rtl_lms(s, ddest, dsrc1, id_src2->simm, 2);
}

def_EHelper(lhu) {
  DbgPrintf("op: lhu\n");
  rtl_lm(s, ddest, dsrc1, id_src2->simm, 2);
}

def_EHelper(lbu) {
  DbgPrintf("op: lbu\n");
  rtl_lm(s, ddest, dsrc1, id_src2->simm, 1);
}