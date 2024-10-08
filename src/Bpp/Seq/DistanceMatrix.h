// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_DISTANCEMATRIX_H
#define BPP_SEQ_DISTANCEMATRIX_H

#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Matrix/Matrix.h>
#include <Bpp/Numeric/VectorExceptions.h> // DimensionException
#include <string>
#include <vector>


namespace bpp
{
/// A Matrix class to store phylogenetic distances.
class DistanceMatrix final : public virtual Clonable
{
private:
  RowMatrix<double> distances_;
  std::vector<std::string> names_;

public:
  /**
   * @brief Build a new distance matrix with specified names.
   * The dimension of the matrix will be equal to the number of names
   * @param names The names to use.
   */
  DistanceMatrix(const std::vector<std::string>& names)
    : distances_(names.size(), names.size())
    , names_(names)
  {
    reset();
  }

  /**
   * @brief Build a new distance matrix with specified size.
   * Row names will be named 'Taxon 0', 'Taxon 1', and so on.
   * @param n The size of the matrix.
   */
  DistanceMatrix(std::size_t n)
    : distances_(n, n)
    , names_(n)
  {
    resize(n);
  }

  DistanceMatrix(const DistanceMatrix& dist)
    : distances_(dist.distances_)
    , names_(dist.names_)
  {}

  DistanceMatrix& operator=(const DistanceMatrix& dist)
  {
    std::size_t n = dist.size();
    resize(n);
    for (std::size_t i = 0; i < n; ++i)
    {
      for (std::size_t j = 0; j < n; ++j)
      {
        distances_(i, j) = dist(i, j);
      }
    }
    names_ = dist.names_;
    return *this;
  }

  DistanceMatrix* clone() const
  {
    return new DistanceMatrix(*this);
  }

public:
  /**
   * @brief Reset the distance matrix: all distances are set to 0.
   */
  void reset()
  {
    std::size_t n = size();
    for (std::size_t i = 0; i < n; ++i)
    {
      for (std::size_t j = 0; j < n; ++j)
      {
        distances_(i, j) = 0;
      }
    }
  }

  /**
   * @return The dimension of the matrix.
   */
  std::size_t size() const
  {
    return names_.size();
  }

  /**
   * @return The names associated to the matrix.
   */
  const std::vector<std::string>& getNames() const
  {
    return names_;
  }

  /**
   * @return The ith name.
   * @param i Name index.
   * @throw IndexOutOfBoundsException If i is not a valid index.
   */
  const std::string& getName(std::size_t i) const
  {
    if (i >= size())
      throw IndexOutOfBoundsException("DistanceMatrix::getName. Invalid indice.", i, 0, size());
    return names_[i];
  }

  /**
   * @brief Set the ith name.
   *
   * @param i Name index.
   * @param name The new name.
   * @throw IndexOutOfBoundsException If i is not a valid index.
   */
  void setName(std::size_t i, const std::string& name)
  {
    if (i >= size())
      throw IndexOutOfBoundsException("DistanceMatrix::setName. Invalid indice.", i, 0, size());
    names_[i] = name;
  }

  /**
   * @brief Set the names associated to the matrix.
   *
   * @param names Matrix names.
   * @throw DimensionException If 'names' have not the same size as the matrix.
   */
  void setNames(const std::vector<std::string>& names)
  {
    if (names.size() != names_.size())
      throw DimensionException("DistanceMatrix::setNames. Invalid number of names.", names.size(), names_.size());
    names_ = names;
  }

  /**
   * @brief Get the index of a given name.
   *
   * @param name The name to look for.
   * @return The position of the name.
   * @throw Exception If no names are attached to this matrix, or if the name was not found.
   */
  std::size_t getNameIndex(const std::string& name) const;

  /**
   * @brief Change the dimension of the matrix.
   *
   * @param n the new dimension of the matrix.
   */
  void resize(std::size_t n)
  {
    // RowMatrix<double>::resize(n, n);
    distances_.resize(n, n);
    names_.resize(n);
    for (std::size_t i = 0; i < n; ++i)
    {
      names_[i] = "Taxon " + std::to_string(i);
    }
    reset();
  }

  /**
   * @brief Access by name.
   *
   * @param iName Name 1 (row)
   * @param jName Name 2 (column)
   * @return A reference toward the specified distance.
   * @throw Exception if the matrix has no name of if one of the name do not match existing names.
   */
  virtual const double& operator()(const std::string& iName, const std::string& jName) const
  {
    std::size_t i = getNameIndex(iName);
    std::size_t j = getNameIndex(jName);
    // return operator()(i,j);
    return distances_(i, j);
  }

  /**
   * @brief Access by name.
   *
   * @param iName Name 1 (row)
   * @param jName Name 2 (column)
   * @return A reference toward the specified distance.
   * @throw Exception if the matrix has no name of if one of the name do not match existing names.
   */
  virtual double& operator()(const std::string& iName, const std::string& jName)
  {
    std::size_t i = getNameIndex(iName);
    std::size_t j = getNameIndex(jName);
    // return operator()(i,j);
    return distances_(i, j);
  }

  virtual const double& operator()(std::size_t i, std::size_t j) const
  {
    // return RowMatrix<double>::operator()(i, j);
    return distances_(i, j);
  }
  virtual double& operator()(std::size_t i, std::size_t j)
  {
    // return RowMatrix<double>::operator()(i, j);
    return distances_(i, j);
  }

  virtual const Matrix<double>& asMatrix() const
  {
    return distances_;
  }

  virtual Matrix<double>& asMatrix()
  {
    return distances_;
  }
};
} // end of namespace bpp.
#endif // BPP_SEQ_DISTANCEMATRIX_H
