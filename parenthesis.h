
#ifndef __PARENTHESIS_H
#define __PARENTHESIS_H

#define STACKINCREAMENT 10
#define STACK_INIT_SIZE 100
#define OVERFLOW -2
#define OK 1
#define ERROR 0

#define WAF_NOT_HIT     0
#define WAF_HIT         1

typedef struct {
    int rule_id;
    int hit;
} waf_hit_t;


typedef struct {
    unsigned char *pos_s;
    unsigned char *pos_e;
    char e;
} selement_t;

typedef struct
{
    selement_t *base;
    selement_t *top;
    int stacksize;
} sqstack_t;

int parenthesis_init (sqstack_t * s);
int parenthesis_match (sqstack_t * s, unsigned char *str, int *rule_ids, int *rule_hits, int rule_size);

#endif


