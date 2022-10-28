%question 2d
% set parameter
N = 1000;
h = 0.01; %s
v = 1; %m/s
w_min = -pi/4; %[rad/s]
w_max = pi/4; %[rad/s]
y_0 = 1.5; %[m]
h_0 = 0; %[rad]
r = 1;
Q = [[1, 0]; [0 1]]; % Identity matrix
% Define cost function
H = sparse(diag(ones(N,1)*r));
for i = 1:N
    H = blkdiag(H,Q);
end
H = 2*H;
% Define inequality constrain
A_1 = [sparse(diag(-ones(N,1))); diag(ones(N,1))];
A = [A_1 zeros(2*N, 2*N)];
b = [-1*ones(N,1)*w_min; ones(N,1)*w_max];
% Construct the w
A_left = (zeros(2*N,N));
for i= 1:N
    A_left(2*i, i) = -h;
end
%construct the y_k and h_k
d1 = sparse([0, repmat([-h*v,0],1,N-1)]);
d2 = -1*ones(2*N-2,1);
A_right = (eye(2*N)+diag(d1,-1)+diag(d2,-2));
Aq = [A_left,A_right];
bq = [y_0+h*v*h_0;h_0;zeros(2*N-2,1)];
f = zeros(3*N,1);
% now, use quadprog to solve the problem
[x_op fval] = quadprog(H,f,A,b,Aq,bq);
% find optimal value for all
op = reshape(x_op(N+1:3*N),[2,N]);
y_op = op(1,:);
h_op = op(2,:);
% now graph y_k and h_k
t = (0:N)*h;
plot(t,[y_0,y_op])
title("y possition")
xlabel("time [t]")
ylabel("y [m]")
plot(t,[h_0,h_op])
title("Heading of the Robot")
xlabel("time [t]")
ylabel("h [rad]")

% Question f
y = zeros(1, N+1);
head = zeros(1,N+1);
y(1) = y_0;
head(1) = h_0;
for i = 1:N
    y(i+1) = y(i)+h*v*head(i);
    head(i+1) = head(i) + h*w_op(i) +(h*pi/25);
end
plot(t,y)
title("y possition")
xlabel("time [t]")
ylabel("y [m]")
plot(t,head)
title("Heading of the Robot")
xlabel("time [t]")
ylabel("h [rad]")
