// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#pragma once

#include <sgpp/base/datatypes/DataMatrix.hpp>
#include <sgpp/base/datatypes/DataVector.hpp>
#include <sgpp/base/exception/not_implemented_exception.hpp>
#include <sgpp/base/grid/Grid.hpp>
#include <sgpp/base/operation/hash/OperationMatrix.hpp>
#include <sgpp/datadriven/datamining/modules/fitting/FitterConfiguration.hpp>
#include <sgpp/datadriven/scalapack/BlacsProcessGrid.hpp>
#include <sgpp/datadriven/tools/Dataset.hpp>
#include <sgpp/solver/SLESolver.hpp>
#include <sgpp/solver/TypesSolver.hpp>

#include <sgpp/datadriven/algorithm/GridFactory.hpp>

#include <memory>
#include <vector>

namespace sgpp {

using sgpp::base::DataMatrix;
using sgpp::base::DataVector;
using sgpp::base::Grid;
using sgpp::base::OperationMatrix;
using sgpp::solver::SLESolver;
using sgpp::solver::SLESolverConfiguration;

namespace datadriven {

/**
 * Base class for arbitrary machine learning models based on adaptive sparse grids. A model tries to
 * generalize high dimensional training data by using sparse grids. An underlying model can be
 * trained using training data, its accuracy can be improved by using the adaptivity of sparse
 * grids and the underlying grid(s) of a model can be retrained on other data. Once a model is
 * trained it can be evaluated on unseen data.
 */
class ModelFittingBase {
 public:
  /**
   * Default constructor
   */
  ModelFittingBase();

  // TODO(lettrich): fix this as soon as all member variables are copyable.
  /**
   * Copy constructor - we cannot deep copy all member variables yet.
   * @param rhs const reference to the scorer object to copy from.
   */
  ModelFittingBase(const ModelFittingBase &rhs) = delete;

  /**
   * Move constructor
   * @param rhs R-value reference to a scorer object to moved from.
   */
  ModelFittingBase(ModelFittingBase &&rhs) = default;

  // TODO(lettrich): fix this as soon as all member variables are copyable.
  /**
   * Copy assign operator - we cannot deep copy all member variables yet.
   * @param rhs const reference to the scorer object to copy from.
   * @return rerefernce to this with updated values.
   */
  ModelFittingBase &operator=(const ModelFittingBase &rhs) = delete;

  /**
   * Move assign operator
   * @param rhs R-value reference to an a scorer object to move from.
   * @return rerefernce to this with updated values.
   */
  ModelFittingBase &operator=(ModelFittingBase &&rhs) = default;

  /**
   * virtual destructor.
   */
  virtual ~ModelFittingBase() = default;

  // TODO(lettrich): fix this as soon as all member variables are copyable.
  /**
   * Polymorphic clone pattern
   * @return deep copy of this object. New object is owned by caller.
   */
  // virtual ModelFittingBase* clone() const = 0;

  // TODO(lettrich): dataset should be const.
  /**
   * Fit the grid to the dataset by determinig the weights of an initial grid
   * @param dataset the training dataset that is used to fit the model.
   */
  virtual void fit(Dataset &dataset) = 0;

  /**
   * Improve accuracy of the model on the given training data by adaptive refinement of the grid.
   * @return true if refinement was performed, else false.
   */
  virtual bool refine() = 0;

  // TODO(lettrich): dataset should be const
  /**
   * Train the grid of an existing model with new samples.
   * @param dataset the training dataset that is used to fit the model.
   */
  virtual void update(Dataset &dataset) = 0;

  /**
   * Evaluate the fitted model at a single data point.
   * @param sample vector with the coordinates in all dimensions of that sample.
   * @return evaluation of the model.
   */
  virtual double evaluate(const DataVector &sample) = 0;

  // TODO(lettrich): this should be a const operation as well as the samples matrix as soon
  // operation multiple eval has been taken care of.
  /**
   * Evaluate the fitted model on a set of data points
   * @param samples matrix where each row represents a sample and the columns contain the
   * coordinates in all dimensions of that sample.
   * @param results vector where each row will contain the evaluation of the respective sample on
   * the current model.
   */
  virtual void evaluate(DataMatrix &samples, DataVector &results) = 0;

  /**
   * Resets the state of the entire model
   */
  virtual void reset() = 0;

  /**
   * @returns the BLACS process grid, useful if the fitter uses ScaLAPACK
   */
  virtual std::shared_ptr<BlacsProcessGrid> getProcessGrid() const {
    throw sgpp::base::not_implemented_exception("getProcessGrid() not implemented in this fitter");
  }

  /**
   * Get the configuration of the fitter object.
   * @return configuration of the fitter object
   */
  const FitterConfiguration &getFitterConfiguration() const;

  /**
   * Whether the Solver produces output or not.
   */
  bool verboseSolver;

  // virtual std::string& storeFitter();
  // void storeClassificator();

 protected:
  /**
   * Factory member function that generates a grid from configuration.
   * @param gridConfig configuration for the grid object
   * @return new grid object that is owned by the caller.
   */
  Grid *buildGrid(const sgpp::base::GeneralGridConfiguration &gridConfig) const;

  /**
     * Factory member function that generates a grid from configuration.
     * @param gridConfig configuration for the grid object
     * @param geometryConfig configuration for the geometry parameters
     * @return new grid object that is owned by the caller.
     */
  Grid *buildGrid(const sgpp::base::GeneralGridConfiguration &gridConfig,
                  const GeometryConfiguration &geometryConfig) const;

  /**
   * Factory member function to build the solver for the least squares regression problem according
   * to the config.
   * @param config configuratin for the solver object
   */
  SLESolver *buildSolver(const SLESolverConfiguration &config) const;

  /**
   * Configure solver based on the desired configuration
   * @param solver the solver object to be modified.
   * @param config configuration updating the for the solver.
   */
  void reconfigureSolver(SLESolver &solver, const SLESolverConfiguration &config) const;

  /*
   * This method is used to pass the interactions for a geometry aware sparse grid to the offline
   * object
   * @param geometryConfig from configuration file
   * @return interactions
   */
  std::vector<std::vector<size_t>> getInteractions(
      const GeometryConfiguration &geometryConfig) const;

  /**
   * Configuration object for the fitter.
   */
  std::unique_ptr<FitterConfiguration> config;

  /**
   * Pointer to #sgpp::datadriven::Dataset. The initial grid is fitted on the given data. Adaptive
   * refinement is then performed on the very same data. The used dataset used for refinement
   * overwritten once either fit() or update() introduce a new dataset.
   */
  Dataset *dataset;

  /**
   * Solver for the learning problem
   */
  std::unique_ptr<SLESolver> solver;
};
} /* namespace datadriven */
} /* namespace sgpp */
