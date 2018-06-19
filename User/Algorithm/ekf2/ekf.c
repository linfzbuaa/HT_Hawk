/*
 * File: ekf_func_quad_6dof.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 17-Jun-2018 20:30:18
 */

/* Include Files */
#include "rt_strap.h"
#include "ekf.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void qkf_initialize(void)
{
  rt_InitInfAndNaN(8U);
}

/*
 * Arguments    : float X[7]
 *                float P[49]
 *                const float acc[3]
 *                const float gyro[3]
 *                const float mag[3]
 *                float dt
 *                const float Q[49]
 *                const float R[36]
 * Return Type  : void
 */
void qkf_run(float X[7], float P[49], const float acc[3], const
  float gyro[3], const float mag[3], float dt, const float Q[49], const
  float R[36])
{
  float q0;
  float q1;
  float q2;
  float q3;
  float scale;
  float absxk;
  float t;
  float bx;
  float b_X;
  float c_X;
  float d_X;
  float e_X;
  float f_X;
  float g_X;
  float h_X;
  float i_X;
  float j_X;
  float k_X;
  float l_X;
  float m_X;
  float n_X;
  float o_X;
  float p_X;
  float q_X;
  float r_X;
  float s_X;
  float t_X;
  float u_X;
  int k;
  float PHI[49];
  signed char I[49];
  int i2;
  float b_PHI[49];
  float cbn[9];
  int i3;
  float x[2];
  float mR[3];
  float H[42];
  float K[42];
  float b_H[36];
  float b_bx[3];
  float c_H[42];
  float b_cbn[3];
  float c_cbn[6];
  float d_cbn[6];
  float b_acc[6];
  static const signed char b[3] = { 0, 0, -1 };

  static const signed char b_I[49] = { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 1 };

  q0 = X[0];
  q1 = X[1];
  q2 = X[2];
  q3 = X[3];

  /*  gyro bias */
  /*  传感器模型 */
  /*  系统观测模型 */
  scale = X[1];
  absxk = X[0];
  t = X[4];
  bx = X[3];
  b_X = X[5];
  c_X = X[2];
  d_X = X[6];
  e_X = X[2];
  f_X = X[3];
  g_X = X[4];
  h_X = X[0];
  i_X = X[5];
  j_X = X[1];
  k_X = X[6];
  l_X = X[3];
  m_X = X[2];
  n_X = X[4];
  o_X = X[1];
  p_X = X[5];
  q_X = X[0];
  r_X = X[6];
  s_X = X[4];
  t_X = X[5];
  u_X = X[6];
  X[0] = ((X[0] - X[1] * (gyro[0] - X[4]) * dt / 2.0) - X[2] * (gyro[1] - X[5]) *
          dt / 2.0) - X[3] * (gyro[2] - X[6]) * dt / 2.0;
  X[1] = ((scale + absxk * (gyro[0] - t) * dt / 2.0) - bx * (gyro[1] - b_X) * dt
          / 2.0) + c_X * (gyro[2] - d_X) * dt / 2.0;
  X[2] = ((e_X + f_X * (gyro[0] - g_X) * dt / 2.0) + h_X * (gyro[1] - i_X) * dt /
          2.0) - j_X * (gyro[2] - k_X) * dt / 2.0;
  X[3] = ((l_X - m_X * (gyro[0] - n_X) * dt / 2.0) + o_X * (gyro[1] - p_X) * dt /
          2.0) + q_X * (gyro[2] - r_X) * dt / 2.0;
  X[4] = s_X;
  X[5] = t_X;
  X[6] = u_X;

  /*  四元数归一化 */
  scale = norm(*(float (*)[4])&X[0]);
  for (k = 0; k < 4; k++) {
    X[k] /= scale;
  }

  /*  状态转移矩阵 */
  /*  四元数更新    */
  /*  观测协方差更新 */
  for (k = 0; k < 49; k++) {
    I[k] = 0;
  }

  PHI[0] = 0.0 * dt;
  PHI[7] = -gyro[0] / 2.0 * dt;
  PHI[14] = -gyro[1] / 2.0 * dt;
  PHI[21] = -gyro[2] / 2.0 * dt;
  PHI[28] = q1 / 2.0 * dt;
  PHI[35] = q2 / 2.0 * dt;
  PHI[42] = q3 / 2.0 * dt;
  PHI[1] = gyro[0] / 2.0 * dt;
  PHI[8] = 0.0 * dt;
  PHI[15] = gyro[2] / 2.0 * dt;
  PHI[22] = -gyro[1] / 2.0 * dt;
  PHI[29] = -q0 / 2.0 * dt;
  PHI[36] = q3 / 2.0 * dt;
  PHI[43] = -q2 / 2.0 * dt;
  PHI[2] = gyro[1] / 2.0 * dt;
  PHI[9] = -gyro[2] / 2.0 * dt;
  PHI[16] = 0.0 * dt;
  PHI[23] = gyro[0] / 2.0 * dt;
  PHI[30] = -q3 / 2.0 * dt;
  PHI[37] = -q0 / 2.0 * dt;
  PHI[44] = q1 / 2.0 * dt;
  PHI[3] = gyro[2] / 2.0 * dt;
  PHI[10] = gyro[1] / 2.0 * dt;
  PHI[17] = -gyro[0] / 2.0 * dt;
  PHI[24] = 0.0 * dt;
  PHI[31] = q2 / 2.0 * dt;
  PHI[38] = -q1 / 2.0 * dt;
  PHI[45] = -q0 / 2.0 * dt;
  for (k = 0; k < 7; k++) {
    I[k + 7 * k] = 1;
    PHI[4 + 7 * k] = 0.0 * dt;
    PHI[5 + 7 * k] = 0.0 * dt;
    PHI[6 + 7 * k] = 0.0 * dt;
  }

  for (k = 0; k < 7; k++) {
    for (i2 = 0; i2 < 7; i2++) {
      b_PHI[i2 + 7 * k] = (float)I[i2 + 7 * k] + PHI[i2 + 7 * k];
    }
  }

  for (k = 0; k < 7; k++) {
    for (i2 = 0; i2 < 7; i2++) {
      PHI[k + 7 * i2] = 0.0;
      for (i3 = 0; i3 < 7; i3++) {
        PHI[k + 7 * i2] += b_PHI[k + 7 * i3] * P[i3 + 7 * i2];
      }
    }
  }

  for (k = 0; k < 7; k++) {
    for (i2 = 0; i2 < 7; i2++) {
      scale = 0.0;
      for (i3 = 0; i3 < 7; i3++) {
        scale += PHI[k + 7 * i3] * b_PHI[i2 + 7 * i3];
      }

      P[k + 7 * i2] = scale + Q[k + 7 * i2];
    }
  }

  /* %%% 这一坨看不懂	 */
  my_quat2cbn(*(float (*)[4])&X[0], cbn);
  for (k = 0; k < 3; k++) {
    mR[k] = 0.0;
    for (i2 = 0; i2 < 3; i2++) {
      mR[k] += cbn[k + 3 * i2] * mag[i2];
    }
  }

  x[0] = mR[0];
  x[1] = mR[1];
  bx = 0.0;
  scale = 3.3121686421112381E-170;
  for (k = 0; k < 2; k++) {
    absxk = fabs(x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      bx = 1.0 + bx * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      bx += t * t;
    }
  }

  bx = scale * sqrt(bx);

  /* 计算预测值 */
  H[0] = 2.0 * X[2];
  H[6] = -2.0 * X[3];
  H[12] = 2.0 * X[0];
  H[18] = -2.0 * X[1];
  H[24] = 0.0;
  H[30] = 0.0;
  H[36] = 0.0;
  H[1] = -2.0 * X[1];
  H[7] = -2.0 * X[0];
  H[13] = -2.0 * X[3];
  H[19] = -2.0 * X[2];
  H[25] = 0.0;
  H[31] = 0.0;
  H[37] = 0.0;
  H[2] = -2.0 * X[0];
  H[8] = 2.0 * X[1];
  H[14] = 2.0 * X[2];
  H[20] = -2.0 * X[3];
  H[26] = 0.0;
  H[32] = 0.0;
  H[38] = 0.0;
  H[3] = 2.0 * (-2.0 * mR[2] * X[2]);
  H[9] = 2.0 * (2.0 * mR[2] * X[3]);
  H[15] = 2.0 * (-4.0 * bx * X[2] - 2.0 * mR[2] * X[0]);
  H[21] = 2.0 * (-4.0 * bx * X[3] + 2.0 * mR[2] * X[1]);
  H[27] = 0.0;
  H[33] = 0.0;
  H[39] = 0.0;
  H[4] = 2.0 * (-2.0 * bx * X[3] + 2.0 * mR[2] * X[1]);
  H[10] = 2.0 * (2.0 * bx * X[2] + 2.0 * mR[2] * X[0]);
  H[16] = 2.0 * (2.0 * bx * X[1] + 2.0 * mR[2] * X[3]);
  H[22] = 2.0 * (-2.0 * bx * X[0] + 2.0 * mR[2] * X[2]);
  H[28] = 0.0;
  H[34] = 0.0;
  H[40] = 0.0;
  H[5] = 2.0 * (2.0 * bx * X[2]);
  H[11] = 2.0 * (2.0 * bx * X[3] - 4.0 * mR[2] * X[1]);
  H[17] = 2.0 * (2.0 * bx * X[0] - 4.0 * mR[2] * X[2]);
  H[23] = 2.0 * (2.0 * bx * X[1]);
  H[29] = 0.0;
  H[35] = 0.0;
  H[41] = 0.0;

  /*  更新卡尔曼增益 */
  for (k = 0; k < 7; k++) {
    for (i2 = 0; i2 < 6; i2++) {
      K[k + 7 * i2] = 0.0;
      for (i3 = 0; i3 < 7; i3++) {
        K[k + 7 * i2] += P[k + 7 * i3] * H[i2 + 6 * i3];
      }
    }
  }

  for (k = 0; k < 6; k++) {
    for (i2 = 0; i2 < 7; i2++) {
      c_H[k + 6 * i2] = 0.0;
      for (i3 = 0; i3 < 7; i3++) {
        c_H[k + 6 * i2] += H[k + 6 * i3] * P[i3 + 7 * i2];
      }
    }

    for (i2 = 0; i2 < 6; i2++) {
      scale = 0.0;
      for (i3 = 0; i3 < 7; i3++) {
        scale += c_H[k + 6 * i3] * H[i2 + 6 * i3];
      }

      b_H[k + 6 * i2] = scale + R[k + 6 * i2];
    }
  }

  mrdivide(K, b_H);

  /*  卡尔曼更新阶段，公式3-5 */
  b_bx[0] = bx;
  b_bx[1] = 0.0;
  b_bx[2] = mR[2];
  for (k = 0; k < 3; k++) {
    mR[k] = 0.0;
    b_cbn[k] = 0.0;
    for (i2 = 0; i2 < 3; i2++) {
      mR[k] += cbn[i2 + 3 * k] * (float)b[i2];
      b_cbn[k] += cbn[i2 + 3 * k] * b_bx[i2];
    }

    b_acc[k] = acc[k];
    b_acc[k + 3] = mag[k];
    d_cbn[k] = mR[k];
    d_cbn[k + 3] = b_cbn[k];
  }

  for (k = 0; k < 6; k++) {
    c_cbn[k] = d_cbn[k] - b_acc[k];
  }

  /* 得到估计状态 */
  for (k = 0; k < 7; k++) {
    scale = 0.0;
    for (i2 = 0; i2 < 6; i2++) {
      scale += K[k + 7 * i2] * c_cbn[i2];
    }

    X[k] -= scale;
    for (i2 = 0; i2 < 7; i2++) {
      scale = 0.0;
      for (i3 = 0; i3 < 6; i3++) {
        scale += K[k + 7 * i3] * H[i3 + 6 * i2];
      }

      PHI[k + 7 * i2] = (float)b_I[k + 7 * i2] - scale;
    }

    for (i2 = 0; i2 < 7; i2++) {
      b_PHI[k + 7 * i2] = 0.0;
      for (i3 = 0; i3 < 7; i3++) {
        b_PHI[k + 7 * i2] += PHI[k + 7 * i3] * P[i3 + 7 * i2];
      }
    }
  }

  for (k = 0; k < 7; k++) {
    for (i2 = 0; i2 < 7; i2++) {
      P[i2 + 7 * k] = b_PHI[i2 + 7 * k];
    }
  }

  /*  更新协方差矩阵 */
  /*  归一化四元数 */
  scale = norm(*(float (*)[4])&X[0]);
  for (k = 0; k < 4; k++) {
    X[k] /= scale;
  }
}

