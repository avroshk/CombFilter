% MUSI 6202 in class exercise 4 - implement a biquad filter 
% CW @ GTCMT 2015
% objective: using the filter coefficient, implement the biquad filter
% [y] = myBiquadFilter(b, a, x)
% b = 3*1 vector, coefficients of numerator
% a = 3*1 vector, coefficients of denumerator
% x = L*1 vector, signal to be filtered
% y = L*1 vector, output signal

function [y] = myBiquadFilter(b, a, x)

% initialization 
b_zp = zeros(3, 1);
for i = 1:length(b)
    b_zp(i) = b(i);
end
a_zp = zeros(3, 1);
for i = 1:length(a)
    a_zp(i) = a(i);
end

L = length(x);
y = zeros(L, 1);

% filtering
for i = 1:L
    y(i) = -a_zp(2) * y(max(1, i-1)) - a_zp(3) * y(max(1, i-2)) + ...
        b_zp(1) * x(i) + b_zp(2) * x(max(1, i-1)) + b_zp(3) * x(max(1, i-2));    
end
