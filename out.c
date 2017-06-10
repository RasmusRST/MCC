void GNSS_KF_Epoch(Ref<Vector3d> GNSS_KF_config, Ref<Vector3d> P_matrix_old, Ref<Vector3d> x_est_old, Ref<Vector3d> tor_s, Ref<Vector3d> no_meas, Ref<Vector3d> GNSS_measurements	// input
		Ref<Vector3d> P_matrix_new, Ref<Vector3d> x_est_new	 //output
)
{
/*GNSS_KF_Epoch - Implements one cycle of the GNSS extended Kalman filter
  
   Software for use with "Principles of GNSS, Inertial, and Multisensor
   Integrated Navigation Systems," Second Edition.
  
   This function created 12/4/2012 by Paul Groves
  
   Inputs:
     GNSS_measurements     GNSS measurement data:
       Column 1              Pseudo-range measurements (m)
       Column 2              Pseudo-range rate measurements (m/s)
       Columns 3-5           Satellite ECEF position (m)
       Columns 6-8           Satellite ECEF velocity (m/s)
     no_meas               Number of satellites for which measurements are
                           supplied
     tor_s                 propagation interval (s)
     x_est_old             previous Kalman filter state estimates
     P_matrix_old          previous Kalman filter error covariance matrix
     GNSS_KF_config
       .accel_PSD              Acceleration PSD per axis (m^2/s^3)
       .clock_freq_PSD         Receiver clock frequency-drift PSD (m^2/s^3)
       .clock_phase_PSD        Receiver clock phase-drift PSD (m^2/s)
       .pseudo_range_SD        Pseudo-range measurement noise SD (m)
       .range_rate_SD          Pseudo-range rate measurement noise SD (m/s)
  
   Outputs:
     x_est_new             updated Kalman filter state estimates
       Rows 1-3            estimated ECEF user position (m)
       Rows 4-6            estimated ECEF user velocity (m/s)
       Row 7               estimated receiver clock offset (m) 
       Row 8               estimated receiver clock drift (m/s)
     P_matrix_new          updated Kalman filter error covariance matrix

*/

/* Copyright 2012, Paul Groves
   License: BSD; see license.txt for details

   Constants (sone of these could be changed to inputs at a later date)
*/
c = 299792458.0f;// Speed of light in m/s
omega_ie = 7.292115f;// Earth rotation rate in rad/s

/* Begins

   SYSTEM PROPAGATION PHASE

   1. Determine transition matrix using (9.147) and (9.150)
*/
Phi_matrix = eye(7);
Phi_matrix = tor_s;
Phi_matrix = tor_s;
Phi_matrix = tor_s;
Phi_matrix = tor_s;

// 2. Determine system noise covariance matrix using (9.152)
Q_matrix = zeros(7);
Q_matrix.block<0,0>(3,3) = eye(2) * GNSS_KF_configaccel_PSD * pow(tor_s, 3.0f) / 3.0f;
Q_matrix.block<0,3>(3,3) = eye(2) * GNSS_KF_configaccel_PSD * pow(tor_s, 2.0f) / 2.0f;
Q_matrix.block<3,0>(3,3) = eye(2) * GNSS_KF_configaccel_PSD * pow(tor_s, 2.0f) / 2.0f;
Q_matrix.block<3,3>(3,3) = eye(2) * GNSS_KF_configaccel_PSD * tor_s;
Q_matrix = (GNSS_KF_configclock_freq_PSD * pow(tor_s, 3.0f) / 3.0f) + GNSS_KF_configclock_phase_PSD * tor_s;
Q_matrix = GNSS_KF_configclock_freq_PSD * pow(tor_s, 2.0f) / 2.0f;
Q_matrix = GNSS_KF_configclock_freq_PSD * pow(tor_s, 2.0f) / 2.0f;
Q_matrix = GNSS_KF_configclock_freq_PSD * tor_s;

// 3. Propagate state estimates using (3.14)
x_est_propagated = Phi_matrix * x_est_old;

// 4. Propagate state estimation error covariance matrix using (3.15)
P_matrix_propagated = Phi_matrix * P_matrix_old * Phi_matrix.transpose() + Q_matrix;

// MEASUREMENT UPDATE PHASE

// Skew symmetric matrix of Earth rate
Omega_ie = Skew_symmetric();

u_as_e_T = zeros;
pred_meas = zeros;

// Loop measurements
for (int j = 0; i < no_meas; i++ )
{

// Predict approx range 
delta_r = GNSS_measurements.transpose() - x_est_propagated.block<0,0>(3,1);
approx_range = sqrt(delta_r.transpose() * delta_r);

// Calculate frame rotation during signal transit time using (8.36)
C_e_I = MatrixXd<3,3>(1.0f,omega_ie * approx_range / c,0.0f, -omega_ie * approx_range / c,1.0f,0.0f,0.0f,0.0f,1.0f);

// Predict pseudo-range using (9.165)
delta_r = C_e_I * GNSS_measurements.transpose() - x_est_propagated.block<0,0>(3,1);
range = sqrt(delta_r.transpose() * delta_r);
pred_meas = range + x_est_propagated(6);

// Predict line of sight
u_as_e_T = delta_r.transpose() / range;

// Predict pseudo-range rate using (9.165)
range_rate = u_as_e_T * (C_e_I * (GNSS_measurements.transpose() + Omega_ie * GNSS_measurements.transpose()) - (x_est_propagated.block<3,0>(3,1) + Omega_ie * x_est_propagated.block<0,0>(3,1)));
pred_meas = range_rate + x_est_propagated(7);

}// for j

// 5. Set-up measurement matrix using (9.163)
H_matrix.block<1-1,1-1>(no_meas-1+1,3-1+1) =  -u_as_e_T.block<1-1,1-1>(no_meas-1+1,3-1+1);
H_matrix.block<1-1,4-1>(no_meas-1+1,6-4+1) = zeros;
H_matrix = ones;
H_matrix = zeros;
H_matrix.block<(no_meas + 1)-1,1-1>((2 * no_meas)-(no_meas + 1)+1,3-1+1) = zeros;
H_matrix.block<(no_meas + 1)-1,4-1>((2 * no_meas)-(no_meas + 1)+1,6-4+1) =  -u_as_e_T.block<1-1,1-1>(no_meas-1+1,3-1+1);
H_matrix = zeros;
H_matrix = ones;

// 6. Set-up measurement noise covariance matrix assuming all measurements
// are independent and have equal variance for a given measurement type
R_matrix.block<1-1,1-1>(no_meas-1+1,no_meas-1+1) = eye(no_meas) * pow(GNSS_KF_configpseudo_range_SD, 2.0f);
R_matrix.block<1-1,(no_meas + 1)-1>(no_meas-1+1,(2 * no_meas)-(no_meas + 1)+1) = zeros(no_meas);
R_matrix.block<(no_meas + 1)-1,1-1>((2 * no_meas)-(no_meas + 1)+1,no_meas-1+1) = zeros(no_meas);
R_matrix.block<(no_meas + 1)-1,(no_meas + 1)-1>((2 * no_meas)-(no_meas + 1)+1,(2 * no_meas)-(no_meas + 1)+1) = eye(no_meas) * pow(GNSS_KF_configrange_rate_SD, 2.0f);

// 7. Calculate Kalman gain using (3.21)
K_matrix = P_matrix_propagated * H_matrix.transpose() * inv(H_matrix * P_matrix_propagated * H_matrix.transpose() + R_matrix);

// 8. Formulate measurement innovations using (3.88)
delta_z = GNSS_measurements - pred_meas;
delta_z = GNSS_measurements - pred_meas;

// 9. Update state estimates using (3.24)
x_est_new = x_est_propagated + K_matrix * delta_z;

// 10. Update state estimation error covariance matrix using (3.25)
P_matrix_new = (eye(7) - K_matrix * H_matrix) * P_matrix_propagated;

// Ends