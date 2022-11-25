def_EHelper(lui) {
  DbgPrintf("op: lui\n");
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(addi) {
  DbgPrintf("op: addi.src1 = %d simm = %d\n", *dsrc1, id_src2->simm);
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
  DbgPrintf("op: addi end. dest = %d\n", *ddest);
}

def_EHelper(andi) {
  DbgPrintf("op: andi.src1 = %d simm = %d\n", *dsrc1, id_src2->simm);
  rtl_andi(s, ddest, dsrc1, id_src2->simm);
  DbgPrintf("op: andi end. dest = %d\n", *ddest);
}


def_EHelper(auipc) {
  DbgPrintf("op: auipc: ddest = %u, imm = %d\n", *ddest, id_src1->imm);
  rtl_auipc(s, ddest, id_src1->imm);
}

def_EHelper(add) {
  DbgPrintf("op: add.src1 = %d src2 = %d\n", *dsrc1, *dsrc2);
  rtl_add(s, ddest, dsrc1, dsrc2);
  DbgPrintf("op: add end. dest = %d\n", *ddest);
}

def_EHelper(sub) {
  DbgPrintf("op: sub. dest = %u src1 = %d src2 = %d\n", *ddest, *dsrc1, *dsrc2);
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(and) {
  DbgPrintf("op: or. dest = %u src1 = %d src2 = %d\n", *ddest, *dsrc1, *dsrc2);
  rtl_and(s, ddest, dsrc1, dsrc2);
}


def_EHelper(or) {
  DbgPrintf("op: or. dest = %u src1 = %d src2 = %d\n", *ddest, *dsrc1, *dsrc2);
  rtl_or(s, ddest, dsrc1, dsrc2);
}

def_EHelper(xor) {
  DbgPrintf("op: xor. dest = %u src1 = %d src2 = %d\n", *ddest, *dsrc1, *dsrc2);
  rtl_xor(s, ddest, dsrc1, dsrc2);
}


def_EHelper(xori) {
  DbgPrintf("op xori. ddest = %d src1 = %d imm = %d\n", *ddest, *dsrc1,
         id_src2->simm);
  rtl_xori(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(ori) {
  DbgPrintf("op ori. ddest = %d src1 = %d imm = %d\n", *ddest, *dsrc1,
         id_src2->simm);
  rtl_ori(s, ddest, dsrc1, id_src2->simm);
}


def_EHelper(mul) {
  DbgPrintf("op mul. ddest = %d src1 = %d src2 = %d\n", *ddest, *dsrc1, *dsrc2);
  rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulh) {
  DbgPrintf("op mulh. ddest = %d src1 = %d src2 = %d\n", *ddest, *dsrc1, *dsrc2);
  rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}


def_EHelper(div) {
  DbgPrintf("op div. ddest = %d src1 = %d src2 = %d\n", *ddest, *dsrc1, *dsrc2);
  rtl_divs_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(rem) {
  DbgPrintf("op rem. ddest = %d src1 = %d src2 = %d\n", *ddest, *dsrc1, *dsrc2);
  rtl_divs_r(s, ddest, dsrc1, dsrc2);
  DbgPrintf("op rem. ddest = %d \n", *ddest);
}
