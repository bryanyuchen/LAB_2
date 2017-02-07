%Forward Kinematics function for position:

function [x,y,z] = ForwardKinematicsLab2(t1,t2)

syms T1 T2;

T1 = t1;
T2 = t2;


T_1 = [cos(T1),-sin(T1),0,0;sin(T1),cos(T1),0,0;0,0,1,3/5;0,0,0,1];
T_2 = [sin(T2),cos(T2),0,27/2;0,0,-1,0;-cos(T2),sin(T2),0,0;0,0,0,1];
T_3 = [1,0,0,2;0,1,0,0;0,0,1,0;0,0,0,1];

T =  T_1 * T_2 * T_3
res = T*[0;0;0;1];
x = res(1);
y = res(2);
z = res(3);
end