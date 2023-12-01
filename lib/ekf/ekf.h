// This disables some run-time checks which slightly speeds up your program.
// However, this should not be done prematurely, as it also hides otherwise hard-to-find programming mistakes (such as combining expressions of incompatible dimensions)!
// Disable EIGEN DEBUGGING!!!
#define NDEBUG
#include <ArduinoEigenDense.h>

class EKF
{
public:
    EKF();

    void initialize(double delta_t, const Eigen::VectorXd &initial_state, const Eigen::MatrixXd &initial_covariance, const Eigen::MatrixXd &process_noise_covariance, const Eigen::MatrixXd &noise_covariance, const Eigen::MatrixXd &Hd);
    void step();

    Eigen::VectorXd state;
    Eigen::VectorXd Z;
    Eigen::MatrixXd covariance;

private:
    double dt;
    Eigen::MatrixXd Q;   // Process noise covariance
    Eigen::MatrixXd R_d; // (measurement noise variance) Matrices
    Eigen::MatrixXd H_d;

    void predict(const Eigen::MatrixXd &J_k_k);
    void correct();
    Eigen::MatrixXd CalculateJacobian();
};