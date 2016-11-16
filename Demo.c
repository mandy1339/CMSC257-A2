////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmsc257-assign2.c
//  Description    : This is the file that contains main. It's purpose is to
//                   test my improved versions of the memory handling functions.
//                   malloc() is called 10 times, calloc() is called 10 times
//                   realloc() is called 10 times, and free is called 10 times
//                   the memory leaks displayed include the size of the linked
//                   list and the free memory chunks.
//
//   Author        : Armando L. Toledo
//   Last Modified : 10/28/2016
//
//

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "MemHandler.h"

int main()
{
    int occupied_bytes = 0;            //keeps track of requested byte allocations and deallocations
    void * starting_prg_brk = sbrk(0);
    printf("Testing Functions and Analyzing Memory\nlegend for the block_meta's: (meta->size, meta->free)\n\n\n");

    printf("Starting Heap Address: %p\n\n", starting_prg_brk);

    printf("State before any calls_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    //5 malloc calls
    char * a = (char *)malloc(100);
    occupied_bytes += 100;             //requested 100 bytes
    printf("State after Malloc 100 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    char * b = (char *)malloc(105);     //requested 105 bytes
    occupied_bytes += 105;
    printf("State after Malloc 105 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    char * c = (char *)malloc(99);      //requested 99 bytes
    occupied_bytes += 99;
    printf("State after Malloc 99 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    char * d = (char *)malloc(50);
    occupied_bytes += 50;
    printf("State after Malloc 50 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    char * e = (char *)malloc(50);
    occupied_bytes += 50;
    printf("State after Malloc 50 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    //5 realloc calls
    a = (char *)realloc(a, 80);
    occupied_bytes += 80;             //new occupied memory
    occupied_bytes -= 100;            //new non occupied memory
    printf("State after realloc 80 bytes to block a (100, 0)_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    b = (char *)realloc(b, 133);
    occupied_bytes += 133;
    occupied_bytes -= 108;
    printf("State after realloc 133 bytes to block b (108, 0)_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    c = (char *)realloc(c, 400);
    occupied_bytes += 400;
    occupied_bytes -= 100;
    printf("State after realloc 400 bytes to block c (100, 0)_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    d = (char *)realloc(d, 300); //este no
    occupied_bytes += 300;
    occupied_bytes -= 52;
    printf("State after realloc 300 bytes to block d (52, 0)_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    e = (char *)realloc(e, 500);
    occupied_bytes += 500;
    occupied_bytes -= 52;
    printf("State after realloc 500 bytes to block e (52, 0)_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));


    //5 free calls
    free(a);
    a = NULL;
    occupied_bytes -= 80;
    printf("State after freeing a (80 block) _________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    free(b);
    b = NULL;
    occupied_bytes -= 133;
    printf("State after freeing b (136 block) _________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    free(c);
    c = NULL;
    occupied_bytes -= 400;
    printf("State after freeing c (400 block) _________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    free(d);
    d = NULL;
    occupied_bytes -= 300;
    printf("State after freeing d (300 block) _________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    free(e);
    e = NULL;
    occupied_bytes -= 500;
    printf("State after freeing e (500 block) _________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));


    //5 malloc calls
    char * k = (char *)malloc(100);
    occupied_bytes += 100;
    printf("State after Malloc 100 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    char * l = (char *)malloc(105);
    occupied_bytes += 105;
    printf("State after Malloc 105 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    char * m = (char *)malloc(99);
    occupied_bytes += 99;
    printf("State after Malloc 99 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    char * n = (char *)malloc(50);
    occupied_bytes += 50;
    printf("State after Malloc 50 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    char * o = (char *)malloc(80);
    occupied_bytes += 80;
    printf("State after Malloc 80 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));


    //5 realloc calls
    (char *)realloc(k, 198);
    occupied_bytes += 198;
    occupied_bytes -= 100;
    printf("State after realloc 198 bytes to k block (100,0)______________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)realloc(l, 250);
    occupied_bytes += 250;
    occupied_bytes -= 108;
    printf("State after realloc 250 bytes to l block (108, 0)_____________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)realloc(m, 231);
    occupied_bytes += 231;
    occupied_bytes -= 100;
    printf("State after realloc 231 bytes to m block (100, 0)______________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)realloc(n, 400);
    occupied_bytes += 400;
    occupied_bytes -= 52;
    printf("State after realloc 400 bytes to n block (52, 0)_________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)realloc(o, 500);
    occupied_bytes += 500;
    occupied_bytes -= 80;
    printf("State after realloc 500 bytes to o block (80, 0)_________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));


    //10 calloc calls
    (char *)calloc(200, 1);
    occupied_bytes += 200;
    printf("State after calloc 200 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(300, 1);
    occupied_bytes += 300;
    printf("State after calloc 300 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(200, 1);
    occupied_bytes += 200;
    printf("State after calloc 200 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(900, 1);
    occupied_bytes += 900;
    printf("State after calloc 900 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(200, 1);
    occupied_bytes += 200;
    printf("State after calloc 200 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(350, 1);
    occupied_bytes += 350;
    printf("State after calloc 350 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(400, 1);
    occupied_bytes += 400;
    printf("State after calloc 400 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(320, 1);
    occupied_bytes += 320;
    printf("State after calloc 320 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(410, 1);
    occupied_bytes += 410;
    printf("State after calloc 410 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    (char *)calloc(198, 1);
    occupied_bytes += 198;
    printf("State after calloc 198 bytes_________________________________________");
    show_state();
    printf("Memory Leaks v2: %d\n\n\n", memory_leaks_v2(starting_prg_brk, sbrk(0), occupied_bytes));

    printf("Ending Heap Address: %p\n\n", sbrk(0));
	return 0;


}

