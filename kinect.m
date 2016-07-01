clc;
clear;

%==========================================================
% get region of interest from IFM
% xMin = -747.6228
% xMax = -432.0003
% yMin = -157.9750
% yMax = 124.6000

% xMin = -121
% xMax = 123
% yMin = -194.4135
% yMax = 50.7453

xMin =-1.1500e+03


xMax =-679.9571


yMin =-275.2474


yMax =149.3591

xRange = xMax - xMin
yRange = yMax - yMin

% load data parameter
dist = 15; % meter
angle = 30;
object = 'wall';
%----------------------------------------------------------

%==========================================================
% rotation
applyRotationY = 1;
rotateY = -31.5 * pi/180;
% rotation along y
R_Y = [cos(rotateY) 0 sin(rotateY) ; 0 1 0 ; -sin(rotateY) 0 cos(rotateY)];

% rotation along x
applyRotationX = 1;
rotateX = -2 * pi/180;
R_X = [1 0 0 ; 0 cos(rotateX) -sin(rotateX) ; 0 sin(rotateX) cos(rotateX)];
%----------------------------------------------------------

%==========================================================
% image parameters
NumFrame = 40;
showPointCloud = 1;
drawSurface = 1;
showPointCloud_RoI = 1;
showSurface = 1;
showStd = 1;
row = 424;
col = 512;
pointCloud = zeros(row,col,3,NumFrame);
%----------------------------------------------------------

%==========================================================
% load data
for imNum = 1:NumFrame
	
% 	filename = strcat('/Volumes/TRANSCEND/Energid/pointCloud/kinect/kinect_wall', num2str(imNum - 1), '.txt');
    filename = strcat('/Volumes/TRANSCEND/Energid/pointCloud/kinect/',object,'/distance_',num2str(dist),'m_',num2str(angle),'degree_', num2str(imNum - 1), '.txt');
	
	P = importdata(filename);
	P = P.data;

	% row = size(P,1);
	% col = size(P,2)/3;
	% pointCloud = zeros(row,col,3);

	% for yy = 1:row
	%     for xx = 1:col
	%         pointCloud(yy,xx,1,imNum) = P(yy, 1 + (xx - 1)*3 );
	%         pointCloud(yy,xx,2,imNum) = P(yy, 2 + (xx - 1)*3 );
	%         pointCloud(yy,xx,3,imNum) = P(yy, 3 + (xx - 1)*3 );
	%     end
	% end
	
    xTemp =  P(:,1:3:end);
    yTemp =  P(:,2:3:end);
    zTemp =  P(:,3:3:end);

    xTemp = reshape(xTemp,1,[]);
    yTemp = reshape(yTemp,1,[]);
    zTemp = reshape(zTemp,1,[]);
    
    pointCloud_Temp = [xTemp ; yTemp ; zTemp];
	
	% apply rotation
	if applyRotationY
	    pointCloud_Temp = (R_Y*pointCloud_Temp);
    end
	if applyRotationX
	    pointCloud_Temp = (R_X*pointCloud_Temp);
    end

    xTemp =  pointCloud_Temp(1,:);
    yTemp =  pointCloud_Temp(2,:);
    zTemp =  pointCloud_Temp(3,:);

    pointCloud(:,:,1,imNum) = reshape(xTemp,[row col]);
    pointCloud(:,:,2,imNum) = reshape(yTemp,[row col]);
    pointCloud(:,:,3,imNum) = reshape(zTemp,[row col]);

end

pointCloud = pointCloud * 1000;

% compute average point cloud
avgPointCloud = mean(pointCloud,4);
%----------------------------------------------------------

%==========================================================
% find region of interest
xCoord = mean( avgPointCloud(:,:,1), 1 );
yCoord = mean( avgPointCloud(:,:,2), 2 );

% find indices
[~,xMin_Idx] = min(abs(xCoord-xMin));
[~,xMax_Idx] = min(abs(xCoord-xMax));

