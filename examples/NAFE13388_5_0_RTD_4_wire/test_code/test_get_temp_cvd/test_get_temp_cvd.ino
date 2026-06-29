/*
 * Unit test for get_temp_cvd() — no hardware required.
 *
 * Verifies the Callendar-Van Dusen conversion against IEC 60751
 * reference (temperature, resistance) pairs for Pt100, and checks
 * how many Newton-Raphson iterations are needed to converge in the
 * negative-temperature branch.
 *
 * This only tests the double-argument overloads (get_temp(double),
 * get_temp_cvd(double)), so no AFE / SPI hardware is touched.
 */

#include <math.h>

double get_temp(double resistance) {
  static constexpr double coef_pt100 = 0.385;
  return (resistance - 100) / coef_pt100;
}

//  iterations is exposed as a parameter here (vs. fixed at 5 in the
//  library code) so the convergence test below can sweep it
double get_temp_cvd(double resistance, int iterations = 5) {
  static constexpr double R0 = 100.0;
  static constexpr double A = 3.9083e-3;
  static constexpr double B = -5.775e-7;
  static constexpr double C = -4.183e-12;

  if (resistance >= R0) {
    double ratio = resistance / R0;
    return (-A + sqrt(A * A - 4.0 * B * (1.0 - ratio))) / (2.0 * B);
  }

  double t = (resistance - R0) / (R0 * 0.00385);

  for (int i = 0; i < iterations; i++) {
    double f = R0 * (1.0 + A * t + B * t * t + C * (t - 100.0) * t * t * t) - resistance;
    double df = R0 * (A + 2.0 * B * t + C * (4.0 * t * t * t - 300.0 * t * t));
    t -= f / df;
  }

  return t;
}

//  IEC 60751 reference table for Pt100 (temperature[degC], resistance[ohm])
//  Values taken from the standard's resistance table, 10 degC steps
struct ref_point {
  double temp;
  double resistance;
};

ref_point reference_table[] = {
  { -50.0,  80.31 },
  { -40.0,  84.27 },
  { -30.0,  88.22 },
  { -20.0,  92.16 },
  { -10.0,  96.09 },
  {   0.0, 100.00 },
  {  10.0, 103.90 },
  {  20.0, 107.79 },
  {  30.0, 111.67 },
  {  40.0, 115.54 },
  {  50.0, 119.40 },
  {  60.0, 123.24 },
  {  70.0, 127.07 },
  {  80.0, 130.89 },
  {  90.0, 134.70 },
  { 100.0, 138.50 },
  { 110.0, 142.29 },
  { 120.0, 146.06 },
  { 130.0, 149.82 },
  { 140.0, 153.58 },
  { 150.0, 157.31 },
};

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("\n***** get_temp_cvd() unit test *****\n");

  //  ---------------------------------------------------------------
  //  Test 1: CVD output vs IEC 60751 reference table, -50 to 150 degC
  //  ---------------------------------------------------------------
  Serial.println("Test 1: CVD vs IEC 60751 reference table");
  Serial.println("ref_temp, ref_R, cvd_temp, error(degC), linear_temp, linear_error(degC)");

  double max_cvd_error = 0.0;
  double max_linear_error = 0.0;

  for (auto &p : reference_table) {
    double cvd_temp = get_temp_cvd(p.resistance);
    double linear_temp = get_temp(p.resistance);

    double cvd_error = cvd_temp - p.temp;
    double linear_error = linear_temp - p.temp;

    if (fabs(cvd_error) > max_cvd_error)
      max_cvd_error = fabs(cvd_error);
    if (fabs(linear_error) > max_linear_error)
      max_linear_error = fabs(linear_error);

    Serial.print(p.temp, 1);
    Serial.print(", ");
    Serial.print(p.resistance, 2);
    Serial.print(", ");
    Serial.print(cvd_temp, 4);
    Serial.print(", ");
    Serial.print(cvd_error, 4);
    Serial.print(", ");
    Serial.print(linear_temp, 4);
    Serial.print(", ");
    Serial.println(linear_error, 4);
  }

  Serial.print("\nmax |CVD error|    = ");
  Serial.print(max_cvd_error, 5);
  Serial.println(" degC");
  Serial.print("max |linear error| = ");
  Serial.print(max_linear_error, 5);
  Serial.println(" degC");

  //  ---------------------------------------------------------------
  //  Test 2: Newton-Raphson convergence, negative-temperature branch
  //  ---------------------------------------------------------------
  Serial.println("\nTest 2: Newton-Raphson convergence (T < 0 degC)");
  Serial.println("iterations, temp_at(-50C), temp_at(-30C), temp_at(-10C)");

  for (int iter = 0; iter <= 5; iter++) {
    Serial.print(iter);
    Serial.print(", ");
    Serial.print(get_temp_cvd(80.31, iter), 6);
    Serial.print(", ");
    Serial.print(get_temp_cvd(88.22, iter), 6);
    Serial.print(", ");
    Serial.println(get_temp_cvd(96.09, iter), 6);
  }

  Serial.println("\n(Compare successive rows above -- once the value");
  Serial.println(" stops changing within your required precision,");
  Serial.println(" that's the iteration count actually needed.)");
}

void loop() {
}
