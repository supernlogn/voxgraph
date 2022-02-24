#ifndef VOXGRAPH_BACKEND_CONSTRAINT_COST_FUNCTIONS_PLANES_COST_FUNCTION_H_
#define VOXGRAPH_BACKEND_CONSTRAINT_COST_FUNCTIONS_PLANES_COST_FUNCTION_H_

#include <ceres/ceres.h>
#include <ros/ros.h>

#include "voxgraph/backend/constraint/constraint.h"
#include "voxgraph/frontend/plane_collection/plane_type.h"
#include "voxgraph/frontend/submap_collection/voxgraph_submap.h"
#include "voxgraph/tools/visualization/cost_function_visuals.h"

namespace voxgraph {
class PlanesCostFunction : public ceres::CostFunction{
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  enum class JacobianEvaluationMethod { kAnalytic = 0, kNumeric };

  struct Config {
    // What method to use to calculate the Jacobians
    JacobianEvaluationMethod jacobian_evaluation_method =
        JacobianEvaluationMethod::kAnalytic;
    // Cost to assign for voxels that can't be interpolated
    // i.e. that don't exist in the other submap
    double no_correspondence_cost = 0;

    // Visuals for debugging purposes
    bool visualize_residuals = false;
    bool visualize_gradients = false;
    bool visualize_transforms_ = false;
  };

  PlanesCostFunction(
      const Transformation& T_M_R_origin,
      const PlaneType::ConstPtr origin_plane,
      const Transformation& T_M_R_destination,
      const PlaneType::ConstPtr destination_plane,
      const Constraint::InformationMatrix& sqrt_information_matrix,
      const Config& config);

  static ceres::CostFunction* Create(
      const Transformation& T_M_R_origin,
      const PlaneType::ConstPtr origin_plane,
      const Transformation& T_M_R_destination,
      const PlaneType::ConstPtr destination_plane,
      const Constraint::InformationMatrix& sqrt_information_matrix_,
      const PlanesCostFunction::Config& planes_cost_config);
  /**
   * @brief for easy-direct loss computation
   *
   * @tparam T
   * @param pose_A
   * @param pose_B
   * @param residuals
   * @return true
   * @return false
   */
  bool Evaluate(double const* const* parameters, double* residuals,
                double** jacobians) const;
  // template <typename T>
  // bool operator()(const T* const pose_A, const T* const pose_B,
  //                 T* residuals) const;

 protected:
  Config config_;
  const Constraint::InformationMatrix sqrt_information_matrix_;
  Transformation T_M_R_origin_init_;
  Transformation T_M_R_destination_init_;
  Transformation T_P_R_origin_;
  Transformation T_P_R_destination_;
  PlaneType::ConstPtr origin_plane_;
  PlaneType::ConstPtr destination_plane_;
};
}  // namespace voxgraph

#endif  // VOXGRAPH_BACKEND_CONSTRAINT_COST_FUNCTIONS_PLANES_COST_FUNCTION_H_
