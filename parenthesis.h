
#ifndef __PARENTHESIS_H
#define __PARENTHESIS_H

#define STACKINCREAMENT 10
#define STACK_INIT_SIZE 100

#define OK 1
#define ERROR 0

#define PSIS_MATCHED        1
#define PSIS_NOT_MATCHED    0

typedef struct {
    int rule_id;
    int hit;
} waf_hit_t;

typedef struct {
    char *pos_s;
    char *pos_e;
    char e;
} selement_t;

typedef struct
{
    selement_t *base;
    selement_t *top;
    int stacksize;
} sqstack_t;

int parenthesis_init (sqstack_t * s);
int parenthesis_match (sqstack_t * s, char *str, int *rule_ids, int *rule_hits, size_t rule_size, int *matched_out);
void parenthesis_fini(sqstack_t * s);

#endif


