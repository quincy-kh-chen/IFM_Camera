% IFM o3d303 test
clc;
clear;

% load data parameter
dist = 15; % meter
angle = 0;
object = 'wall';
center = 0;
%==========================================================
% rotation along y
applyRotationY = 1;
rotateY = 2 * pi/180;
R_Y = [cos(rotateY) 0 sin(rotateY) ; 0 1 0 ; -sin(rotateY) 0 cos(rotateY)];

% rotation along x
applyRotationX = 1;
rotateX = 3 * pi/180;
R_X = [1 0 0 ; 0 cos(rotateX) -sin(rotateX) ; 0 sin(rotateX) cos(rotateX)];

% rotation along z
% R = [cos(rotate), -sin(rotate), 0 ; sin(rotate), cos(rotate),0 ; 0 0 1];

applyZOffset =1;
actualDepth = 1500;
%----------------------------------------------------------


% image parameters
NumFrame = 40;
windowSize = 40;
NumRow = 132;
NumCol = 176;

% parameters
showPointCloud = 1;
showAvgPointCloud = 1;
showSurface = 1;
showStd = 1;
drawSurface = 1;
showStdSpatial = 1;

% x = zeros(176,132,40);
% y = zeros(176,132,40);
% z = zeros(176,132,40); 

x = zeros(NumCol,NumRow,40);
y = zeros(NumCol,NumRow,40);
z = zeros(NumCol,NumRow,40); 
avgPointCloud = zeros(NumCol,NumRow,3); 


%range of interest
% WallImageRow = [-windowSize/2 + NumRow/2 : -1 + windowSize/2 + NumRow/2]; % 132
% WallImageCols = [-windowSize/2 + NumCol/2: -1 + windowSize/2 + NumCol/2]; % 176
WallImageRow = [-windowSize/2 + NumCol/2 + floor(1.5*center) : -1 + windowSize/2 + NumCol/2 + floor(1.5*center)]; % 132
WallImageCols = [-windowSize/2 + NumRow/2 + floor(0*center) : -1 + windowSize/2 + NumRow/2 + floor(0*center)]; % 176

% first calculate std deviation over time
WallDepth = zeros(length(WallImageRow),length(WallImageCols),NumFrame);

for imNum = 1:NumFrame
  
  % % depth
  % filename = strcat('/Volumes/TRANSCEND/Energid/pointCloud/IFM/wall/distances_distance_',num2str(dist),'m_',num2str(angle),'degree_', num2str(imNum - 1), '.txt');
  % % filename = strcat('Data/distances_wall_', num2str(imNum - 1), '.txt');
  % Depth = importdata(filename);
  % WallDepth(:,:,imNum) = 1000* Depth(WallImageRow,WallImageCols);
  
  % point cloud
  filename = strcat('/Volumes/TRANSCEND/Energid/pointCloud/IFM/',object,'/pointCloud_distance_',num2str(dist),'m_',num2str(angle),'degree_', num2str(imNum - 1), '.txt');
