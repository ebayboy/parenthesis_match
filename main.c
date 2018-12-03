
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
    int rule_size = 10;
    int rule_ids[10] = {30000, 30001, 30002, 30003, 30004, 30005, 30006, 30007, 30008, 30009};
    int rule_hits[10] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

    sqstack_t s;
    memset(&s, 0, sizeof(s));

    parenthesis_init(&s);

    int i;
    for (i = 0; i < rule_size; i++) {
        printf("rule_id[%d]=[%d]\n", rule_ids[i], rule_hits[i]);
    }

    if (waf_fread_file("rule.txt",  &buf,  &buflen) == -1) {
        fprintf(stderr, "fread_file error!\n");
        return -1;
    }

    printf("buf:[%s] len:[%d]\n", buf, buflen);

    parenthesis_match(&s, buf, rule_ids, rule_hits, rule_size);

    parenthesis_fini(&s);

out:
    if (buf) {
        free(buf);
    }

    return 0;
}

