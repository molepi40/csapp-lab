/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "yik",              /* Team name */

    "Hongjing Xiang",     /* First member full name */
    "molepi40@mail.edu.cn",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j, I, J, block_dim;
    block_dim = 16;

    for (J = 0; J < dim; J += block_dim) {
        for (I = 0; I < dim; I += block_dim) {
            for (j = J; j < J + block_dim; j += 4) {
                for (i = I; i < I + block_dim; i += 4) {
                    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
                    dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
                    dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)];
                    dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];

                    dst[RIDX(dim-2-j, i, dim)] = src[RIDX(i, j+1, dim)];
                    dst[RIDX(dim-2-j, i+1, dim)] = src[RIDX(i+1, j+1, dim)];
                    dst[RIDX(dim-2-j, i+2, dim)] = src[RIDX(i+2, j+1, dim)];
                    dst[RIDX(dim-2-j, i+3, dim)] = src[RIDX(i+3, j+1, dim)];

                    dst[RIDX(dim-3-j, i, dim)] = src[RIDX(i, j+2, dim)];
                    dst[RIDX(dim-3-j, i+1, dim)] = src[RIDX(i+1, j+2, dim)];
                    dst[RIDX(dim-3-j, i+2, dim)] = src[RIDX(i+2, j+2, dim)];
                    dst[RIDX(dim-3-j, i+3, dim)] = src[RIDX(i+3, j+2, dim)];

                    dst[RIDX(dim-4-j, i, dim)] = src[RIDX(i, j+3, dim)];
                    dst[RIDX(dim-4-j, i+1, dim)] = src[RIDX(i+1, j+3, dim)];
                    dst[RIDX(dim-4-j, i+2, dim)] = src[RIDX(i+2, j+3, dim)];
                    dst[RIDX(dim-4-j, i+3, dim)] = src[RIDX(i+3, j+3, dim)];
                }
            }
        }
    }
}

