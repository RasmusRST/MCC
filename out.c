T = sqrt(pow(G, 2.0f) + (F - V * G) / (2.0f * G - E)) - G;
V = pow((sqrt(D) - Q), (1.0f / 3.0f)) - pow((sqrt(D) + Q), (1.0f / 3.0f));
L_b = copysign(1.0f, r_eb_e(3.0f)) + atan((1.0f - pow(T, 2.0f)) / (2.0f * T * sqrt(1.0f - pow(e, 2.0f))));
C_e_n = MatrixXd<1,2>( +sin_lat * cos_long,4.0f);
