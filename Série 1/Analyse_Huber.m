% Nom du fichier d'output a analyser
filename = 'output.out';


width = 12;
height = 8.5;
alw = 0.75;
fsz = 11;
msz = 8;
lw = 1.5;


% Chargement des donnees
data = load(filename);

% Extraction des quantites d'interet
% (Le code c++ ecrit t, z(t) et v(t) en colonnes.)
t = data(:,1);
z = data(:,2);
v = data(:,3);

% Figures
%figure('NumberTitle', 'Off', 'Name', [filename ': z(t)'])
%plot(t, z, '-')
%xlabel('t [s]')
%ylabel('z [m]')
%grid on

%figure('NumberTitle', 'Off', 'Name', [filename ': v(t)'])
%plot(t, v, '-')
%xlabel('t [s]')
%ylabel('v [m/s]')
%grid on

%figure('NumberTitle', 'Off', 'Name', [filename ': v(z)'])
%plot(z, v, '-')
%xlabel('z [m]')
%ylabel('v [m/s]')
%grid on



S1000 = '1000Steps.out';
S2000 = '2000Steps.out';
S4000 = '4000Steps.out';
S8000 = '8000Steps.out';
S16000 = '16000Steps.out';
S32000 = '32000Steps.out';

data1 = load(S1000);
data2 = load(S2000);
data4 = load(S4000);
data8 = load(S8000);
data16 = load(S16000);
data32 = load(S32000);
zfin = [data1(1001,2) data2(2001,2) data4(4001,2) data8(8001,2) data16(16001,2) data32(32001,2)];
vfin = [data1(1001,3) data2(2001,3) data4(4001,3) data8(8001,3) data16(16001,3) data32(32001,3)];
nsteps = [1000 2000 4000 8000 16000 32000];
 

myPlot(12,8.5,'ConvergenceVa',86400./(nsteps), vfin,0:10:90,-1500:500:1500,' $\Delta t [s]$ ','$v_{final} [m/s]$',[0,90],'auto','ConvergenceVa' ,0.75,0.5,11,8);

myPlot(12,8.5,'ConvergenceZa',86400./(nsteps), zfin,0:10:90,0.6e8:0.2e8:2e8,' $\Delta t [s]$ ','$z_{final} [m]$',[0,90],'auto','ConvergenceZa' ,0.75,0.5,11,8);


function myPlot(sizeX, sizeY, Title, X, Y, Xticks, Yticks, Xlabel, Ylabel, Xlim, Ylim, fileName, alw, lw, fsz, msz)

    figure('NumberTitle', 'Off')
    pos = get(gcf, 'Position');
    set(gcf, 'Position', [pos(1) pos(2) sizeX*100, sizeY*100]); %<- Set size
    set(gca, 'FontSize', fsz, 'LineWidth', alw); %<- Set properties
    
    
    plot(X, Y, '+','LineWidth',lw,'MarkerSize',msz) %Plot
    
    %Set limits
    xlim(Xlim);
    ylim(Ylim);
    
    xlabel(Xlabel,'Interpreter','latex')
    ylabel(Ylabel,'Interpreter','latex')
    grid on
    
    set(gca,'XTick',Xticks);
    set(gca,'YTick',Yticks);
    
    % Here we preserve the size of the image when we save it.
    set(gcf,'InvertHardcopy','on');
    set(gcf,'PaperUnits', 'centimeters');
    papersize = get(gcf, 'PaperSize');
    left = (papersize(1)- sizeX)/2;
    bottom = (papersize(2)- sizeY)/2;
    myfiguresize = [left, bottom, sizeX, sizeY];
    set(gcf,'PaperPosition', myfiguresize);
    
    print(fileName,'-depsc2','-r300');

end

