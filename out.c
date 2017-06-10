/*GNSS_KF_Epoch - Implements one cycle of the GNSS extended Kalman filter
  
   Software for use with "Principles of GNSS, Inertial, and Multisensor
   Integrated Navigation Systems," Second Edition
  
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
       accel_PSD              Acceleration PSD per axis (m^2/s^3)
       clock_freq_PSD         Receiver clock frequency-drift PSD (m^2/s^3)
       clock_phase_PSD        Receiver clock phase-drift PSD (m^2/s)
       pseudo_range_SD        Pseudo-range measurement noise SD (m)
       range_rate_SD          Pseudo-range rate measurement noise SD (m/s)
  
   Outputs:
     x_est_new             updated Kalman filter state estimates
       Rows 1-3            estimated ECEF user position (m)
       Rows 4-6            estimated ECEF user velocity (m/s)
       Row 7               estimated receiver clock offset (m) 
       Row 8               estimated receiver clock drift (m/s)
     P_matrix_new          updated Kalman filter error covariance matrix
*/


// Copyright 2012, Paul Groves
// License: BSD; see licensetxt for details

// Constants (sone of these could be changed to inputs at a later date)
c = 299792458.0f;// Speed of light in m/s
omega_ie = 7292115.0f;// Earth rotation rate in rad/s

// Begins

// SYSTEM PROPAGATION PHASE


// 1 Determine transition matrix using (9147) and (9150)
Phi_matrix = eye(7);
Phi_matrix = tor_s;
Phi_matrix = tor_s;
Phi_matrix = tor_s;
Phi_matrix = tor_s;

// 2 Determine system noise covariance matrix using (9152)
Q_matrix = (GNSS_KF_configclock_freq_PSD * pow(tor_s, 3.0f) / 3.0f) + GNSS_KF_configclock_phase_PSD * tor_s;
Q_matrix = GNSS_KF_configclock_freq_PSD * pow(tor_s, 2.0f) / 2.0f;
Q_matrix = GNSS_KF_configclock_freq_PSD * pow(tor_s, 2.0f) / 2.0f;
Q_matrix = GNSS_KF_configclock_freq_PSD * tor_s;

// 3 Propagate state estimates using (314)
x_est_propagated = Phi_matrix * x_est_old;

// 4 Propagate state estimation error covariance matrix using (315)
P_matrix_propagated = Phi_matrix * P_matrix_old * Phi_matrix.transpose() + Q_matrix;

// MEASUREMENT UPDATE PHASE

// Skew symmetric matrix of Earth rate