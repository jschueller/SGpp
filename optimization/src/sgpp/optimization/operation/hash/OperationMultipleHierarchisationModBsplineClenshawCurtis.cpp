// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <sgpp/globaldef.hpp>

#include <sgpp/optimization/operation/hash/OperationMultipleHierarchisationModBsplineClenshawCurtis.hpp>
#include <sgpp/base/operation/hash/OperationEvalModBsplineClenshawCurtisNaive.hpp>
#include <sgpp/optimization/sle/solver/Auto.hpp>
#include <sgpp/optimization/sle/system/HierarchisationSLE.hpp>

namespace sgpp {
namespace optimization {

OperationMultipleHierarchisationModBsplineClenshawCurtis::
    OperationMultipleHierarchisationModBsplineClenshawCurtis(
        base::ModBsplineClenshawCurtisGrid& grid)
    : grid(grid) {}

OperationMultipleHierarchisationModBsplineClenshawCurtis::
    ~OperationMultipleHierarchisationModBsplineClenshawCurtis() {}

bool OperationMultipleHierarchisationModBsplineClenshawCurtis::doHierarchisation(
    base::DataVector& nodeValues) {
  HierarchisationSLE system(grid);
  sle_solver::Auto solver;
  base::DataVector b(nodeValues);
  return solver.solve(system, b, nodeValues);
}

void OperationMultipleHierarchisationModBsplineClenshawCurtis::doDehierarchisation(
    base::DataVector& alpha) {
  base::GridStorage& storage = grid.getStorage();
  const size_t d = storage.getDimension();
  base::OperationEvalModBsplineClenshawCurtisNaive opNaiveEval(storage, grid.getDegree());
  base::DataVector nodeValues(storage.getSize());
  base::DataVector x(d, 0.0);

  for (size_t j = 0; j < storage.getSize(); j++) {
    storage.getCoordinates(storage[j], x);
    nodeValues[j] = opNaiveEval.eval(alpha, x);
  }

  alpha.resize(storage.getSize());
  alpha = nodeValues;
}

bool OperationMultipleHierarchisationModBsplineClenshawCurtis::doHierarchisation(
    base::DataMatrix& nodeValues) {
  HierarchisationSLE system(grid);
  sle_solver::Auto solver;
  base::DataMatrix B(nodeValues);
  return solver.solve(system, B, nodeValues);
}

void OperationMultipleHierarchisationModBsplineClenshawCurtis::doDehierarchisation(
    base::DataMatrix& alpha) {
  base::GridStorage& storage = grid.getStorage();
  const size_t d = storage.getDimension();
  base::OperationEvalModBsplineClenshawCurtisNaive opNaiveEval(storage, grid.getDegree());
  base::DataVector nodeValues(storage.getSize(), 0.0);
  base::DataVector x(d, 0.0);
  base::DataVector alpha1(storage.getSize(), 0.0);

  for (size_t i = 0; i < alpha.getNcols(); i++) {
    alpha.getColumn(i, alpha1);

    for (size_t j = 0; j < storage.getSize(); j++) {
      storage.getCoordinates(storage[j], x);
      nodeValues[j] = opNaiveEval.eval(alpha1, x);
    }

    alpha.setColumn(i, nodeValues);
  }
}
}  // namespace optimization
}  // namespace sgpp