/*
 * Arguments    : float A[42]
 *                const float B[36]
 * Return Type  : void
 */
void mrdivide(float A[42], const float B[36])
{
  float b_A[36];
  int k;
  int j;
  signed char ipiv[6];
  int c;
  int jAcol;
  int jy;
  int ix;
  float smax;
  int iy;
  int i;
  float s;
  memcpy(&b_A[0], &B[0], 36U * sizeof(float));
  for (k = 0; k < 6; k++) {
    ipiv[k] = (signed char)(1 + k);
  }

  for (j = 0; j < 5; j++) {
    c = j * 7;
    jAcol = 0;
    ix = c;
    smax = fabs(b_A[c]);
    for (k = 2; k <= 6 - j; k++) {
      ix++;
      s = fabs(b_A[ix]);
      if (s > smax) {
        jAcol = k - 1;
        smax = s;
      }
    }

    if (b_A[c + jAcol] != 0.0) {
      if (jAcol != 0) {
        ipiv[j] = (signed char)((j + jAcol) + 1);
        ix = j;
        iy = j + jAcol;
        for (k = 0; k < 6; k++) {
          smax = b_A[ix];
          b_A[ix] = b_A[iy];
          b_A[iy] = smax;
          ix += 6;
          iy += 6;
        }
      }

      k = (c - j) + 6;
      for (i = c + 1; i + 1 <= k; i++) {
        b_A[i] /= b_A[c];
      }
    }

    iy = c;
    jy = c + 6;
    for (jAcol = 1; jAcol <= 5 - j; jAcol++) {
      smax = b_A[jy];
      if (b_A[jy] != 0.0) {
        ix = c + 1;
        k = (iy - j) + 12;
        for (i = 7 + iy; i + 1 <= k; i++) {
          b_A[i] += b_A[ix] * -smax;
          ix++;
        }
      }

      jy += 6;
      iy += 6;
    }
  }

  for (j = 0; j < 6; j++) {
    jy = 7 * j;
    jAcol = 6 * j;
    for (k = 1; k <= j; k++) {
      iy = 7 * (k - 1);
      if (b_A[(k + jAcol) - 1] != 0.0) {
        for (i = 0; i < 7; i++) {
          A[i + jy] -= b_A[(k + jAcol) - 1] * A[i + iy];
        }
      }
    }

    smax = 1.0 / b_A[j + jAcol];
    for (i = 0; i < 7; i++) {
      A[i + jy] *= smax;
    }
  }

  for (j = 5; j >= 0; j--) {
    jy = 7 * j;
    jAcol = 6 * j;
    for (k = j + 1; k + 1 < 7; k++) {
      iy = 7 * k;
      if (b_A[k + jAcol] != 0.0) {
        for (i = 0; i < 7; i++) {
          A[i + jy] -= b_A[k + jAcol] * A[i + iy];
        }
      }
    }
  }

  for (jAcol = 4; jAcol >= 0; jAcol--) {
    if (ipiv[jAcol] != jAcol + 1) {
      iy = ipiv[jAcol] - 1;
      for (jy = 0; jy < 7; jy++) {
        smax = A[jy + 7 * jAcol];
        A[jy + 7 * jAcol] = A[jy + 7 * iy];
        A[jy + 7 * iy] = smax;
      }
    }
  }
}