%   filename = strcat('Data/point_cloud_wall_', num2str(imNum - 1), '.txt');
  P = importdata(filename);
  P = 1000* P;

  % apply rotation
  if applyRotationY
      P = (R_Y*P')';
  end

  if applyRotationX
      P = (R_X*P')';
  end

  x(:,:,imNum) = reshape(P(:,1),[NumCol,NumRow]);
  y(:,:,imNum) = reshape(P(:,2),[NumCol,NumRow]);
  z(:,:,imNum) = reshape(P(:,3),[NumCol,NumRow]);
  
  tempDepth = z(:,:,imNum);
  WallDepth(:,:,imNum) = tempDepth(WallImageRow,WallImageCols);

end

% R= eye(2);
% %R = [cos(-pi/6) sin(-pi/6); -sin(-pi/6) cos(-pi/6)];
% rotatedMeanAlCoordinates = R*[meanAlX;meanAlDepth];
% rotatedMeanWallCoordinates = R*[meanWallX;meanWallDepth];


% compute average point cloud
avgPointCloud (:,:,1) = mean(x,3);
avgPointCloud (:,:,2) = mean(y,3);
avgPointCloud (:,:,3) = mean(z,3);

% apply z offset 
zAverage = mean(z(:));
zOffset = zAverage - actualDepth;
if applyZOffset
   avgPointCloud (:,:,3) - zOffset;
   WallDepth = WallDepth - zOffset;
end

%==========================================================
% compute mean depth for every single pixel across time 
meanWallDepth = mean(WallDepth,3);

% compute std for every single pixel across time 
stdWall = std(WallDepth,0,3);

% compute the average std of the std for every single pixel across time 
std_Time = mean(stdWall(:))

% compute z range
depthRange =  (max(meanWallDepth(:)) - min(meanWallDepth(:)))

% compute spatial std
std_Spatial = std(meanWallDepth(:))
%----------------------------------------------------------


%==========================================================
% extract point cloud of interest region
xRoI = x(WallImageRow,WallImageCols,:);
yRoI = y(WallImageRow,WallImageCols,:);
zRoI = z(WallImageRow,WallImageCols,:);

xRoIMean = mean(xRoI,3);
yRoIMean = mean(yRoI,3);
zRoIMean = mean(zRoI,3);

% ouput the x y range for finding the pixel indices of Kinect
xMin = min(xRoIMean(:)) 
xMax = max(xRoIMean(:))
yMin = min(yRoIMean(:)) 
yMax = max(yRoIMean(:))
%----------------------------------------------------------


%==========================================================
% show point cloud
if (showPointCloud)
    
    figure(1)
    hold off
    
    % draw whole point cloud
    px = avgPointCloud(:,:,1);
    py = avgPointCloud(:,:,2);
    pz = avgPointCloud(:,:,3);
    scatter3(px(:),py(:),pz(:),'.','b')

    xlabel('x(mm)')
    ylabel('y(mm)')
    zlabel('z(mm)')
    axis('equal')
    title('average point cloud')
    hold on;
    rotate3d on;
 
   % draw RoI
    px = xRoIMean;
    py = yRoIMean;
    pz = zRoIMean;
    scatter3(px(:),py(:),pz(:),'.','r')

end
%----------------------------------------------------------

%==========================================================
% show average point cloud
if (showSurface)
    
    figure(2)
    hold off
    
    px = xRoIMean;
    py = yRoIMean;
    pz = meanWallDepth;
    
    if(drawSurface)
        [xIdeal, yIdeal] = meshgrid(min(px(:)):max(px(:)),min(py(:)):max(py(:)));
        zIdeal = ones(size(xIdeal)) * mean(pz(:));
        h = surf(xIdeal,yIdeal,zIdeal);
        set(h,'edgecolor','none')
        alpha(.4)
    end  
%     scatter3(px(:),py(:),pz(:),'.','b')
    h = surface(px,py,pz);
    set(h,'edgecolor','none')
    colorbar;
    view(67,18);
    xlabel('x(mm)')
    ylabel('y(mm)')
    zlabel('z(mm)')
    axis('equal')
    title('average reconstructed surface')
    rotate3d on
    hold on
end
%----------------------------------------------------------

%==========================================================
% show std_Time
if (showStd)
    
    figure(3)
    hold off
    
    px = xRoIMean;
    py = yRoIMean;
    pz = stdWall;

    h=surf(px,py,pz);
    set(h,'edgecolor','none')
    rotate3d on;
    colorbar;
    view(0,90);
    xlabel('x(mm)')
    ylabel('y(mm)')
    zlabel('std (mm)')
    title('standard deviation across time')
    zlim([0 2.5]);

    figure(4)
    hold off
    h=surf(px,py,pz);
    set(h,'edgecolor','none')
    rotate3d on;
    colorbar;
    view(-31,34);
    xlabel('x(mm)')
    ylabel('y(mm)')
    zlabel('std (mm)')
    title('standard deviation across time')
    zlim([0 2.5]);
end
%----------------------------------------------------------

%==========================================================
% show std_Time
if (showStdSpatial)
    
    figure(5)
    hold off
    % compute spatial std
    px = xRoIMean;
    py = yRoIMean;
    pz = meanWallDepth;

    h=surf(px,py,pz);
    set(h,'edgecolor','none')
    rotate3d on;
    colorbar;
    view(-31,34);
    xlabel('x(mm)')
    ylabel('y(mm)')
    zlabel('z(mm)')
    title('spatial standard deviation')
    hold on 

    % plot std
    px = xRoIMean;
    py = yRoIMean;
    pz = meanWallDepth + std_Spatial;
    h=surf(px,py,pz);
    set(h,'edgecolor','none')
    alpha(.4)

    px = xRoIMean;
    py = yRoIMean;
    pz = meanWallDepth - std_Spatial;

    h=surf(px,py,pz);
    set(h,'edgecolor','none')
    alpha(.4)

    if(drawSurface)
        [xIdeal, yIdeal] = meshgrid(min(px(:)):max(px(:)),min(py(:)):max(py(:)));
        zIdeal = ones(size(xIdeal)) * actualDepth;
        h = surf(xIdeal,yIdeal,zIdeal);
        set(h,'edgecolor','none')
        alpha(.4)
    end  
end
%----------------------------------------------------------
xRange = xMax - xMin
yRange = yMax - yMin

% XRes = 512;
% tanHalfH = tan(pi*(70.6/2)/180);

% meanWallX = zeros(size(meanWallDepth));
% meanAlX = zeros(size(meanAlDepth));

% for i=1:length(WallImageCols)
%   meanWallX(i) = 2*meanWallDepth(i)*tanHalfH*(WallImageCols(i)/XRes - 0.5);
% end  

% for i=1:length(AlImageCols)
%   meanAlX(i) = 2*meanAlDepth(i)*tanHalfH*(AlImageCols(i)/XRes - 0.5);
% end


% figure(1);
% plot(meanWallX,meanWallDepth,'LineWidth',2);
% hold on
% plot(meanWallX, meanWallDepth+stdWall/2,'r','LineWidth',2);
% plot(meanWallX, meanWallDepth-stdWall/2,'r','LineWidth',2);
% xlabel('x (mm)', 'FontSize', 14);
% ylabel('d (mm)', 'FontSize', 14);

% figure(2);
% plot(meanAlX,meanAlDepth,'LineWidth',2);
% hold on;
% plot(meanAlX, meanAlDepth+stdAl/2,'r','LineWidth',2);
% plot(meanAlX, meanAlDepth-stdAl/2,'r','LineWidth',2);
% xlabel('x (mm)', 'FontSize', 14);
% ylabel('d (mm)', 'FontSize', 14);

% R= eye(2);
% %R = [cos(-pi/6) sin(-pi/6); -sin(-pi/6) cos(-pi/6)];
% rotatedMeanAlCoordinates = R*[meanAlX;meanAlDepth];
% rotatedMeanWallCoordinates = R*[meanWallX;meanWallDepth];

% stdDeviationAlSpread = std(rotatedMeanAlCoordinates(2,:))
% stdDeviationWallSpread = std(rotatedMeanWallCoordinates(2,:))