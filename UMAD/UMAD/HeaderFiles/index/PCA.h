/** @file

* @brief Define the PCA class

* @author Abel, Xavier

* @date 2013/5/28

* @version 1.0

*/



#ifndef SQAI_INDEXALGORITHM_PCA_H

#define SQAI_INDEXALGORITHM_PCA_H

#include "../../SourceFiles/util/GSL/include/gsl/gsl_matrix_double.h"

#include "../../SourceFiles/util/GSL/include/gsl/gsl_vector_double.h"

#include <vector>

using namespace std;

/**

* @class CPCA PCA.h "..\..\HeaderFiles\index\PCA.h"

* @brief This class do the Principal Component Analysis (PCA)

* @author Abel, Xavier

*

* Using the GSL library to do the matrix calculation. Support more than 19000x19000 matrix.

*/

class CPCA

{

public:

    /** none parameter constructor */

    CPCA(){}



    /** destructor */

    ~CPCA(){}



    /**

    * @brief Compute Principal Component Analysis with EM method.

    * @author Abel, Xavier

    * @param[in] matrix the matrix to do PCA, needs not to be centerized already. This method will centerize it. Each row is a data point.

    * @param[in] pcNum  the number of pc you want to get

    * @return a gsl_matrix of size [pcNum x (dim +1)], the first column is the variance of each PC in descending order. The remain of each row is a PC.

    */

    static gsl_matrix* EMPCA(gsl_matrix* matrix, const int pcNum);

    static vector<int> pivotSelectionByAngleWithAxes(const gsl_matrix* pcaResult, int numP);
	static vector<int> pivotSelectionByPCAResultProjection(gsl_matrix* matrix,const gsl_matrix* pcaResult, int numP);
	static vector<int> pivotSelectionByAngleWithPoint(gsl_matrix* matrix,const gsl_matrix* pcaResult, int numP);

private:

    /** 

    *  @brief the function used to get the inverse matrix

    *  @author Xavier

    *  @param[in] m  the origin matrix

    *  @param[out] mi  the inverse matrix

    *  

    *  GSL lib has no inverse function. Using LU to get the inverse matrix.

    */

    static void matrix_inverse(gsl_matrix *m,gsl_matrix *mi);



    /** 

    *  @brief the function used to get the transpose matrix

    *  @author Xavier

    *  @param[in] m the origin matrix

    *  @param[out] ct the transpose matrix

    *  

    *  Using gsl_matrix_transpose_memcpy(mi, m). Just used to change the position of two param

    *  and make the program more easily to read.

    */

    static void matrix_transpose(gsl_matrix *m,gsl_matrix *ct);



    /** 

    *  @brief the function used to contribulte the multiplication of two matrix

    *  @author Xavier

    *  @param[in] m1 the multiplicand matrix

    *  @param[in] m2 the multiplier matrix

    *  @param[out] m3 the product matrix

    *  

    *  m3 = m1 * m2 using gsl_blas_dgemm in GSL lib

    */

    static void matrix_mul(gsl_matrix *m1,gsl_matrix *m2,gsl_matrix *m3);



    /**

    *  @brief the function used to contribulte the covariance matrix

    *  @author Xavier

    *  @param[in] r the origin matrix

    *  @param[out] m the cov matrix

    *   

    *  using gsl_stats_covariance in GSL lib to calculate the covariance of two cols.

    */

    static void cov_calculate(gsl_matrix *r, gsl_matrix *m);



    /** 

    *  @brief the function used to centersize the matrix

    *  @author Abel

    *  @param[in] matrix the matrix to be centersized

    *    

    *  the first step of EMPCA

    */

    static void centerize(gsl_matrix* matrix);



    /** 

    *  @brief the function used to calculate the inner product of two vectors;

    *  @author Abel

    *  @param[in] vec1 the first vector

    *  @param[in] vec2 the second vector

    *  @return the inner product of vec1 and vec2; 

    *    

    *  result = Sum(vec1[i] * vec2[i]), 0 < i < max( vec1.size , vec2.size )

    */

    static double vecMult(const gsl_vector* vec1, const gsl_vector* vec2);