/*
 * %%旋转矢量转换为四元数
 * Arguments    : const float ang[3]
 *                float q[4]
 * Return Type  : void
 */
void my_ang2quat(const float ang[3], float q[4])
{
  float b_norm;
  int i;
  float f;
  b_norm = 0.0;
  for (i = 0; i < 3; i++) {
    b_norm += ang[i] * ang[i];
  }

  b_norm = sqrt(b_norm);
  if (b_norm > 1.0E-40) {
    f = sin(b_norm / 2.0) / b_norm;
  } else {
    f = 0.5;
  }

  q[0] = cos(b_norm / 2.0);
  for (i = 0; i < 3; i++) {
    q[i + 1] = f * ang[i];
  }
}

/*
 * CALCH
 *     H = CALCH(BX,BZ,Q0,Q1,Q2,Q3)
 * Arguments    : float bx
 *                float bz
 *                float q0
 *                float q1
 *                float q2
 *                float q3
 *                float H[42]
 * Return Type  : void
 */
void my_calcH(float bx, float bz, float q0, float q1, float q2, float q3,
              float H[42])
{
  float t2;
  float t3;
  float t4;
  float t13;
  float t5;
  float t8;
  float t9;
  float t12;
  float t14;
  float b_t3[42];
  int i0;
  int i1;

  /*     This function was generated by the Symbolic Math Toolbox version 8.0. */
  /*     17-Jun-2018 00:29:49 */
  t2 = q0 * 2.0;
  t3 = q2 * 2.0;
  t4 = bz * q1 * 2.0;
  t13 = bx * q3 * 2.0;
  t5 = t4 - t13;
  t8 = bx * q1 * 2.0 + bz * q3 * 2.0;
  t9 = bx * q0 * 2.0;
  t12 = bx * q2 * 2.0 + bz * q0 * 2.0;
  t14 = bz * q2 * 2.0;
  b_t3[0] = t3;
  b_t3[1] = q1 * -2.0;
  b_t3[2] = -t2;
  b_t3[3] = t9 - bz * q2 * 2.0;
  b_t3[4] = t5;
  b_t3[5] = t12;
  b_t3[6] = q3 * -2.0;
  b_t3[7] = -t2;
  b_t3[8] = q1 * 2.0;
  b_t3[9] = t8;
  b_t3[10] = t12;
  b_t3[11] = -t4 + t13;
  b_t3[12] = t2;
  b_t3[13] = q3 * -2.0;
  b_t3[14] = t3;
  b_t3[15] = bx * q2 * -2.0 - bz * q0 * 2.0;
  b_t3[16] = t8;
  b_t3[17] = t9 - t14;
  b_t3[18] = q1 * -2.0;
  b_t3[19] = -t3;
  b_t3[20] = q3 * -2.0;
  b_t3[21] = t5;
  b_t3[22] = -t9 + t14;
  b_t3[23] = t8;
  memset(&b_t3[24], 0, 18U * sizeof(float));
  for (i0 = 0; i0 < 7; i0++) {
    for (i1 = 0; i1 < 6; i1++) {
      H[i1 + 6 * i0] = b_t3[i1 + 6 * i0];
    }
  }
}

