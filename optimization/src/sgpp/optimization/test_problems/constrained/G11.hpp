// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef SGPP_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_G11_HPP
#define SGPP_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_G11_HPP

#include <sgpp/globaldef.hpp>

#include <sgpp/optimization/test_problems/constrained/ConstrainedTestProblem.hpp>

namespace sgpp {
namespace optimization {
namespace test_problems {

/**
 * G11 objective function.
 *
 * Definition:
 * \f[\bar{f}(\bar{\vec{x}}) :=
 * \bar{x}_1^2 + (\bar{x}_2 - 1)^2\f]
 */
class G11Objective : public TestScalarFunction {
 public:
  /**
   * Constructor.
   */
  G11Objective();

  /**
   * Destructor.
   */
  ~G11Objective() override;

  /**
   * @param x     point \f$\vec{x} \in [0, 1]^d\f$
   * @return      \f$f(\vec{x})\f$
   */
  double evalUndisplaced(const base::DataVector& x) override;

  /**
   * @param[out] clone pointer to cloned object
   */
  void clone(std::unique_ptr<ScalarFunction>& clone) const override;
};

/**
 * G11 inequality constraint function.
 *
 * Definition: empty, i.e., no constraint
 */
class G11InequalityConstraint : public TestVectorFunction {
 public:
  /**
   * Constructor.
   */
  G11InequalityConstraint();

  /**
   * Destructor.
   */
  ~G11InequalityConstraint() override;

  /**
   * @param       x       point \f$\vec{x} \in \mathbb{R}^d\f$
   * @param[out]  value   \f$\vec{f}(\vec{x})\f$
   */
  void evalUndisplaced(const base::DataVector& x, base::DataVector& value) override;

  /**
   * @param[out] clone pointer to cloned object
   */
  void clone(std::unique_ptr<VectorFunction>& clone) const override;
};

/**
 * G11 equality constraint function.
 *
 * Definition:
 * \f[\bar{h}(\bar{\vec{x}}) :=
 * \bar{x}_2 - \bar{x}_1^2\f]
 */
class G11EqualityConstraint : public TestVectorFunction {
 public:
  /**
   * Constructor.
   */
  G11EqualityConstraint();

  /**
   * Destructor.
   */
  ~G11EqualityConstraint() override;

  /**
   * @param       x       point \f$\vec{x} \in \mathbb{R}^d\f$
   * @param[out]  value   \f$\vec{f}(\vec{x})\f$
   */
  void evalUndisplaced(const base::DataVector& x, base::DataVector& value) override;

  /**
   * @param[out] clone pointer to cloned object
   */
  void clone(std::unique_ptr<VectorFunction>& clone) const override;
};

/**
 * G11 constrained test problem.
 *
 * * Number of parameters: 2
 * * Number of inequality constraints: 0
 * * Number of equality constraints: 1
 * * Domain: \f$\bar{\vec{x}} \in [-1, 1]^2\f$
 * * Optimal point: \f$\bar{\vec{x}}_{\text{opt}} =
 *   (\pm 1/\sqrt{2}, 1/2)\f$
 * * Optimal function value: \f$\bar{f}(\bar{\vec{x}}_{\text{opt}}) =
 *   3/4\f$
 */
class G11 : public ConstrainedTestProblem {
 public:
  /**
   * Constructor.
   */
  G11();

  /**
   * Destructor.
   */
  ~G11() override;

  /**
   * @return  objective function of the test problem
   */
  TestScalarFunction& getObjectiveFunction() override;

  /**
   * @return  inequality function of the test problem
   */
  TestVectorFunction& getInequalityConstraintFunction() override;

  /**
   * @return  equality constraint function of the test problem
   */
  TestVectorFunction& getEqualityConstraintFunction() override;

  /**
   * @param[out] x minimal point
   *               \f$\vec{x}_\opt \in [0, 1]^d\f$
   * @return       minimal function value
   *               \f$f(\vec{x}_\opt)\f$
   */
  double getOptimalPointUndisplaced(base::DataVector& x) override;

 protected:
  /// objective function
  G11Objective f;
  /// inequality constraint function
  G11InequalityConstraint g;
  /// equality constraint function
  G11EqualityConstraint h;
};
}  // namespace test_problems
}  // namespace optimization
}  // namespace sgpp

#endif /* SGPP_OPTIMIZATION_TEST_PROBLEMS_CONSTRAINED_G11_HPP */
