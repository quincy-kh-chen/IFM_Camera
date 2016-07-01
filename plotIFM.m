IFM = 0;

pointCloud = 1;
intensity = 0;
distance = 0;


if pointCloud == 1
    if ~IFM 
%     A = importdata('Data/pointCloud_room.txt');
    A = importdata('/Volumes/TRANSCEND/Energid/pointCloud/pointCloud_room1.txt');
    A = A.data;
    end
    
    if IFM
    A = importdata('Data/point_cloud_wall_0.txt');
    end
    
    A(:,1)=A(:,1)*1000;
    A(:,2)=A(:,2)*1000;
    A(:,3)=A(:,3)*1000;
    scatter3(A(:,1),A(:,2),A(:,3),'.','b')
    axis('equal')
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