static float rt_atan2d_snf(float u0, float u1);

/* Function Definitions */

/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
static float rt_atan2d_snf(float u0, float u1)
{
  float y;
  int b_u0;
  int b_u1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }

    if (u1 > 0.0) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }

    y = atan2(b_u0, b_u1);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/*
 * Arguments    : const float q_data[4]
 *                const int q_size[2]
 *                float *roll
 *                float *pitch
 *                float *yaw
 * Return Type  : void
 */
void my_quat2att(const float q_data[4], const int q_size[2], float *roll,
                 float *pitch, float *yaw)
{
  float q11;
  float q22;
  float q33;
  float q44;
  (void)q_size;
  q11 = q_data[0] * q_data[0];
  q22 = q_data[1] * q_data[1];
  q33 = q_data[2] * q_data[2];
  q44 = q_data[3] * q_data[3];
  *roll = rt_atan2d_snf(2.0 * (q_data[0] * q_data[1] + q_data[2] * q_data[3]),
                        ((q11 - q22) - q33) + q44);
  *pitch = asin(2.0 * (q_data[0] * q_data[2] - q_data[1] * q_data[3]));
  *yaw = rt_atan2d_snf(2.0 * (q_data[1] * q_data[2] + q_data[0] * q_data[3]),
                       ((q11 + q22) - q33) - q44);
}

