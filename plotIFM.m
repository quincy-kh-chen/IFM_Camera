pointCloud = 0;
intensity = 1;
distance = 0;

if pointCloud == 1
    A = importdata('Data/point_cloud_wall_0.txt');
    A(:,1)=A(:,1);
    A(:,2)=A(:,2);
    A(:,3)=A(:,3);
    scatter3(A(:,1),A(:,2),A(:,3),'.','b')
    % hold on
end

if intensity == 1
    I = importdata('Data/intensity_wall_0.txt');
    scale = max(I(:));
    I = I./scale; 
    imshow(I)
end

if distance == 1
    D = importdata('Data/distances_wall_0.txt');
end



xlabel('x')
ylabel('y')
zlabel('z')
% axis('equal')
rotate3d on;