#include "ekf.h"
#include <iostream>
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

    for (int i = 0; i < num_steps; ++i)
    {
        x = rk4_step(x, f_step_size);
    }

    return x;
}

void EKF::initialize(double delta_t, const Eigen::VectorXd &initial_state, const Eigen::MatrixXd &initial_covariance, const Eigen::MatrixXd &process_noise_covariance, const Eigen::MatrixXd &Rd, const Eigen::MatrixXd &Hd)
{
    state = initial_state;
    Z = initial_state;
    covariance = initial_covariance;
    Q = process_noise_covariance;
    R_d = Rd;
    H_d = Hd;
    dt = delta_t;
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

    for (int i = 1; i <= order; ++i)
    {
        result += A_power;

        if (i % 2 == 0)
        {
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
    // double t1 = millis();
    //Eigen::MatrixXd Ad = matrix_exp(J_k_k * dt);
    // double t2 = millis();
    // state = rk4(state, 0.1, 0.0, dt);
    state = rk4(state, dt, 0.0, dt);
    // double t3 = millis();
    covariance = J_k_k * covariance * J_k_k.transpose() + Q;
    //covariance = Ad * covariance * Ad.transpose() + Q;
    // double t4 = millis();

    //std::cout << "prediction:  " << state(0) / 1000000.0 << ", " << state(1) / 1000000.0 << ", " << state(2) / 1000000.0 << ", " << state(3) << ", " << state(4) << ", " << state(5)<< "\n";

    //Serial.printf("matrix_exp: %f, rk4: %f, cov_multiply: %f \n", t2-t1, t3-t2, t4-t3);
}

// void EKF::correct()
// {
//     Eigen::MatrixXd K_k1 = covariance * H_d.transpose() * (H_d * covariance * H_d.transpose() + R_d).inverse();

//     covariance = covariance -  K_k1 * (H_d * covariance * H_d.transpose() + R_d) * K_k1.transpose();


//     state = state +  K_k1 * (Z - H_d * state);

//     std::cout << "correction:  " << state(0) / 1000000.0 << ", " << state(1) / 1000000.0 << ", " << state(2) / 1000000.0 << ", " << state(3) << ", " << state(4) << ", " << state(5) << "\n";
// }

void EKF::correct()
{
    Eigen::MatrixXd S = H_d * covariance * H_d.transpose() + R_d;      // Innovation covariance
    Eigen::MatrixXd K_k1 = covariance * H_d.transpose() * S.inverse(); // Kalman gain

    // Update state estimate.
    state = state + K_k1 * (Z - H_d * state);

    // Update covariance matrix.
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(state.size(), state.size()); // Identity matrix
    covariance = (I - K_k1 * H_d) * covariance;
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