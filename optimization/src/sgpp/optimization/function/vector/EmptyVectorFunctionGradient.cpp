// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#include <sgpp/globaldef.hpp>
#include <sgpp/optimization/function/vector/EmptyVectorFunctionGradient.hpp>

namespace sgpp {
namespace optimization {

WrapperVectorFunctionGradient& EmptyVectorFunctionGradient::getInstance() {
  static WrapperVectorFunctionGradient wrapperVectorFunctionGradient(
      0, 0, [](const base::DataVector& x, base::DataVector& value, base::DataMatrix& gradient) {});
  return wrapperVectorFunctionGradient;
}
}  // namespace optimization
}  // namespace sgpp
