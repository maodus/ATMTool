#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atm/atm_decode.h"
#include "atm/atm_encode.h"


int main(int argc, char *argv[]){
    if (argc < 2){
        return 1;
    }

    char *option = argv[1];

    if (!strcmp(option, "-d" )){
        DecodeATM(argv[2]);
    } else if (!strcmp(option, "-e" )) {
        EncodeATM(argv[2]);
    } else if(!strcmp(option, "-h")){
        printf("Usage : ./ATMTool [<-option>] filename\n");
        printf("\t-h : Displays program usage info\n");
        printf("\t-d : Decode a .ATM file into 5 separate .AT3 tracks\n");
        printf("\t-e : Encode 5 seperate .AT3 tracks into one .ATM file\n\t     The tracks must be named:\n\t\tdrums.at3, bass_guitar.at3, lead_guitar.at3, vocals.at3, fill.at3\n");
    }

    return 0;
}