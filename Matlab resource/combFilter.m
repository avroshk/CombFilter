% Using the filter coefficient, implement an FIR and IIR comb filter
% [y] = combFilter(x, a, b, z)
% y = L*1 vector, output signal
% x = L*1 vector, signal to be filtered
% a = FIR gain coefficient 
% b = IIR gain coefficient
% z = delay in number of samples

function [y] = combFilter(x, a, b, z)
L = length(x);
y = zeros(L, 1);

for i = 1:z
    y(i) = x(i);
end

% filtering
for i = z+1:L
    y(i) = x(i) + a * x(max(1, i-z)) + b * y(max(1, i-z));  
end
