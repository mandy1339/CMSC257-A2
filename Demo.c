#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "MemHandler.h"
//#include <stdlib.h>

int main()
{

    printf("Testing Functions and Analyzing Memory\n\n");

    printf("Start Heap Address: %p\n\n", sbrk(0));

    printf("State before any calls_________________________________________");
    show_state();

    //5 malloc calls
    char * a = (char *)malloc(100);
    printf("State after Malloc 100 bytes_________________________________________");
    show_state();

    char * b = (char *)malloc(105);
    printf("State after Malloc 105 bytes_________________________________________");
    show_state();

    char * c = (char *)malloc(99);
    printf("State after Malloc 99 bytes_________________________________________");
    show_state();

    char * d = (char *)malloc(50);
    printf("State after Malloc 50 bytes_________________________________________");
    show_state();

    char * e = (char *)malloc(50);
    printf("State after Malloc 50 bytes_________________________________________");
    show_state();

    char * as = (char *)calloc(600, 1);
    printf("State after calloc 198 bytes_________________________________________");
    show_state();

    char * asd = (char *)calloc(600, 1);
    printf("State after calloc 198 bytes_________________________________________");
    show_state();

    char * asdf = (char *)calloc(600, 1);
    printf("State after calloc 198 bytes_________________________________________");
    show_state();


    //5 realloc calls
    char * f = (char *)realloc(a, 198);
    printf("State after realloc 198 bytes_________________________________________");
    show_state();

    char * g = (char *)realloc(b, 133);
    printf("State after realloc 133 bytes_________________________________________");
    show_state();

    char * h = (char *)realloc(c, 400);
    printf("State after realloc 400 bytes_________________________________________");
    show_state();

    char * i = (char *)realloc(d, 300);
    printf("State after realloc 300 bytes_________________________________________");
    show_state();

    char * j = (char *)realloc(e, 500);
    printf("State after realloc 500 bytes_________________________________________");
    show_state();


    //5 free calls
    free(f);
    f = NULL;
    printf("State after freeing f _________________________________________");
    show_state();

    free(g);
    g = NULL;
    printf("State after freeing g _________________________________________");
    show_state();

    free(h);
    h = NULL;
    printf("State after freeing h _________________________________________");
    show_state();

    free(i);
    i = NULL;
    printf("State after freeing i _________________________________________");
    show_state();

    free(j);
    j = NULL;
    printf("State after freeing j _________________________________________");
    show_state();


    //5 malloc calls
    char * k = (char *)malloc(100);
    printf("State after Malloc 100 bytes_________________________________________");
    show_state();

    char * l = (char *)malloc(105);
    printf("State after Malloc 105 bytes_________________________________________");
    show_state();

    char * m = (char *)malloc(99);
    printf("State after Malloc 99 bytes_________________________________________");
    show_state();

    char * n = (char *)malloc(50);
    printf("State after Malloc 50 bytes_________________________________________");
    show_state();

    char * o = (char *)malloc(50);
    printf("State after Malloc 50 bytes_________________________________________");
    show_state();


    //5 realloc calls
    char * p = (char *)realloc(k, 198);
    printf("State after realloc 198 bytes_________________________________________");
    show_state();

    char * q = (char *)realloc(l, 250);
    printf("State after realloc 198 bytes_________________________________________");
    show_state();

    char * r = (char *)realloc(m, 231);
    printf("State after realloc 198 bytes_________________________________________");
    show_state();

    char * s = (char *)realloc(n, 400);
    printf("State after realloc 198 bytes_________________________________________");
    show_state();

    char * t = (char *)realloc(o, 500);
    printf("State after realloc 198 bytes_________________________________________");
    show_state();


    //10 calloc calls
    char * u = (char *)malloc(200);
    printf("State after mallocc ccc cccc 600 bytes_________________________________________");
    show_state();

    char * v = (char *)malloc(200);
    printf("State after calloc 700 bytes_________________________________________");
    show_state();

    char * w = (char *)malloc(200);
    printf("State after calloc 800 bytes_________________________________________");
    show_state();

    char * x = (char *)malloc(200);
    printf("State after calloc 900 bytes_________________________________________");
    show_state();

    char * y = (char *)malloc(200);
    printf("State after calloc 1000 bytes_________________________________________");
    show_state();

    char * z = (char *)calloc(200, 1);
    printf("State after calloc 1100 bytes_________________________________________");
    show_state();

    char * a1 = (char *)calloc(200, 1);
    printf("State after calloc 1200 bytes_________________________________________");
    show_state();

    char * b1 = (char *)calloc(200, 1);
    printf("State after calloc 1300 bytes_________________________________________");
    show_state();

    char * c1 = (char *)calloc(200, 1);
    printf("State after calloc 1400 bytes_________________________________________");
    show_state();

    char * d1 = (char *)calloc(198, 1);
    printf("State after calloc 198 bytes_________________________________________");
    show_state();

    printf("End Heap Address: %p\n\n", sbrk(0));



	return 0;


}

