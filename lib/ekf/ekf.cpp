#include "ekf.h"

typedef Eigen::VectorXd state_type;

EKF::EKF() {}

inline Eigen::Matrix<double, 6, 1> f(const Eigen::Matrix<double, 6, 1> &x)
{
    Eigen::Matrix<double, 6, 1> dxdt;
    dxdt << -x(2) * x(4) + x(1) * x(5),
        x(2) * x(3) - x(0) * x(5),
        -x(1) * x(3) + x(0) * x(4),
        -x(4) * x(5),
        x(3) * x(5),
        -x(3) * x(4) / 3;
    return dxdt;
}

inline Eigen::Matrix<double, 6, 1> rk4_step(const Eigen::Matrix<double, 6, 1> &x, double step_size)
{
    Eigen::Matrix<double, 6, 1> k1 = f(x);
    k1 *= step_size;

    Eigen::Matrix<double, 6, 1> half_k1 = 0.5 * k1;

    Eigen::Matrix<double, 6, 1> k2 = f(x + half_k1);
    k2 *= step_size;

    Eigen::Matrix<double, 6, 1> half_k2 = 0.5 * k2;

    Eigen::Matrix<double, 6, 1> k3 = f(x + half_k2);
    k3 *= step_size;

    Eigen::Matrix<double, 6, 1> k4 = f(x + k3);
    k4 *= step_size;

    // Reuse the result matrix to minimize memory copies
    Eigen::Matrix<double, 6, 1> result = x + (1.0 / 6.0) * step_size * (k1 + 2 * k2 + 2 * k3 + k4);

    return result;
}

Eigen::VectorXd rk4(const Eigen::VectorXd &x_initial, double f_step_size, double t_start, double t_end)
{
    Eigen::VectorXd x = x_initial;
    int num_steps = static_cast<int>((t_end - t_start) / f_step_size);

    for (int i = 0; i < num_steps; ++i) {
        x = rk4_step(x, f_step_size);
    }

    return x;
}

void EKF::initialize(double delta_t)
{
    state = Eigen::VectorXd::Zero(6);
    Z = Eigen::VectorXd::Zero(6);
    covariance = Eigen::MatrixXd::Zero(6, 6);
    Q = 0.02 * Eigen::MatrixXd::Identity(6, 6);
    H_d = Eigen::MatrixXd::Identity(6, 6);
    dt = delta_t;

    Eigen::MatrixXd R_d(6, 6);
    R_d << 2.02559220e-01, 5.17515015e-03, -3.16669361e-02, -1.76503506e-04, -3.74891174e-05, -7.75657503e-05,
        5.17515015e-03, 1.55389381e-01, 1.07780468e-02, -2.90511952e-05, -8.02931174e-06, -1.26277622e-05,
        -3.16669361e-02, 1.07780468e-02, 3.93162684e-01, 9.29630074e-05, 1.22496815e-05, 5.67092127e-05,
        -1.76503506e-04, -2.90511952e-05, 9.29630074e-05, 1.80161545e-05, -2.27002599e-09, -6.07376965e-07,
        -3.74891174e-05, -8.02931174e-06, 1.22496815e-05, -2.27002599e-09, 6.70144060e-06, 2.97298687e-08,
        -7.75657503e-05, -1.26277622e-05, 5.67092127e-05, -6.07376965e-07, 2.97298687e-08, 8.52192033e-06;

    Q.diagonal() << 0.008, 0.07, 0.005, 0.1, 0.1, 0.1;
}

void EKF::step()
{
    Eigen::MatrixXd J = CalculateJacobian();

    predict(J);

    correct();
}

Eigen::MatrixXd matrix_exp(const Eigen::MatrixXd &A, int order = 5)
{
    int n = A.rows();
    Eigen::MatrixXd result = Eigen::MatrixXd::Identity(n, n);
    Eigen::MatrixXd A_power = A;
    Eigen::MatrixXd identity = Eigen::MatrixXd::Identity(n, n);

    for (int i = 1; i <= order; ++i) {
        result += A_power;

        if (i % 2 == 0) {
            result -= A_power;
        }

        A_power = A_power * A / static_cast<double>(i + 1);
    }
    // Pade Approximant for Speedup (SciPy+Matlab use this).
    Eigen::MatrixXd U = A * 0.5;
    Eigen::MatrixXd V = identity - U;
    Eigen::MatrixXd P = result * V + U;
    Eigen::MatrixXd Q = -result * U + V;

    result = P.inverse() * Q;

    return result;
}

void EKF::predict(const Eigen::MatrixXd &J_k_k)
{

    state = rk4(state, 0.1, 0.0, dt);
    covariance = J_k_k * covariance * J_k_k.transpose() + Q;
}

void EKF::correct()
{
    Eigen::MatrixXd K_k1 = covariance * H_d.transpose() * (H_d * covariance * H_d.transpose() + R_d).inverse();
    covariance -= K_k1 * (H_d * covariance * H_d.transpose() + R_d) * K_k1.transpose();
    state += K_k1 * (Z - H_d * state);
}

Eigen::MatrixXd EKF::CalculateJacobian()
{
    double Bx = Z(0), By = Z(1), Bz = Z(2), wx = Z(3), wy = Z(4), wz = Z(5);
    Eigen::MatrixXd J(6, 6);

    J << 0, wz, -wy, 0, -Bz, By,
        -wz, 0, wx, Bz, 0, -Bx,
        wy, -wx, 0, -By, Bx, 0,
        0, 0, 0, 0, -wz, -wy,
        0, 0, 0, wz, 0, wx,
        0, 0, 0, -wx / 3, -wy / 3, 0;

    return J;
}