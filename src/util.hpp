/*
 *       util.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      �gor Bonadio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#ifndef UTIL_H
#define UTIL_H
//#define NDEBUG

#include "crossplatform.hpp"

#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <vector>
#include <string>
#include <cmath>
#include "Sequence.hpp"
#include "SequenceEntry.hpp"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/math/special_functions/erf.hpp>

#ifdef WIN32
 #define _min _cpp_min
 #define _max _ccp_max
#else
 #define _min std::min
 #define _max std::max
#endif

#define _erf boost::math::erf

namespace tops {

  typedef std::vector <double> DoubleVector;
  typedef std::vector <int> IntVector;
  typedef boost::numeric::ublas::matrix<double> Matrix;
  typedef boost::numeric::ublas::matrix<float> fMatrix;
  typedef boost::numeric::ublas::mapped_matrix<double> MMatrix;
  typedef boost::numeric::ublas::compressed_matrix<float,boost::numeric::ublas::row_major> CMatrix;
  typedef boost::numeric::ublas::matrix<int> IntMatrix;
  typedef std::vector <std::string> StringVector;
  typedef std::map <std::string, std::string> StringMap;

  //! Remove spaces from the ends of the string
  void trim_spaces (std::string & s);

  //! Split the string by using a separator
  void split_regex (const std::string & s, std::vector <std::string> & result, const boost::regex & re);

  //! Calculates the value of log(exp(log_a) + exp(log_b))
  DLLEXPORT double log_sum( double log_a, double log_b);
  DLLEXPORT double log_sum_2( double log_a, double log_b);

  double lookup (double x);


  //! Divides the a by b
  double safe_division(double a, double b);

  //! Returns true if a is close to b with a given tolerance
  bool close(double a, double b, double tolerance) ;

  ///! Calculates the value of D mod d
  int mod(int D, int d);

  //! Reads a list of sequences from a file
  void readSequencesFromFile(SequenceEntryList & s, AlphabetPtr alphabet, std::string file_name) ;

  //! Reads a list of sequences from a file
  void readSequencesFromFile(SequenceList & s, AlphabetPtr alphabet, std::string file_name) ;


  /*   Sheather and Jones bandwidth */
  double sj_bandwidth(const DoubleVector &data);
  double kernel_density_estimation(double x, double bw, const DoubleVector &data);
  double kernel_density_estimation_gaussian(double x, double bw, const DoubleVector &data);

  /* Epanechnikov kernel */
  double epanechnikov(double x, double h);
    /* normal kernel */
    double kernel_normal(double x, double h);
// code from R-1.7.0/src/appl/bandwidths.c
#define abs9(a) (a > 0 ? a:-a)
  void band_den_bin(int n, int nb, double *d, const DoubleVector &x,  DoubleVector &cnt);
  void band_phi6_bin(int n, int nb, double d, DoubleVector &x, double h, double *u);
  void band_phi4_bin(int n, int nb, double d, DoubleVector x, double h, double *u);

  double mean(const DoubleVector &data);

  double var(const DoubleVector &data);
  /* quantile */
  double quantile (DoubleVector data, double q);

    /* interquantile */
  double iqr (const DoubleVector &data);

  //! reads a table with two column, the first column is a key, and the second column is a double value.
  void readMapFromFile(std::map<std::string, double> & s, std::string  file_name);


#ifndef HUGE
#include <limits>
#define HUGE (std::numeric_limits<double>::max())
#endif
}
#endif
