
figure(1);
hold off;

figure(2)
hold off;

%range of interest
CardboardImageRow = 195;
CardboardImageCols = [180:370];%[180:235 260:370];

AlImageRow = 207;
AlImageCols = [180:235 260:370];

% first calculate std deviation over time
CardboardDepth = zeros(40,length(CardboardImageCols));
AlDepth = zeros(40,length(AlImageCols));

for imNum = 1:40
  filename = sprintf('%d.tif',imNum);
  im = imread(filename);

  CardboardDepth(imNum,:) = double(im(CardboardImageRow,CardboardImageCols))*4000/65535;
  AlDepth(imNum,:) = double(im(AlImageRow,AlImageCols))*4000/65535;
end

meanCardboardDepth = mean(CardboardDepth);
meanAlDepth = mean(AlDepth);

stdCardboard = std(CardboardDepth);
stdAl = std(AlDepth);

meanStdCardboard = mean(std(CardboardDepth))
meanStdAl = mean(std(AlDepth))


XRes = 512;
tanHalfH = tan(pi*(70.6/2)/180);

meanCardboardX = zeros(size(meanCardboardDepth));
meanAlX = zeros(size(meanAlDepth));

for i=1:length(CardboardImageCols)
  meanCardboardX(i) = 2*meanCardboardDepth(i)*tanHalfH*(CardboardImageCols(i)/XRes - 0.5);
end  

for i=1:length(AlImageCols)
  meanAlX(i) = 2*meanAlDepth(i)*tanHalfH*(AlImageCols(i)/XRes - 0.5);
end

figure(1);
plot(meanCardboardX,meanCardboardDepth,'LineWidth',2);
hold on
plot(meanCardboardX, meanCardboardDepth+stdCardboard/2,'r','LineWidth',2);
plot(meanCardboardX, meanCardboardDepth-stdCardboard/2,'r','LineWidth',2);
xlabel('x (mm)', 'FontSize', 14);
ylabel('d (mm)', 'FontSize', 14);

figure(2);
plot(meanAlX,meanAlDepth,'LineWidth',2);
hold on;
plot(meanAlX, meanAlDepth+stdAl/2,'r','LineWidth',2);
plot(meanAlX, meanAlDepth-stdAl/2,'r','LineWidth',2);
xlabel('x (mm)', 'FontSize', 14);
ylabel('d (mm)', 'FontSize', 14);

R= eye(2);
%R = [cos(-pi/6) sin(-pi/6); -sin(-pi/6) cos(-pi/6)];
rotatedMeanAlCoordinates = R*[meanAlX;meanAlDepth];
rotatedMeanCardboardCoordinates = R*[meanCardboardX;meanCardboardDepth];

stdDeviationAlSpread = std(rotatedMeanAlCoordinates(2,:))
stdDeviationCardboardSpread = std(rotatedMeanCardboardCoordinates(2,:))