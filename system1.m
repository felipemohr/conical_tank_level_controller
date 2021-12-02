clear; close all; clc

pkg load control

R = 0.1325;
h2_ = 0.5;
H2 = 0.67;
k2 = 0.225;

H_conico = tf(1,[pi*R^2*h2_^2/(H2^2) k2/(2*sqrt(h2_))]);

kb = 0.26;
tau_p = 0.1;

H_bomba = tf([kb], [tau_p 1]);

H = H_bomba*H_conico;

[num den] = tfdata(H, 'v');
zeros = zero(H);
poles = pole(H);


xi = 0.7;                        % Coeficiente de amortecimento
ts = 120;                           % Tempo de acomodação 2%
wn = 4/(xi*ts);               % Frequência natural não amortecida
wd = wn*sqrt(1-xi^2);  % Frequência natural amortecida
sigma = xi*wn;

s1 = -sigma + wd*j;       % Posição desejada dos polos

zc2 = 0.1;                      % Raiz do controlador PI


% Calculando a condição angular
ztheta_pi = rad2deg(angle(s1+zc2)); % An
ptheta_pi = rad2deg(angle(s1));

ptheta_1 = rad2deg(angle(s1-poles(1)));
ptheta_2 = rad2deg(angle(s1-poles(2)));

excesso = abs(-ptheta_1 - ptheta_2 - ptheta_pi + ztheta_pi) - 180;

zc1 = sigma + wd/tand(excesso);    % Raiz do controlador PD
kc = abs(s1*(den(1)*s1^2 + den(2)*s1 + den(3))/((num(1))*(s1+zc1)*(s1+zc2)));

s = tf('s');
Gc = kc*(s+zc1)*(s+zc2)/s  % Controlador PID
%rlocus(Gc*H);



kp = kc*(zc1+zc2);
ki = kc*zc1*zc2;
kd = kc;


tfki=tf(ki,[1 0]);
tfkd=tf([kd 0],1);

tfdki=c2d(tfki,0.01,'zoh');
tfdkd=c2d(tfkd,0.01,'matched');

PID = pid(kp, ki, 0);
G = H*PID;
GF = feedback(G, 1);
