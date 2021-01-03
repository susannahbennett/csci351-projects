/* 
 * CS:APP Data Lab 
 * 
 * Susannah Bennett - 95333
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
       // 0x55 is a word with even-numbered bits set to 1
       int x = 0x55;
       // We shift x left and OR to repeat the pattern of even-numbered bits, up to 16 bits and then 32 bits
       x = x | (x<<8);
       x = x | (x<<16);
       return x;
}
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  // I solved this puzzle by making a chart and working backwards
  return ~x & ~y;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  return (x >> (n<<3)) & 0xFF;  // shift it over 8 * byte n times and mask/reveal the farthest right 8 bits
}
/* 
 * tc2sm - Convert from two's complement to sign-magnitude 
 *   where the MSB is the sign bit
 *   You can assume that x > TMin
 *   Example: tc2sm(-5) = 0x80000005.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int tc2sm(int x) {
  int y = x >> 31; // finds the sign of x (either all 1s or 0s, -1 or 0)
  int removeSign = (y+x)^y; // find the absolute value of x, using XOR to toggle the bits
  int addSB = (x & (1<<31)); // determines whether to add the sign bit (1 in MSB) or not 
  return removeSign|addSB; // adds sign bit (or not) to absolute value of x
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
  int leftShift = (x << n); // shifts x left by n, leaving trailing 0s
  int bitsToWrap = x >> (32+~n+1); // shifts x right by 32-n to get the bits that will fall off in the left shift
  int mask = (1 << n) + ~0; // isolates the bits that would fall off in the left shift
  return leftShift^(bitsToWrap & mask); // combines the isolated bits that would have fallen off and the rest of the shifted number
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  // variable declaration for first mask
  int evenBit, totalEB, shift;
  // variable declaration for second mask
  int doublesBit, totalDB;
  // variable declaration for third mask;
  int quadBit, totalQB;
  // variable declaration for fourth mask;
  int eightBit, totalEIB;
  // variable declaration for fifth mask;
  int totalST;
  // variable declaration for final total
  int total;
  
  // make first mask (1 bit wide)
  evenBit = 0x55; // 0101 0101
  totalEB = (evenBit << 8) | evenBit;
  shift = totalEB << 16;
  totalEB = shift | totalEB;

  // make second mask (2 bits wide)
  doublesBit = 0x33; // 0011 0011
  totalDB = (doublesBit << 8 ) | doublesBit;
  shift = totalDB << 16;
  totalDB = shift | totalDB;

  // make third mask (4 bits wide)
  quadBit = 0x0F; // 0000 11111
  totalQB = (quadBit << 8) | quadBit;
  shift = totalQB << 16;
  totalQB = shift | totalQB;

  // make fourth mask (8 bits wide)
  eightBit = 0xFF; // 1111 11111
  shift = eightBit << 16;
  totalEIB = shift | eightBit;
  
  // make fifth mask (16 bits wide)
  totalST = (0xFF << 8) | 0xFF; // 16 bits of 0s and 16 bits of 1

  // adding together bits for the count
  total = (x & totalEB) + ((x>>1) & totalEB);
  total = (total & totalDB) + ((total>>2) & totalDB);
  total = (total & totalQB) + ((total>>4) & totalQB);
  total = (total & totalEIB) + ((total>>8) & totalEIB);
  total = (total & totalST) + ((total>>16) & totalST);

  return total;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 0x80 << 24; // returns 1000 0000 0000 ... 0000 because it's the most negative number (sign bit)
}
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
  int sign = (x>>31); // makes the entire number the value of the sign bit (all 1s or 0s) 
  return sign+!((!x)|(!!sign)); // this will either add 1 or 0 to sign, making it either (-1 or 0) + (1 or 0)
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int y = 33 + (~n); // creates the number of shifts to move over, it's like subtracting n from 32 (32 + 1 + ~n = 33 + ~n) 
  return !(x^((x << y) >> y)); // tells you when you shift it back to the right whether any bits fell off when you shifted left
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  int sign = ((x^y)>>31)&1; // shows whether x or y have the same sign or not
  int diff = x+(1+(~y)); // subtract x and y
  // determines whether the difference of x and y is positive or negative
  // if the difference between x and y is positive, return 1, else 0
  // if x and y have different signs and y is greater than 0, then return 0
  return ((!sign & !(diff >> 31)) + ~(!(diff))+1) | (sign & (y>>31));
}
/* 
 * rempwr2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: rempwr2(15,2) = 3, rempwr2(-35,3) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int rempwr2(int x, int n) {
  int sign = x >> 31; // gives us the sign of x
  int abs = (sign + x) ^ sign; // finds the absolute value of x (definition of 2'sC)
  int result = abs & ((1<<n)+(~0x0)); // finding the mod of x and 2^n, combines the absolute value of x with 1 less than 2^n (all positions before 2^n are 1's)  
  return (result+sign)^sign; // reverts the number back to its original sign, using the backwards absolute value
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  // essentially, building the result, continuously adding result to the next result, as you narrow down from 16 bits to 1 bit
  int result = (!!(x>>16))<<4; 
  result = result + (!!(x>>(8 + result))<< 3); 
  result = result + (!!(x>>(4 + result))<< 2);
  result = result + (!!(x>>(2 + result))<< 1);
  result = result + !!(x>>(1 + result));
  return result;
}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
  int abs = uf & 0x7FFFFFFF; // mask for removing sign bit (0111 0000 ... 0000)
  if (abs > (0x7F800000)) // if the absolute value of uf is greater than infinity, then it's a NaN number, so return uf
    return uf;
  else
    return abs; // else, return the absolute value (uf with 0 in sign bit)
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
  int sign = uf & 0x80000000; // for isolating the first bit/sign bit with a mask (1000 0000 ... 0000)
  int abs = uf & 0x7FFFFFFF; // for finding the absolute value/changing sign bit to 0 with a mask (0111 0000 ... 0000)
  int infinity = 0x7F800000; // number of infinity
  int exp = uf & infinity; // for isolating the 8-bit exp section
  int frac = uf & 0x007FFFFF; // for isolating the 23-bit frac section
  
  if (abs >= infinity) { // For special case:
    return uf;
  } else if ((uf & infinity) == 0){ // For denorm case:
      // use 0x00000003 for rounding to even (only round to even if it is 11)
      if ((frac & 0x00000003) == 0x00000003){
	return (sign | (frac >> 1)) + 1; // add 1 because of rounding
      } else {
        return (sign | (frac >> 1)); // shift by 1 divides by 2
      }
      return sign | frac;
  } else if ((uf & exp) == 0x00800000) { // For when norm becomes denorm: (when exp = 1)
      if ((frac & 0x00000003) == 0x00000003) { // rounding to even check
	return (sign | (frac >> 1) | 0x00400000) + 1;
      } else {
	return sign | (frac >> 1) | 0x00400000; // OR with 0x00400000 to move the 1 in exp to over to frac without having to do a shift
      }
  } else { // For normal case:
     exp = exp - 0x00800000; // subtract 1 from exp
     return sign | exp | frac; // assembling the number to return
  }
}
