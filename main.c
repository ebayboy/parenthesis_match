
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "common.h"

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

static int waf_fread_file(const char *filename, unsigned char **buf, int *buflen);


typedef int status;
//typedef char selement_t;

typedef struct {
    unsigned char *pos_s;
    unsigned char *pos_e;
    char e;
} selement_t;

typedef struct
{
    selement_t *base;
    selement_t *top;
    status stacksize;
} sqstack_t;

status Init (sqstack_t * s)
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
status Gettop (sqstack_t * s, selement_t *e)
{
    if (s->top == s->base)
        return ERROR;
    e = *(s->top - 1);
    return OK;
}
#endif

status push (sqstack_t * s, selement_t *e)
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

status pop (sqstack_t * s, selement_t ** e)
{
    if (s->top == s->base) {
        return ERROR;
    }

    *e = --s->top;

    return OK;
}

status stackempty (sqstack_t * s)
{
    if (s->top == s->base)
        return OK;
    return ERROR;
}

status clearstack (sqstack_t * s)
{
    if (s->top == s->base)
        return ERROR;
    s->top = s->base;
    return OK;
}

static int calculate_exp_result(unsigned char *buf, size_t buflen)
{

    return 0;
}

//括号匹配算法
status Parenthesis_match (sqstack_t * s, unsigned char *str)
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

/* @exp: (30004 & 30005 | !30006) */
static int waf_parse_exp(unsigned char *exp_in, size_t inlen, unsigned char *exp_out, size_t outlen)
{
    unsigned char *s, *e, *pos;
    int i;

    if (exp_in == NULL || exp_out == NULL) {
        return -1;
    }

    pos = exp_in;
    for (i = 0; i < inlen; i++) {
        if (pos[i] == ' ' || pos[i] == '(' || pos[i] == ')') {
            continue;   
        }

        if (pos[i] == '&' || pos[i] == '|' || pos[i] == '!') {
            /* operator */
        }

        /* rule_id */
         
    }


    return 0;
}

/** 
 * buf:[30002 & (30003 | (30004 & 30005 | !30006) | 30006) | (30005 & 3006))] len:[70]
 * star-end: 17-40 buff:[(30004 & 30005 | !30006)]
 * star-end: 8-49 buff:[(30003 | (30004 & 30005 | !30006) | 30006)]
 * star-end: 53-66 buff:[(30005 & 3006)]
 * star-end: 53-67 buff:[(30005 & 3006))]
 **/
int main (int argc, char **argv)
{
    unsigned char *buf = NULL;
    int buflen = 0;
    sqstack_t s;

    Init (&s);

    if (waf_fread_file("rule.txt",  &buf,  &buflen) == -1) {
        fprintf(stderr, "fread_file error!\n");
        return -1;
    }

    printf("buf:[%s] len:[%d]\n", buf, buflen);

    Parenthesis_match (&s, buf);

out:
    if (buf) {
        free(buf);
    }

    return 0;
}

static int waf_fread_file(const char *filename, unsigned char **buf, int *buflen)
{
    FILE *fp = NULL;
    unsigned char *temp = NULL;
    size_t fsize = 0;
    fpos_t fpos; 
    int i;

    waf_hit_t hits[10] = {
        {30001, WAF_HIT} , { 30002, WAF_NOT_HIT}
    };

    for (i = 0; i < 10; i++) {
        hits[i].rule_id = 30000 + i;
        if (i%2 == 0) {
            hits[i].hit = WAF_HIT;
        } else {
            hits[i].hit = WAF_NOT_HIT;
        }
    }

    /* show hits */

    for (i  = 0; i < 10; i++) {
        printf("hits[%d]: rule_id:[%d] hit:[%d]\n", 
                i, hits[i].rule_id, hits[i].hit);
    }

    if (filename == NULL || buf == NULL  || buflen == NULL) {
        return -1; 
    }   

    if ((fp = fopen(filename, "r")) == NULL) {
        return -1; 
    }   

    fgetpos(fp, &fpos); 
    fseek(fp, 0, SEEK_END);
    if ((fsize = ftell(fp)) == -1) {
        fclose(fp);
        return -1; 
    }   
    fsetpos(fp,&fpos); 

    if ((temp = (char*)malloc(fsize + 1)) == NULL) {
        fclose(fp);
        return -1; 
    }   
    memset(temp, 0, sizeof(temp));

    if (fread(temp, fsize, 1, fp) != 1) {
        fclose(fp);
        return -1; 
    }   

    *buf = temp;
    *buflen = fsize;

    fclose(fp);

    return 0;
}

