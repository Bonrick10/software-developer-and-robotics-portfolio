% Author: Beonrik Pascual

clear; clc;

startup_rvc; % Startup the rvc toolbox

load hershey;
    
scale = 0.04; % Select the scale of the digit. 1 = 100%, 0.1 = 10% scale

op_expr = '^\s*-?\d+\s*[\+\-\*]\s*-?\d+\s*$';
input_str = input("Enter 10 alphanumeric characters or a mathematical operation: ", 's');
is_operation = false;

if ~isempty(regexp(input_str, op_expr, 'once'))
    is_operation = true;
end

trans_expr = '^\s*-?\d+(\.\d+)?\s*-?\d+(\.\d+)?\s*-?\d+(\.\d+)?\s*$';
transform = '';
while isempty(regexp(transform, trans_expr, 'once'))
    transform = input("Enter transformation data [x(mm)] [y(mm)] [rot(deg)]: ", 's');
end

tokens = regexp(transform, '(-?\d+(\.\d+)?)\s*(-?\d+(\.\d+)?)\s*(-?\d+(\.\d+)?)', 'tokens');
    
parts = tokens{1};

trans_x = str2double(parts{1});
trans_y = str2double(parts{2});
rot_ang = deg2rad(str2double(parts{3}));

traj = [];
offset_x = 0;
kerning_x = 0.009;
lift = 0.015;