/*
 * Arguments    : const float q[4]
 *                float cbn[9]
 * Return Type  : void
 */
void my_quat2cbn(const float q[4], float cbn[9])
{
  float q11;
  float q12;
  float q13;
  float q14;
  float q22;
  float q23;
  float q24;
  float q33;
  float q34;
  float q44;
  q11 = q[0] * q[0];
  q12 = q[0] * q[1];
  q13 = q[0] * q[2];
  q14 = q[0] * q[3];
  q22 = q[1] * q[1];
  q23 = q[1] * q[2];
  q24 = q[1] * q[3];
  q33 = q[2] * q[2];
  q34 = q[2] * q[3];
  q44 = q[3] * q[3];
  cbn[0] = ((q11 + q22) - q33) - q44;
  cbn[3] = 2.0 * (q23 - q14);
  cbn[6] = 2.0 * (q24 + q13);
  cbn[1] = 2.0 * (q23 + q14);
  cbn[4] = ((q11 - q22) + q33) - q44;
  cbn[7] = 2.0 * (q34 - q12);
  cbn[2] = 2.0 * (q24 - q13);
  cbn[5] = 2.0 * (q34 + q12);
  cbn[8] = ((q11 - q22) - q33) + q44;
}

/*
 * %%四元数相乘
 * Arguments    : const float q1[4]
 *                const float q2[4]
 *                float q[4]
 * Return Type  : void
 */
void my_quatmul(const float q1[4], const float q2[4], float q[4])
{
  /*     %%四元数相乘 */
  q[0] = ((q1[0] * q2[0] - q1[1] * q2[1]) - q1[2] * q2[2]) - q1[3] * q2[3];
  q[1] = ((q1[1] * q2[0] + q1[0] * q2[1]) - q1[3] * q2[2]) + q1[2] * q2[3];
  q[2] = ((q1[2] * q2[0] + q1[3] * q2[1]) + q1[0] * q2[2]) - q1[1] * q2[3];
  q[3] = ((q1[3] * q2[0] - q1[2] * q2[1]) + q1[1] * q2[2]) + q1[0] * q2[3];

  /*      %%进行归一化处理 */
  /*      normQ = norm(d); */
  /*      q = d/normQ; */
}

/*
 * File trailer for ekf_func_quad_6dof.c
 *
 * [EOF]
 */


/*
 * Arguments    : const float x[4]
 * Return Type  : float
 */
float norm(const float x[4])
{
  float y;
  float scale;
  int k;
  float absxk;
  float t;
  y = 0.0;
  scale = 3.3121686421112381E-170;
  for (k = 0; k < 4; k++) {
    absxk = fabs(x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0 + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrt(y);
}

