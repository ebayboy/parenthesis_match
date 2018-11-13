
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACKINCREAMENT 10
#define STACK_INIT_SIZE 100
#define OVERFLOW -2
#define OK 1
#define ERROR 0

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

//括号匹配算法
status Parenthesis_match (sqstack_t * s, char *str)
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
                        fprintf(stderr, "buff:[%s]\n", buff);
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

int main (int argc, char **argv)
{
    char str[100], enter;
    sqstack_t s;

    Init (&s);
    printf ("请输入字符串:\n");
    scanf ("%s", str);
    scanf ("%c", &enter);

    Parenthesis_match (&s, str);

    return 0;
}
