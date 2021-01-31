/*
 * driveContol
 *    * Inputs: 
 *      * Throttle
 *      * Vehicle Speed
 *    * Calculate
 *      * Control effort
 *      * Map effort to range of outputs
 *    * Output
 *      * Vehicle throttle
 *      * Steering position
 * 
 */
// Dependencies:
/*
 * g_Input
 * g_Setpoint
 * g_Output
 * g_maxEffort
 * g_minEffort
 * g_wideOpenReverse
 * g_wideOpenThrottle
 * g_steering 
 * g_minRcRange
 * g_maxRcRange
 * g_fullRight
 * g_fullLeft
 */
void pidControl(int throttleInput){
double effort = 0;

    g_Input = g_avgSpeed;           // Input as vehicle speed
    g_Setpoint = throttleInput;   // Setpoint as throttle (RC/Auto)
    speedPID.Compute();
    effort = g_Output;            // Control effort as output

    // Scaling effort
    if (effort > g_maxEffort) {
      effort = g_maxEffort;
    }
    else if (effort < g_minEffort) {
      effort = g_minEffort;
    }

    // Mapping control effort to PWM range
    g_throttle = map(effort,g_minEffort,g_maxEffort,g_wideOpenReverse,g_wideOpenThrottle);
}


void steeringControl(int steeringInput){
  g_steering = map(steeringInput,g_minRcRange,g_maxRcRange,g_fullRight,g_fullLeft);
  
}

  