    /** 

    *  @brief the function used to finished the iteration work in EM algorithmn

    *  @author Xaiver

    *  @param[in] data the transpose of the origin matrix

    *  @param[in] C the random matrix

    *  @param[in] iterNum the time to iterate

    *   

    *  C is also the output param.

    *  This is the second step in EMPCA

    */

    static void iteration (gsl_matrix *data,gsl_matrix *C,int iterNum);



    /** 

    *  @brief the function used to copy a part of A matrix into B matrix

    *  @author Xavier, Abel

    *  @param[in] inBeginRow : the begin row of the input matrix

    *  @param[in] inBeginCol : the begin col of the input matrix

    *  @param[in] rows : the number of rows you want to copy

    *  @param[in] cols : the number of cols you want to copy

    *  @param[in] in : the input matrix

    *  @param[in] outBeginRow : the begin row of the output matrix

    *  @param[in] outBeginCol : the begin col of the output matrix

    *  @param[out] out : the output matrix

    *   

    *  out[i, j] = in[I, J]

    *  inBeginRow  <= i < inBeinRow  + rows, inBeginCol  <= j < inBeinCol  + cols, 

    *  outBeginRow <= I < outBeinRow + rows, outBeginCol <= J < outBeinCol + cols,   

    */

    static void viewPartCopy(int inBeginRow, int inBeginCol, int rows, int cols, 

        const gsl_matrix *in, int outBeginRow, int outBeginCol, gsl_matrix *out);
	



    /** 

    *  @brief the function used to copy a part of A matrix and change to absolute value into B matrix

    *  @author Xavier, Abel

    *  @param[in] inBeginRow : the begin row of the input matrix

    *  @param[in] inBeginCol : the begin col of the input matrix

    *  @param[in] rows : the number of rows you want to copy

    *  @param[in] cols : the number of cols you want to copy

    *  @param[in] in : the input matrix

    *  @param[in] outBeginRow : the begin row of the output matrix

    *  @param[in] outBeginCol : the begin col of the output matrix

    *  @param[out] out : the output matrix

    *   

    *  out[i, j] = in[I, J]

    *  inBeginRow  <= i < inBeinRow  + rows, inBeginCol  <= j < inBeinCol  + cols, 

    *  outBeginRow <= I < outBeinRow + rows, outBeginCol <= J < outBeinCol + cols,   

    */

    static void viewPartCopyWithABS(int inBeginRow, int inBeginCol, int rows, int cols, 

        const gsl_matrix *in, int outBeginRow, int outBeginCol, gsl_matrix *out);



    /** 

    *  @brief the function used to copy a part of A matrix into B matrix

    *  @author Xavier, Abel

    *  @param[in] outBeginRow : the begin row of the output matrix

    *  @param[in] outBeginCol : the begin col of the output matrix

    *  @param[in] rows : the number of rows you want to copy

    *  @param[in] cols : the number of cols you want to copy

    *  @param[in] in : the input matrix

    *  @param[out] out : the output matrix

    *   

    *  out[i, j] = in[I, J]

    *  inBeginRow  <= i < inBeinRow  + rows, inBeginCol  <= j < inBeinCol  + cols, 

    *  0 <= I < rows, 0 <= J < cols,   

    */

    static void viewPartCopy(int outBeginRow, int outBeginCol, 

                 int rows, int cols, const gsl_matrix *in,gsl_matrix *out);



    /** 

    *  @brief the function used to mirror transpose a matrix's columns

    *  @author Abel

    *  @param[in] m : the matrix to transpose

    *   :

    *  swap(col[i], col[n-i]), 0 < i < m.size2

    */

    static void columnFilp(gsl_matrix *m);



    /** 

    *  @brief the function get the rank of the matrix

    *  @author Abel

    *  @param[in] matrix the matrix want to get the rank

    *  @return the rank of matrix

    *   

    *  Using SVD to calculate. This function used in the orthonormaization.

    */

    static int getRank(const gsl_matrix* matrix);



    /** 

    *  @brief the function orthogonalize the matrix

    *  @author Abel

    *  @param[in] matrix the matrix to be orthonormalized

    *   

    *  This is the third step in EMPCA

    */

    static gsl_matrix* orthonormalization(const gsl_matrix* matrix);

};



#endif