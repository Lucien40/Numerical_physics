% Nom du fichier d'output a analyser
filename = 'output.out';


width = 12;
height = 10;
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
 
figure('NumberTitle', 'Off' , 'Name' , [filename ': ConZ'])

pos = get(gcf, 'Position');
set(gcf, 'Position', [pos(1) pos(2) width*100, height*100]); %<- Set size
set(gca, 'FontSize', fsz, 'LineWidth', alw); %<- Set properties
plot(86400./(nsteps), zfin, '+','LineWidth',lw,'MarkerSize',msz)
xlim([0,90]);
 
 xlabel('\Deltat [s]')
 ylabel('z_{final} [m]')
 grid on
 
set(gca,'XTick',0:10:90);
set(gca,'YTick',0.6e8:0.2e8:2e8);

% Here we preserve the size of the image when we save it.
set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'centimeters');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
print('improvedExample','-depsc2','-r300');
 
figure('NumberTitle', 'Off' , 'Name' , [filename ':ConV'])
 plot(86400./(nsteps), vfin, '+')
 xlabel('\Deltat [s]')
 ylabel('v_{final} [m/s]')
 grid on

%% Voici un exemple pour les etudes de convergences:
% nsteps = [1000 2000 4000 8000 16000 32000];
% zfin = [...];
% figure
% plot(nsteps, zfin, '+')
% xlabel('Nombre d''iterations')
% ylabel('z(t_{fin})')
% grid on