if is_operation
    tokens = regexp(input_str, '(-?\d+)\s*([\+\-\*])\s*(-?\d+)', 'tokens');
    
    parts = tokens{1};

    num1 = str2double(parts{1});
    op = parts{2};
    num2 = str2double(parts{3});

    switch op
        case '+'
            num3 = num1 + num2;
        case '-'
            num3 = num1 - num2;
        case '*'
            num3 = num1 * num2;
    end

    line1 = num2str(num1);  
    line2 = num2str(num2);
    line3 = num2str(num3);

    disp("Writing: ");
    disp(line1);
    disp([line2 op]);
    disp(line3);
    
    kerning_y = 12;
    
    line_traj1 = getMathLine(line1, scale, lift, kerning_x, hershey, 0);
    num_height = max(line_traj1(:,2)) - min(line_traj1(:,2));
    bottom = min(line_traj1(:,2));
    
    line_traj2 = getMathLine(line2, scale, lift, kerning_x, hershey, length(line1)-length(line2));
    top = max(line_traj2(:,2));
    
    line_traj2(:,2) = line_traj2(:,2) - top + bottom - kerning_y;
    bottom = min(line_traj2(:,2));

    switch op
        case '+'
            character = hershey{'+'};
            path = getPath(character, scale, lift);
        case '-'
            character = hershey{'-'};
            path = getPath(character, scale, lift);
        case '*'
            character = hershey{'+'};
            path = getPath(character, scale, lift);
            theta = -pi/4;
            R = [cos(theta) -sin(theta) 0;
                 sin(theta)  cos(theta) 0;
                 0           0          1];
            path = R * path;
    end
    
    % centre op
    op_height = max(path(2,:)) - min(path(2,:));
    op_left = min(path(1,:));
    line2_right = max(line_traj2(:,1));
    path(2,:) = path(2,:) + (bottom/1000 - min(path(2,:))) + (num_height/1000 - op_height) / 2;
    path(1,:) = path(1,:) + line2_right/1000 - op_left + kerning_x;
    op_traj = [path'*1000];
    

    line_traj2 = [line_traj2; op_traj];

    line_traj3 = getMathLine(line3, scale, lift, kerning_x, hershey, length(line1)-length(line3));
    curr_align_right = max(line_traj3(:,1));
    top = max(line_traj3(:,2));

    line_traj3(:,2) = line_traj3(:,2) - top + bottom - kerning_y;
    
    traj = [line_traj1; line_traj2; line_traj3];
else
    disp("Writing: ");
    disp(input_str);
    
    traj = getLine(input_str, scale, lift, kerning_x, hershey);
end
scatter3(traj(:,1), traj(:,2), traj(:,3));
plot3(traj(:,1), traj(:,2), traj(:,3));
axis equal;

% Apply transformation here:
trans_traj = traj';
z_base = 27.3;
first_point = trans_traj(1:2,1);

% Set first stroke to 'origin'
trans_traj(1,:) = trans_traj(1,:) - first_point(1);
trans_traj(2,:) = trans_traj(2,:) - first_point(2);
trans_traj(3,:) = trans_traj(3,:) + z_base;


% Rotate first two rows (About Z-axis)
rot_ang = rot_ang + pi/2;
R = [cos(rot_ang) -sin(rot_ang) 0;
     sin(rot_ang)  cos(rot_ang) 0;
     0             0            1];

trans_traj = R * trans_traj;

% Set Translation
trans_traj(1,:) = trans_traj(1,:) + trans_x;
trans_traj(2,:) = trans_traj(2,:) + trans_y;

traj = trans_traj';


% Generate a plot of what we are expecting
scatter3(traj(:,1), traj(:,2), traj(:,3));
plot3(traj(:,1), traj(:,2), traj(:,3));
xlabel('X-axis');
ylabel('Y-axis');
zlabel('Z-axis');

axis equal;


%% NOW USE THE RTDE TOOLBOX TO EXECUTE THIS PATH!


% % TCP Host and Port settings
% host = '127.0.0.1'; % THIS IP ADDRESS MUST BE USED FOR THE VIRTUAL BOX VM
% host = '192.168.230.128'; % THIS IP ADDRESS MUST BE USED FOR THE VMWARE
host = '192.168.0.100'; % THIS IP ADDRESS MUST BE USED FOR THE REAL ROBOT
port = 30003;
% 

% Calling the constructor of rtde to setup tcp connction
rtde = rtde(host,port);

% Setting home
home = [-588.53, -133.30, 227.00, 2.221, 2.221, 0.00];
startPose = [-588.53, -350, 0];

rtde.movej(home);
% Creating a path array
path = [];

% setting move parameters
v = 0.5;
a = 1.2;
blend = 0.0005;

% Populate the path array
for i = 1:length(traj)
    disp(i);
    disp(traj(i,1:3) + startPose);
    point = [[(traj(i,1:3) + startPose),(home(4:6))],a,v,0,blend];
    if isempty(path)
        path = point;
    else
        path = cat(1,path,point);
    end
end
point = [[(home(1:6))],a,v,0,blend];
if isempty(path)
    path = point;
else
    path = cat(1,path,point);
end

% Execute the movement!
poses = rtde.movej(path);

rtde.close;

function path = getPath(character, scale, lift)
    path = [scale*character.stroke; zeros(1,numcols(character.stroke))];
    
    % Where ever there is an nan it indicates that we need to lift up.
    k = find(isnan(path(1,:)));
    
    path(:,k) = path(:,k-1); path(3,k) = lift;
    
    % Add a lift for the next point
    for i=numel(k):-1:1
        idx = k(i);
        next_point = path(:,idx+1);
        path = [path(:,1:idx), path(:,idx), path(:,idx+1:end)];
        path(:,idx+1) = next_point; path(3,idx+1) = lift;
    end
    % Add a lift at the start and end of a character
    path = [[path(1,1); path(2,1); lift], path];
    path = [path, [path(1,end); path(2,end); lift]];
end

function traj = getLine(input_str, scale, lift, kerning_x, font)
    traj = [];
    curr_right = 0;
    for i=1:length(input_str)
        character = font{input_str(i)};
        path = getPath(character, scale, lift);
        char_left = min(path(1,:));
        if i ~= 1
            path(1,:) = path(1,:) + curr_right - char_left + kerning_x;
        end
        
        char_traj = [path'*1000];
        traj = [traj; char_traj];
        curr_right = max(traj(:,1))/1000;
    end
end

function traj = getMathLine(input_str, scale, lift, kerning_x, font, block_offset)
    math_width = 0.8*scale;
    math_block = kerning_x + math_width; 
    curr_block = block_offset;
    traj = [];
    for i=1:length(input_str)
        character = font{input_str(i)};
        path = getPath(character, scale, lift);
        char_left = min(path(1,:));

        path(1,:) = path(1,:) + char_left + math_block * curr_block;
        
        char_traj = [path'*1000];
        traj = [traj; char_traj];
        curr_block = curr_block + 1;
    end
end