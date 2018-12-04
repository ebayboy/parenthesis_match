
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "parenthesis.h"
#include "exp_parser.h"

int parenthesis_init(sqstack_t * s)
{
    s->base = (selement_t *) malloc (STACK_INIT_SIZE * sizeof (selement_t));
    if (!s->base) {
        return PSIS_ERROR;
    }

    s->top = s->base;
    s->stacksize = STACK_INIT_SIZE;

    return PSIS_OK;
}

void parenthesis_fini(sqstack_t * s)
{
    if (s->base == NULL) {
        return ;
    }

    free(s->base);
    s->base = NULL;
}

int push (sqstack_t * s, selement_t *e)
{
    if (s->top - s->base >= s->stacksize) {
        s->base = (selement_t *) realloc (s->base,
            (s->stacksize + STACKINCREAMENT) * sizeof (selement_t));
        if (!s->base) {
            return PSIS_ERROR;
        }
        s->top = s->base + s->stacksize;
        s->stacksize += STACKINCREAMENT;
    }
    memcpy(s->top++, e, sizeof(selement_t));

    return PSIS_OK;
}

int pop (sqstack_t * s, selement_t * e)
{
    if (s->top == s->base) {
        return PSIS_ERROR;
    }

    memcpy(e, --s->top, sizeof(selement_t));

    return PSIS_OK;
}

int stackempty (sqstack_t * s)
{
    if (s->top == s->base)
        return PSIS_OK;
    return PSIS_ERROR;
}

int clearstack (sqstack_t * s)
{
    if (s->top == s->base) {
        return PSIS_ERROR;
    }
    s->top = s->base;

    return PSIS_OK;
}

static int calculate_exp_result(char *start, size_t slen, int *rule_ids, int *rule_hits, size_t rule_size)
{
    if (start == NULL || slen == 0 || rule_ids == NULL 
            || rule_hits == NULL || rule_size == 0) {
        fprintf(stderr, "Error %s:%d input error!\n", __func__, __LINE__);
        return -1;
    }

    return exp_parser_parse(start, slen, rule_ids, rule_hits, rule_size);
}

//括号匹配算法
int parenthesis_match (sqstack_t * s, char *str, int *rule_ids, int *rule_hits, size_t rule_size, int *matched_out)
{
    int i = 0, flag = 0;
    selement_t e;

    if (s == NULL || str == NULL || rule_ids == NULL 
            || rule_hits == NULL || rule_size == 0 || matched_out == NULL) {
        return -1;
    }

    while (str[i] != '\0')
    {
        switch (str[i])
        {
            case '(':
                /* e = new e */
                memset(&e, 0, sizeof(selement_t));
                e.e = '(';
                e.pos_s = str + i;
                push (s, &e);
                break;
            case ')':
                {
                    memset(&e, 0, sizeof(selement_t));
                    pop (s, &e);

                    e.pos_e = str + i;
                    if (e.e != '(') {
                        flag = 1;
                    } else {
                        fprintf(stderr, "before star-end: %d-%d buff:[%.*s]\n", 
                                (int)(e.pos_s - str), (int)(e.pos_e - str),
                                (int)(e.pos_e - e.pos_s + 1), e.pos_s);

                        calculate_exp_result(e.pos_s + 1, e.pos_e - e.pos_s - 1,
                                rule_ids, rule_hits, rule_size);

                        if (*e.pos_s == '(') {
                            *e.pos_s = ' ';
                        } 
                        if (*e.pos_e == ')') {
                            *e.pos_e = ' ';
                        } 
                        fprintf(stderr, "after star-end: %d-%d buff:[%.*s]\n\n", 
                                (int)(e.pos_s - str), (int)(e.pos_e - str), 
                                (int)(e.pos_e - e.pos_s + 1), e.pos_s);
                    }
                }
                break;
            default:
                break;
        }
        if (flag)
            break;
        i++;
    }

    if (!flag && stackempty (s)) {
        printf ("括号匹配成功! before buff:[%s]\n", str);
        calculate_exp_result(str, strlen(str), rule_ids, rule_hits, rule_size);
        printf ("括号匹配成功! after buff:[%s]\n", str);
    } else {
        printf ("括号匹配失败!\n");
    }

    if (atoi(str)  == 1) {
        *matched_out = PSIS_MATCHED;
    } else {
        *matched_out = PSIS_NOT_MATCHED;
    }

    return PSIS_OK;
}

