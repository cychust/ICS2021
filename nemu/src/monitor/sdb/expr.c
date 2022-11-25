#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256,
  TK_EQ,
  TK_AND,
  TK_DEC,
  TK_NEQ,
  TK_DEREF,  // derefence
  TK_NEG,    // negative
  TK_REG,    // reg
  TK_HEX,    // hex
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */
    {"0x[0-9a-fA-F]+", TK_HEX},  // hex
    {"0|[1-9][0-9]*", TK_DEC},   // dec
    {"\\+", '+'},                // plus
    {"-", '-'},                  // minus or negative symbol
    {"\\*", '*'},                // * or derefence
    {"\\/", '/'},                //
    {"\\(", '('},                //
    {"\\)", ')'},                //
    {" +", TK_NOTYPE},           // spaces
    {"&&", TK_AND},              //
    {"==", TK_EQ},               // equal
    {"!=", TK_NEQ},              //
    {"\\$(0|(ra)|(sp)|(gp)|(tp)|(t[0-6])|(s[0-11])|(a[0-7]))", TK_REG},  //
};

// const char *regs[] = {
//   "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
//   "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
//   "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
//   "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
// };

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;
  memset(tokens, 0, sizeof(tokens));

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 &&
          pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        if (substr_len > 32) {
          Log("token len is more than 32.");
          return false;
        }
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i,
            rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        if (rules[i].token_type != TK_NOTYPE) {
          tokens[nr_token].type = rules[i].token_type;
          for (int i = 0; i < substr_len; i++) {
            /* code */
            tokens[nr_token].str[i] = substr_start[i];
          }
          // Log("tokens: pos: %d -> %s", nr_token, tokens[nr_token].str);
          nr_token++;
        }

        switch (rules[i].token_type) {
          default:
            // TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p, int q) {
  if (q == p + 1 || tokens[p].type != '(' || tokens[q].type != ')') {
    return false;
  }
  int left = 0;
  int right = 0;
  for (int i = p + 1; i <= q - 1; i++) {
    /* code */
    if (tokens[i].type == '(') {
      left++;
    }
    if (tokens[i].type == ')') {
      right++;
    }
    if (left < right) {
      return false;
    }
  }
  return left == right;
}

int posOfMainOperate(int p, int q) {
  if (tokens[p].type != '(') {
    /* code */
    return p + 1;
  } else {
    int left = 0;
    int right = 0;
    for (int i = p; i <= q; i++) {
      /* code */
      if (tokens[i].type == '(') {
        left++;
      }
      if (tokens[i].type == ')') {
        /* code */
        right++;
      }
      if (left == right) {
        return i + 1;
      }
    }
    return -1;
  }
}

word_t eval(int p, int q) {
  // Log("eval p: %d, q: %d", p, q);
  if (p > q) {
    // bad expression
    assert(-2);
  } else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if (tokens[p].type == TK_REG) {
      bool success = false;
      return isa_reg_str2val(tokens[p].str, &success);
    }
    if (tokens[p].type == TK_HEX) {
      char h[32];
      memccpy(h, tokens[p].str + 2, 1, 30);
      word_t res = strtol(h, NULL, 16);
      return res;
    }
    return atoi(tokens[p].str);
  } else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  } else {
    if (q - p == 1) {
      if (tokens[p].type == TK_NEG) {
        return -atoi(tokens[q].str);
      } else if (tokens[p].type == TK_DEREF) {
        /* code */
        TODO();
      }
      assert(-1);  // error
    }
    word_t val1;
    word_t val2;
    // op = the position of 主运算符 in the token expression;
    int op = posOfMainOperate(p, q);
    if (op == -1) {
      assert(-1);
    }
    Log("op: pos = %d, type = %d", op, tokens[op].type);
    val1 = eval(p, op - 1);
    val2 = eval(op + 1, q);

    int op_type = tokens[op].type;
    switch (op_type) {
      case '+':
        return val1 + val2;
      case '-': /* ... */
        return val1 - val2;
      case '*': /* ... */
        return val1 * val2;
      case '/': /* ... */
        return val1 / val2;
      case TK_EQ:
        return val1 == val2 ? 0 : -1;
      case TK_AND:
        return val1 && val2;
      default:
        assert(0);
    }
  }
  assert(-1);
  return -1;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();
  for (int i = 0; i < nr_token; i++) {
    /* code */
    if (tokens[i].type == '*' &&
        (i == 0 ||
         (tokens[i - 1].type == '(' || tokens[i - 1].type == '+' ||
          tokens[i - 1].type == '-' || tokens[i - 1].type == '*' ||
          tokens[i - 1].type == '/' || tokens[i - 1].type == TK_AND ||
          tokens[i - 1].type == TK_EQ || tokens[i - 1].type == TK_NEQ))) {
      tokens[i].type = TK_DEREF;
    }
    if (tokens[i].type == '-' &&
        (i == 0 ||
         ((tokens[i - 1].type == '(' || tokens[i - 1].type == '+' ||
           tokens[i - 1].type == '-' || tokens[i - 1].type == '*' ||
           tokens[i - 1].type == '/' || tokens[i - 1].type == TK_AND ||
           tokens[i - 1].type == TK_EQ || tokens[i - 1].type == TK_NEQ)))) {
      tokens[i].type = TK_NEG;
    }
  }

  *success = true;
  return eval(0, nr_token - 1);
}