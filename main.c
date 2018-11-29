
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "parenthesis.h"

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

int main (int argc, char **argv)
{
    unsigned char *buf = NULL;
    int buflen = 0;


    sqstack_t s;
    parenthesis_init (&s);

    if (waf_fread_file("rule.txt",  &buf,  &buflen) == -1) {
        fprintf(stderr, "fread_file error!\n");
        return -1;
    }

    printf("buf:[%s] len:[%d]\n", buf, buflen);

    parenthesis_match (&s, buf);

out:
    if (buf) {
        free(buf);
    }

    return 0;
}

