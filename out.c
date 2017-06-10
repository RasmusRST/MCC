/* ECEF_to_NED - Converts Cartesian  to curvilinear position, velocity
   resolving axes from ECEF to NED and attitude from ECEF- to NED-referenced
  
   Software for use with "Principles of GNSS, Inertial, and Multisensor
   Integrated Navigation Systems," Second Edition.
  
   This function created 2/4/2012 by Paul Groves
  
   Inputs:
     r_eb_e        Cartesian position of body frame w.r.t. ECEF frame, resolved
                   along ECEF-frame axes (m)
     v_eb_e        velocity of body frame w.r.t. ECEF frame, resolved along
                   ECEF-frame axes (m/s)
     C_b_e         body-to-ECEF-frame coordinate transformation matrix
  
   Outputs:
     L_b           latitude (rad)
     lambda_b      longitude (rad)
     h_b           height (m)
     v_eb_n        velocity of body frame w.r.t. ECEF frame, resolved along
                   north, east, and down (m/s)
     C_b_n         body-to-NED coordinate transformation matrix
*/

// Parameters
R_0 = 6378137.0f;//WGS84 Equatorial radius in meters
e = 0.081819f;//WGS84 eccentricity

// Copyright 2012, Paul Groves
// License: BSD; see license.txt for details

// Begins

// Convert position using Borkowski closed-form exact solution
// From (2.113)
lambda_b = atan2(r_eb_e(1),r_eb_e(0));

// From (C.29) and (C.30)
k1 = sqrt(1.0f - pow(e, 2.0f)) * abs(r_eb_e(2));
k2 = pow(e, 2.0f) * R_0;
beta = sqrt(pow(r_eb_e(0), 2.0f) + pow(r_eb_e(1), 2.0f));
E = (k1 - k2) / beta;
F = (k1 + k2) / beta;

// From (C.31)
P = 4.0f / 3.0f * (E * F + 1.0f);

// From (C.32)
Q = 2.0f * (pow(E, 2.0f) - pow(F, 2.0f));

// From (C.33)
D = pow(P, 3.0f) + pow(Q, 2.0f);

// From (C.34)
V = pow((sqrt(D) - Q), (1.0f / 3.0f)) - pow((sqrt(D) + Q), (1.0f / 3.0f));

// From (C.35)
G = 0.500000f * (sqrt(pow(E, 2.0f) + V) + E);

// From (C.36)
T = sqrt(pow(G, 2.0f) + (F - V * G) / (2.0f * G - E)) - G;

// From (C.37)
L_b = copysign(1.0f, r_eb_e(2)) * atan((1.0f - pow(T, 2.0f)) / (2.0f * T * sqrt(1.0f - pow(e, 2.0f))));

// From (C.38)
h_b = (beta - R_0 * T) * cos(L_b) + (r_eb_e(2) - copysign(1.0f, r_eb_e(2)) * R_0 * sqrt(1.0f - pow(e, 2.0f))) * sin(L_b);

// Calculate ECEF to NED coordinate transformation matrix using (2.150)
cos_lat = cos(L_b);
sin_lat = sin(L_b);
cos_long = cos(lambda_b);
sin_long = sin(lambda_b);
C_e_n = MatrixXd<3,3>( -sin_lat * cos_long, -sin_lat * sin_long,cos_lat, -sin_long,cos_long,0.0f, -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat);

// Transform velocity using (2.73)
v_eb_n = C_e_n * v_eb_e;

// Transform attitude using (2.15)
C_b_n = C_e_n * C_b_e;
