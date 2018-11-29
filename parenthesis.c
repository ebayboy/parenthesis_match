
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "parenthesis.h"
#include "exp_parser.h"

int parenthesis_init(sqstack_t * s)
{
    s->base = (selement_t *) malloc (STACK_INIT_SIZE * sizeof (selement_t*));
    if (!s->base)
        exit (OVERFLOW);
    s->top = s->base;
    s->stacksize = STACK_INIT_SIZE;
    return OK;
}

#if 0
//获取栈顶元素
__attribute__((unused))
int Gettop (sqstack_t * s, selement_t *e)
{
    if (s->top == s->base)
        return ERROR;
    e = *(s->top - 1);
    return OK;
}
#endif

int push (sqstack_t * s, selement_t *e)
{
    if (s->top - s->base >= s->stacksize) {
        s->base = (selement_t *) realloc (s->base,
            (s->stacksize + STACKINCREAMENT) * sizeof (selement_t));
        if (!s->base) {
            exit (OVERFLOW);
        }
        s->top = s->base + s->stacksize;
        s->stacksize += STACKINCREAMENT;
    }
    *s->top++ = *e;

    return OK;
}

int pop (sqstack_t * s, selement_t ** e)
{
    if (s->top == s->base) {
        return ERROR;
    }

    *e = --s->top;

    return OK;
}

int stackempty (sqstack_t * s)
{
    if (s->top == s->base)
        return OK;
    return ERROR;
}

int clearstack (sqstack_t * s)
{
    if (s->top == s->base)
        return ERROR;
    s->top = s->base;
    return OK;
}

/* TODO ?  */
static int calculate_exp_result(unsigned char *buf, size_t buflen)
{

    return 0;
}

//括号匹配算法
int parenthesis_match (sqstack_t * s, unsigned char *str)
{
    int i = 0, flag = 0;
    selement_t *e;

    while (str[i] != '\0')
    {
        switch (str[i])
        {
            case '(':
                /* e = new e */
                if ((e = malloc(sizeof(selement_t))) == NULL) {
                    return -1;
                }
                memset(e, 0, sizeof(selement_t));
                e->e = '(';
                e->pos_s = str + i;
                push (s, e);
                break;
            case ')':
                {
                    pop (s, &e);
                    e->pos_e = str + i;
                    if (e->e != '(') {
                        flag = 1;
                    } else {
                        char buff[BUFSIZ] = {0};
                        memcpy(buff, e->pos_s, e->pos_e - e->pos_s + 1);
                        fprintf(stderr, "star-end: %d-%d buff:[%s]\n", e->pos_s - str, e->pos_e - str, buff);
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

    if (!flag && stackempty (s))
        printf ("括号匹配成功!\n");
    else
        printf ("括号匹配失败!\n");
    return OK;
}

static int waf_hit_result_find(waf_hit_t hits[10], int rule_id)
{
    int i;

    for (i = 0; i< 10; i++) {
        if (hits[i].rule_id == rule_id) {
            return hits[i].hit;
        }
    }

    return -1;
}

