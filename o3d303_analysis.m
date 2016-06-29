% IFM o3d303 test
clc;
clear;

% image parameters
NumFrame = 40;
windowSize = 40;
NumRow = 132;
NumCol = 176;

% parameters
showPointCloud = 0;
showAvgPointCloud = 1;
showStd = 1;
drawSurface = 1;

% x = zeros(132,176,40);
% y = zeros(132,176,40);
% z = zeros(132,176,40);
x = zeros(176,132,40);
y = zeros(176,132,40);
z = zeros(176,132,40);

%range of interest
WallImageRow = [-windowSize/2 + NumRow/2 : -1 + windowSize/2 + NumRow/2]; % 132
WallImageCols = [-windowSize/2 + NumCol/2: -1 + windowSize/2 + NumCol/2]; % 176


% first calculate std deviation over time
WallDepth = zeros(length(WallImageRow),length(WallImageCols),NumFrame);

for imNum = 1:NumFrame
  
  % depth
  filename = strcat('Data/distances_wall_', num2str(imNum - 1), '.txt');
  Depth = importdata(filename);
  WallDepth(:,:,imNum) = Depth(WallImageRow,WallImageCols);
  
  % point cloud
  filename = strcat('Data/point_cloud_wall_', num2str(imNum - 1), '.txt');
  P = importdata(filename);
  
%   x(:,:,imNum) = reshape(P(:,1),[132,176]);
%   y(:,:,imNum) = reshape(P(:,2),[132,176]);
%   z(:,:,imNum) = reshape(P(:,3),[132,176]);
  x(:,:,imNum) = reshape(P(:,1),[176,132]);
  y(:,:,imNum) = reshape(P(:,2),[176,132]);
  z(:,:,imNum) = reshape(P(:,3),[176,132]);
end

% compute mean depth for every single pixel across time 
meanWallDepth = mean(WallDepth,3);

% compute std for every single pixel across time 
stdWall = std(WallDepth,0,3);

% compute the average std of the std for every single pixel across time 
std_Time = 1000*mean(stdWall(:))

% compute z range
depthRange =  1000*(max(meanWallDepth(:)) - min(meanWallDepth(:)))

% compute spatial std
std_Spatial = 1000*std(meanWallDepth(:))

% extract point cloud of interest region
xRoI = x(WallImageRow,WallImageCols,:);
yRoI = y(WallImageRow,WallImageCols,:);
zRoI = z(WallImageRow,WallImageCols,:);

xRoIMean = mean(xRoI,3);
yRoIMean = mean(yRoI,3);

% show point cloud
if (showPointCloud)
    
    figure(1)
    hold off
    
    px = xRoI(:,:,1);
    py = yRoI(:,:,1);
    pz = zRoI(:,:,1);
    scatter3(px(:),py(:),pz(:),'.','b')
    xlabel('x')
    ylabel('y')
    zlabel('z')
    axis('equal')
    title('point cloud _ first frame')
    rotate3d on;
end

% show average point cloud
if (showAvgPointCloud)
    
    figure(2)
    hold off
    
    px = xRoIMean*1000;
    py = yRoIMean*1000;
    pz = meanWallDepth*1000;
    
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
    xlabel('x')
    ylabel('y')
    zlabel('z')
    axis('equal')
    title('average point cloud')
    rotate3d on
    hold on
end

% show std_Time
if (showStd)
    
    figure(3)
    hold off
    
    px = xRoIMean;
    py = yRoIMean;
    pz = stdWall.*1000;

    h=surf(pz);
    set(h,'edgecolor','none')
    rotate3d on;
    colorbar;
    view(0,90);
    xlabel('x')
    ylabel('y')
    zlabel('std (mm)')
    title('standard deviation across time')
end

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