m = 10;     % [kg]    Mass of vehicle
b = 2;      % [N*s/m] Representing drag forces, rolling resistance (assuming linear)

s = tf('s');
P_cruise = 1/(m*s + b);

%% Proportional Control

Kp = 20;
C = Kp;

T = feedback(C*P_cruise,1);

t = 0:0.1:20;
r = 10; 
step(r*T,t)
axis([0 20 0 15])

%% PI Control

Kp = 20;
Ki = 2;
C = (Kp + Ki/s);

T = feedback(C*P_cruise,1);

t = 0:0.1:20;       % seconds
r = 10;             % m/s
step(r*T,t)
axis([0 20 0 15])


%% PID Control
Kp = 20;
Ki = 2;
Kd = 2;
C = (Kp + Ki/s + Kd*s);

T = feedback(C*P_cruise,1);

t = 0:0.1:20;       % seconds
r = 10;             % m/s
step(r*T,t)
axis([0 20 0 15])
