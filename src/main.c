/*
    62 instructions
    31 dynamic
    33 static
    29 dynamic finished
*/

#include <stdlib.h>
#include <stdio.h>
#include "asm.h"

int
asm_line(char *line)
{
    int (*instr)(prog_data_t *, reg_data_t *, uint8_t *);
    reg_data_t rd;
    prog_data_t pd;
    uint8_t bytes[255];
    int b_len;
    int ret;

    pd.pass = 2;
    pd.bt = 0;

    printf("instruction: %s\n", line);
    
    to_upper(line);
    ret = parse(line, &rd, &pd, &instr);
    
    if(ret == -1) {
        fprintf(stderr, "Invalid instruction1.\n");
        return 1;
    }

    b_len = instr(&pd, &rd, bytes);

    if(b_len == -1) {
        fprintf(stderr, "Invalid instruction.\n");
        return 1;
    }

    for(int i = 0; i < b_len; i++)
        printf("%02X ", bytes[i]);
    printf("\n");

    return 0;
}

int
asm_file(char *file)
{
    // Variable declarations
    FILE *fpi, *fpo;
    char c, line[255], linecp[255], *linep;
    int (*instr)(prog_data_t *, reg_data_t *, uint8_t *);
    prog_data_t *pd;
    reg_data_t *rd;
    uint8_t bytes[255];
    int b_len;
    int ret, i = 0;
    int linen = 0;

    // Variable assignment
    pd = malloc(sizeof(prog_data_t));
    rd = malloc(sizeof(reg_data_t));
    pd->labn = 0;

    // Open input file
    if(!(fpi = fopen(file, "r"))) {
        fprintf(stderr, "Couldn't open file.\n");
        return 1;
    }

    // Start pass 1
    pd->bt = 0;
    pd->pass = 1;
    while((c = fgetc(fpi)) != EOF) {
        if(c != '\n') {
            line[i] = c;
            i++;
            continue;
        }

        linen++;
        line[i] = 0;
        strcpy(linecp, line);
        linep = trim(linecp);

        to_upper(line);
        ret = parse(line, rd, pd, &instr);

        if(ret == -1) {
            fprintf(stderr, "Couldn't parse instruction on line %d.\n", linen);
            fprintf(stderr, "\"%s\"\n", linep);
            return 1;
        }

        if(ret == 0) {
            line[0] = i = 0;
            continue;
        }

        b_len = instr(pd, rd, bytes);

        if(b_len == -1) {
            fprintf(stderr, "Invalid instruction on line %d.\n", linen);
            fprintf(stderr, "\"%s\"\n", linep);
            return 1;
        }

        pd->bt += b_len;
        line[0] = i = 0;
    }

    // Reopen input file and open output file
    fclose(fpi);

    if(!(fpi = fopen(file, "r"))) {
        fprintf(stderr, "Couldn't open file.\n");
        return 1;
    }

    if(!(fpo = fopen("out.bin", "w"))) {
        fprintf(stderr, "Couldn't open file.\n");
        return 1;
    }

    // Start pass 2
    pd->bt = 0;
    pd->pass = 2;
    while((c = fgetc(fpi)) != EOF) {
        if(c != '\n') {
            line[i] = c;
            i++;
            continue;
        }

        line[i] = 0;

        to_upper(line);
        ret = parse(line, rd, pd, &instr);

        if(ret == -1) {
            fprintf(stderr, "Couldn't parse instruction on line %d.\n", linen);
            fprintf(stderr, "\"%s\"\n", linep);
            return 1;
        }

        if(ret == 0) {
            line[0] = i = 0;
            continue;
        }

        b_len = instr(pd, rd, bytes);
        
        if(b_len == -1) {
            fprintf(stderr, "Invalid instruction on line %d.\n", linen);
            fprintf(stderr, "\"%s\"\n", linep);
            return 1;
        }

        fwrite(bytes, 1, b_len, fpo);
        pd->bt += b_len;
        line[0] = i = 0;
    }

    // Finish
    fclose(fpi);
    fclose(fpo);
    return 0;
}

int
main(int argc, char *argv[])
{
    if(argc != 3) {
        fprintf(stderr, "Invalid input.\n");
        printf("%d\n", argc);
        return 1;
    }

    if(!strcmp("-file", argv[1])) {
        return asm_file(argv[2]);
    } else if(!strcmp("-line", argv[1])) {
        return asm_line(argv[2]);
    } else {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }
}