char rotate_descr_1[] = "rotate: writing first";
void rotate_1(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (j = 0; j < dim; j++)
	for (i = 0; i < dim; i++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

char rotate_descr_2[] = "rotate: blocking";
void rotate_2(int dim, pixel *src, pixel *dst) 
{
    int i, j, I, J, block_dim;
    block_dim = 16;

    for (J = 0; J < dim; J += block_dim) {
        for (I = 0; I < dim; I += block_dim) {
            for (j = J; j < J + block_dim; ++j) {
                for (i = I; i < I + block_dim; ++i) {
                    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
                }
            }
        }
    }
}

char rotate_descr_3[] = "rotate: unrolling";
void rotate_3(int dim, pixel *src, pixel *dst) 
{
    int i, j, I, J, block_dim;
    block_dim = 16;

    for (J = 0; J < dim; J += block_dim) {
        for (I = 0; I < dim; I += block_dim) {
            for (j = J; j < J + block_dim; j += 4) {
                for (i = I; i < I + block_dim; i += 4) {
                    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
                    dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
                    dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)];
                    dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];

                    dst[RIDX(dim-2-j, i, dim)] = src[RIDX(i, j+1, dim)];
                    dst[RIDX(dim-2-j, i+1, dim)] = src[RIDX(i+1, j+1, dim)];
                    dst[RIDX(dim-2-j, i+2, dim)] = src[RIDX(i+2, j+1, dim)];
                    dst[RIDX(dim-2-j, i+3, dim)] = src[RIDX(i+3, j+1, dim)];

                    dst[RIDX(dim-3-j, i, dim)] = src[RIDX(i, j+2, dim)];
                    dst[RIDX(dim-3-j, i+1, dim)] = src[RIDX(i+1, j+2, dim)];
                    dst[RIDX(dim-3-j, i+2, dim)] = src[RIDX(i+2, j+2, dim)];
                    dst[RIDX(dim-3-j, i+3, dim)] = src[RIDX(i+3, j+2, dim)];

                    dst[RIDX(dim-4-j, i, dim)] = src[RIDX(i, j+3, dim)];
                    dst[RIDX(dim-4-j, i+1, dim)] = src[RIDX(i+1, j+3, dim)];
                    dst[RIDX(dim-4-j, i+2, dim)] = src[RIDX(i+2, j+3, dim)];
                    dst[RIDX(dim-4-j, i+3, dim)] = src[RIDX(i+3, j+3, dim)];
                }
            }
        }
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);
    // add_rotate_function(&rotate_1, rotate_descr_1); 
    // add_rotate_function(&rotate_2, rotate_descr_2);
    // add_rotate_function(&rotate_3, rotate_descr_3); 
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
        }
    }
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    // corner 0, 0
    dst[0].red = (unsigned short)(
    ((int)src[0].red + (int)src[1].red + (int)src[dim].red + (int)src[dim+1].red) / 4);
    dst[0].blue = (unsigned short)(
    ((int)src[0].blue + (int)src[1].blue + (int)src[dim].blue + (int)src[dim+1].blue) / 4);
    dst[0].green = (unsigned short)(
    ((int)src[0].green + (int)src[1].green + (int)src[dim].green + (int)src[dim+1].green) / 4);

    // corner 0, dim - 1
    i = 0, j = dim - 1;
    dst[RIDX(i, j, dim)].red = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].red + (int)src[RIDX(i, j-1, dim)].red + (int)src[RIDX(i+1, j-1, dim)].red + (int)src[RIDX(i+1, j, dim)].red) / 4);
    dst[RIDX(i, j, dim)].blue = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].blue + (int)src[RIDX(i, j-1, dim)].blue + (int)src[RIDX(i+1, j-1, dim)].blue + (int)src[RIDX(i+1, j, dim)].blue) / 4);
    dst[RIDX(i, j, dim)].green = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].green + (int)src[RIDX(i, j-1, dim)].green + (int)src[RIDX(i+1, j-1, dim)].green + (int)src[RIDX(i+1, j, dim)].green) / 4);
    
    // corner dim - 1, 0
    i = dim - 1, j = 0;
    dst[RIDX(i, j, dim)].red = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].red + (int)src[RIDX(i-1, j, dim)].red + (int)src[RIDX(i-1, j+1, dim)].red + (int)src[RIDX(i, j+1, dim)].red) / 4);
    dst[RIDX(i, j, dim)].blue = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].blue + (int)src[RIDX(i-1, j, dim)].blue + (int)src[RIDX(i-1, j+1, dim)].blue + (int)src[RIDX(i, j+1, dim)].blue) / 4);
    dst[RIDX(i, j, dim)].green = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].green + (int)src[RIDX(i-1, j, dim)].green + (int)src[RIDX(i-1, j+1, dim)].green + (int)src[RIDX(i, j+1, dim)].green) / 4);
    
    // corner dim - 1, dim - 1
    i = dim - 1, j = dim - 1;
    dst[RIDX(i, j, dim)].red = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].red + (int)src[RIDX(i, j-1, dim)].red + (int)src[RIDX(i-1, j-1, dim)].red + (int)src[RIDX(i-1, j, dim)].red) / 4);
    dst[RIDX(i, j, dim)].blue = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].blue + (int)src[RIDX(i, j-1, dim)].blue + (int)src[RIDX(i-1, j-1, dim)].blue + (int)src[RIDX(i-1, j, dim)].blue) / 4);
    dst[RIDX(i, j, dim)].green = (unsigned short)(
        ((int)src[RIDX(i, j, dim)].green + (int)src[RIDX(i, j-1, dim)].green + (int)src[RIDX(i-1, j-1, dim)].green + (int)src[RIDX(i-1, j, dim)].green) / 4);
    
    // printf("here0\n");

    // line 0
    int cached_red[dim], cached_blue[dim], cached_green[dim];
    for (j = 0; j < dim; ++j) {
        cached_red[j] = (int)src[RIDX(0, j, dim)].red + (int)src[RIDX(1, j, dim)].red;
        cached_blue[j] = (int)src[RIDX(0, j, dim)].blue + (int)src[RIDX(1, j, dim)].blue;
        cached_green[j] = (int)src[RIDX(0, j, dim)].green + (int)src[RIDX(1, j, dim)].green;
        
    }
    for (j = 1; j < dim - 1; ++j) {
        dst[RIDX(0, j, dim)].red = (unsigned short)(
            (cached_red[j-1] + cached_red[j] + cached_red[j+1]) / 6);
        dst[RIDX(0, j, dim)].blue = (unsigned short)(
            (cached_blue[j-1] + cached_blue[j] + cached_blue[j+1]) / 6);
        dst[RIDX(0, j, dim)].green = (unsigned short)(
            (cached_green[j-1] + cached_green[j] + cached_green[j+1]) / 6);
    }

    // line dim - 1
    i = dim - 1;
    for (j = 0; j < dim; ++j) {
        cached_red[j] = (int)src[RIDX(i, j, dim)].red + (int)src[RIDX(i-1, j, dim)].red;
        cached_blue[j] = (int)src[RIDX(i, j, dim)].blue + (int)src[RIDX(i-1, j, dim)].blue;
        cached_green[j] = (int)src[RIDX(i, j, dim)].green + (int)src[RIDX(i-1, j, dim)].green;   
    }
    for (j = 1; j < dim - 1; ++j) {
        dst[RIDX(i, j, dim)].red = (unsigned short)(
            (cached_red[j-1] + cached_red[j] + cached_red[j+1]) / 6);
        dst[RIDX(i, j, dim)].blue = (unsigned short)(
            (cached_blue[j-1] + cached_blue[j] + cached_blue[j+1]) / 6);
        dst[RIDX(i, j, dim)].green = (unsigned short)(
            (cached_green[j-1] + cached_green[j] + cached_green[j+1]) / 6);
    }

    // col 0
    for (i = 0; i < dim; ++i) {
        cached_red[i] = (int)src[RIDX(i, 0, dim)].red + (int)src[RIDX(i, 1, dim)].red;
        cached_blue[i] = (int)src[RIDX(i, 0, dim)].blue + (int)src[RIDX(i, 1, dim)].blue;
        cached_green[i] = (int)src[RIDX(i, 0, dim)].green + (int)src[RIDX(i, 1, dim)].green;
        
    }
    for (i = 1; i < dim - 1; ++i) {
        dst[RIDX(i, 0, dim)].red = (unsigned short)(
            (cached_red[i-1] + cached_red[i] + cached_red[i+1]) / 6);
        dst[RIDX(i, 0, dim)].blue = (unsigned short)(
            (cached_blue[i-1] + cached_blue[i] + cached_blue[i+1]) / 6);
        dst[RIDX(i, 0, dim)].green = (unsigned short)(
            (cached_green[i-1] + cached_green[i] + cached_green[i+1]) / 6);
    }

    // col dim - 1
    j = dim - 1;
    for (i = 0; i < dim; ++i) {
        cached_red[i] = (int)src[RIDX(i, j, dim)].red + (int)src[RIDX(i, j-1, dim)].red;
        cached_blue[i] = (int)src[RIDX(i, j, dim)].blue + (int)src[RIDX(i, j-1, dim)].blue;
        cached_green[i] = (int)src[RIDX(i, j, dim)].green + (int)src[RIDX(i, j-1, dim)].green;
        
    }
    for (i = 1; i < dim - 1; ++i) {
        dst[RIDX(i, j, dim)].red = (unsigned short)(
            (cached_red[i-1] + cached_red[i] + cached_red[i+1]) / 6);
        dst[RIDX(i, j, dim)].blue = (unsigned short)(
            (cached_blue[i-1] + cached_blue[i] + cached_blue[i+1]) / 6);
        dst[RIDX(i, j, dim)].green = (unsigned short)(
            (cached_green[i-1] + cached_green[i] + cached_green[i+1]) / 6);
    }
    // printf("here1\n");

    for (i = 1; i < dim - 1; ++i) {
        for (j = 0; j < dim; ++j) {
            cached_red[j] = (int)src[RIDX(i, j, dim)].red + (int)src[RIDX(i-1, j, dim)].red + (int)src[RIDX(i+1, j, dim)].red;
            cached_blue[j] = (int)src[RIDX(i, j, dim)].blue + (int)src[RIDX(i-1, j, dim)].blue + (int)src[RIDX(i+1, j, dim)].blue;
            cached_green[j] = (int)src[RIDX(i, j, dim)].green + (int)src[RIDX(i-1, j, dim)].green + (int)src[RIDX(i+1, j, dim)].green;
        }
        for (j = 1; j < dim - 1; ++j) {
            dst[RIDX(i, j, dim)].red = (unsigned short)(
                (cached_red[j-1] + cached_red[j] + cached_red[j+1]) / 9);
            dst[RIDX(i, j, dim)].blue = (unsigned short)(
                (cached_blue[j-1] + cached_blue[j] + cached_blue[j+1]) / 9);
            dst[RIDX(i, j, dim)].green = (unsigned short)(
                (cached_green[j-1] + cached_green[j] + cached_green[j+1]) / 9);
        }
    }
    // printf("here2\n");
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

