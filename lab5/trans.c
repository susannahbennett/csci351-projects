/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 * 
 * Susannah Bennett, 95333
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    // Variable for setting the block size based on the matrix dimensions
    int blocksize;
    // Variables for looping over blocks (block for row and block for column)
    int i, j;
    // Variables for looping through each block (row and column)
    int r, c;
    // Variables for holding a temporary value and for the diagonal, respectively
    int temp = 0, d = 0;

    // For working through square matrices (32x32 and 64x64)
    if (N == 32 || N == 64) {
        if (N == 32){
            blocksize = 8;
        } else if (N == 64) {
            blocksize = 4;
        }

        // This outermost loop iterates across blocks by column (outer two loops are column-major order)
        for (j = 0; j < N; j += blocksize){
            // This loop iterates across blocks by row
            for (i = 0; i < N; i += blocksize) {
                // This loop iterates through each block by row
                for(r = i; r < i+blocksize; r++){
                    // This loop iterates through each block by column
                    for (c = j; c < j + blocksize; c++){
                        // When the row and column values aren't equal, change value
                        if (r != c){
                            B[c][r] = A[r][c];
                        } else {
                            // Otherwise change the diagonal value, d, and temp
                            temp = A[r][c];
                            d = r;                        
                        }
                    }
                    // Because it's a square matrix, don't need to move the elements on the diagonal
                    if (i == j){
                        B[d][d] = temp;
                    }
                }
            }
        }
    } else {
        // When the matrix size is random
        blocksize = 16;
        // This loop iterates over blocks in column-major order
        for (i = 0; i < M; i+=blocksize) {
            for (j = 0; j < N; j+=blocksize) {
                for (r = j; (r < N) && (r < j + blocksize); r++) {
                    for (c = i; (c < M) && c < i + blocksize; c++) {
                        if (r != c) {
                            // When row and column are not the same
                            B[c][r] = A[r][c];
                        } else {
                            // When row and column are the same 
                            temp = A[r][c];
                            d = r;
                        }
                        
                    }
                    // When row and column are the same, the diagonal elements get placed
                    if (i == j) {
                        B[d][d] = temp;
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