[~,yMin_Idx] = min(abs(yCoord-yMin));
[~,yMax_Idx] = min(abs(yCoord-yMax));

%range of interest
colRoI = [xMax_Idx : xMin_Idx];
rowRoI = [yMin_Idx : yMax_Idx];

% extract region of interest
avgPointCloud_RoI = avgPointCloud(rowRoI,colRoI,:);
%----------------------------------------------------------

%==========================================================
% compute mean depth for every single pixel across time 
% meanWallDepth = mean(WallDepth,3);
meanDepth = avgPointCloud_RoI(:,:,3);

% compute std for every single pixel across time 
depth_RoI = pointCloud(rowRoI,colRoI,3,:);
std_RoI = std(depth_RoI,0,4);

% compute the average std of the std for every single pixel across time 
std_Time = mean(std_RoI(:))

% compute z range
depthRange =  (max(meanDepth(:)) - min(meanDepth(:)))

% compute spatial std
std_Spatial = std(meanDepth(:))
%----------------------------------------------------------

%========================================================== 
% show average point cloud
if showPointCloud
	% figure(1)
	% hold off
	% % pointCloud = pointCloud * 1000;
	% px = pointCloud(:,:,1,1);
	% py = pointCloud(:,:,2,1);
	% pz = pointCloud(:,:,3,1);
	% scatter3(px(:),py(:),pz(:),'.','b')
	% axis('equal')
	% xlabel('x')
	% ylabel('y')
	% zlabel('z')
	% rotate3d on;
	% hold on

	figure(1)
    hold off
    
	px = avgPointCloud(:,:,1);
	py = avgPointCloud(:,:,2);
	pz = avgPointCloud(:,:,3);

	scatter3(px(:),py(:),pz(:),'.','b')

    xlabel('x')
    ylabel('y')
    zlabel('z')
    axis('equal')
    title('average point cloud')
    rotate3d on
    hold on
end

% show point cloud in interest region
if showPointCloud_RoI
    
    figure(1)
	px = avgPointCloud_RoI(:,:,1);
	py = avgPointCloud_RoI(:,:,2);
	pz = avgPointCloud_RoI(:,:,3);
	scatter3(px(:),py(:),pz(:),'.','r')
	axis('equal')
	xlabel('x')
	ylabel('y')
	zlabel('z')
	rotate3d on;
	hold on
  
end
%----------------------------------------------------------

%==========================================================
% show surfce of RoI
if showSurface

	figure(2)
    hold off
    
	px = avgPointCloud_RoI(:,:,1);
	py = avgPointCloud_RoI(:,:,2);
	pz = avgPointCloud_RoI(:,:,3);
    
    if(drawSurface)
        [xIdeal, yIdeal] = meshgrid(min(px(:)):max(px(:)),min(py(:)):max(py(:)));
        zIdeal = ones(size(xIdeal)) * mean(pz(:));
        h = surf(xIdeal,yIdeal,zIdeal);
        set(h,'edgecolor','none')
        alpha(.4)
    end  

    h = surface(px,py,pz);
    set(h,'edgecolor','none')
    colorbar;
    view(67,18);
    xlabel('x')
    ylabel('y')
    zlabel('z')
    axis('equal')
    title('average point cloud')
    rotate3d on
end
%----------------------------------------------------------

%==========================================================
% show std_Time
if (showStd)
    
    figure(3)
    hold off
    
    px = avgPointCloud_RoI(:,:,1);
	py = avgPointCloud_RoI(:,:,2);
    pz = std_RoI;

    h=surf(pz);
    set(h,'edgecolor','none')
    rotate3d on;
    colorbar;
    view(0,90);
    xlabel('x')
    ylabel('y')
    zlabel('std (mm)')
    title('standard deviation across time')
    zlim([0 2.5]);

end
%----------------------------------------------------------

