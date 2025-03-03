/* This program gives the optimized implementation of stream cipher HC-128 for 32-bit platform
   
   The docuement of HC-128 is available at
   1) Hongjun Wu. ``The Stream Cipher HC-128.'' New Stream Cipher Designs -- The eSTREAM Finalists, LNCS 4986, pp. 39-47, Springer-Verlag, 2008.  
   2) eSTREAM website:  http://www.ecrypt.eu.org/stream/hcp3.html

   -----------------------------------------
   Performance:

   Microprocessor: Intel CORE 2 processor (Core 2 Duo Mobile P9400 2.53GHz) 
   Operating System: 32-bit Debian 5.0 (Linux kernel 2.6.26-2-686)
   Speed of encrypting long message:
   1) 2.1 cycle/byte   compiler: Intel C++ compiler 11.1   compilation option: icc -O2 
   2) 3.9 cycles/byte  compiler: gcc 4.3.2                 compilation option: gcc -O3  

   Microprocessor: Intel CORE 2 processor (Core 2 Quad Q6600 2.4GHz) 
   Operating System: 32-bit Windows Vista Business
   Speed of encrypting long message:
   3) 2.2 cycles/byte  compiler: Intel C++ compiler 11.1    compilation option: icl /O2
   4) 3.4 cycles/byte  compiler: Microsoft Visual C++ 2008  compilation option: release

   ------------------------------------------
   In this simplified optimization program, loop unrolling is applied to the description of HC-128 directly.
   16 steps are executed in each loop.

   ------------------------------------------
   Written by: Hongjun Wu
   Last Modified: December 15, 2009  
*/

#ifndef HC128_H
#define HC128_H

typedef unsigned char uint8;
typedef unsigned long long uint64;

/*for LP64, "int" is 32-bit integer, while "long" is 64-bit integer*/ 
#if defined(_LP64) 
    typedef unsigned int uint32; 
#else 
    typedef unsigned long uint32; 
#endif 

/*define data alignment for different C compilers*/
#if defined(__GNUC__)
      #define DATA_ALIGN16(x) x __attribute__ ((aligned(16)))
#else
      #define DATA_ALIGN16(x) __declspec(align(16)) x
#endif

typedef struct { 
      DATA_ALIGN16(uint32 P[512]);
      DATA_ALIGN16(uint32 Q[512]);
      DATA_ALIGN16(uint32 keystream[16]);  /*16 32-bit keystream words*/
      uint32 counter1024;                  /*counter1024 = i mod 1024 */     
} HC128_State; 

void SixteenSteps(HC128_State *state);
void UpdateSixteenSteps(HC128_State *state);
void Initialization(HC128_State *state, uint8 *key, uint8 *iv);
void EncryptMessage(HC128_State *state, uint8 *message, uint8 *ciphertext, uint64 msglength);
void HC128(uint8 *key, uint8 *iv, uint8 *message,uint8 *ciphertext, uint64 msglength);

#endif
