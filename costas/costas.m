% COSTAS LOOP demo

%--------------------------------------------
% BPSK modulator

samplespersymbol = 16;      % simulation done at this number of samples per symbol
numsymbols       = 200;      % simulate this number of symbol
carrierfreq      = 4;       % times the symbol frequency; <= samplespersymbol

carriersync      = [ 1  1  1  1  1  1  1  1  1  1  1  1  1  1  1 ];
symbolsync       = [ 1 -1  1 -1  1 -1  1 -1  1 -1  1 -1  1 -1  1 ];

sync1    = [carriersync symbolsync];
payload  = (floor((rand(numsymbols-length(sync1),1) * 2)) - 0.5) * 2;
symbols  = [sync1'  ; payload]; %'

offset   = pi / 16;
carrier          = cos(offset + 2 * pi * carrierfreq*((1:samplespersymbol)-1)/samplespersymbol);

fullcarrier      = repmat(carrier', numsymbols, 1);     % '
rcpulse          = rcosdesign(0.25, 3, samplespersymbol, 'sqrt');
rcupsym          = conv(upsample(symbols, samplespersymbol), rcpulse);
modrcsym         = fullcarrier .* rcupsym(1:(samplespersymbol*numsymbols));

figure(1)
pwelch(modrcsym);

figure(2)
subplot(3,1,1);
hold off
plot(rcupsym);
grid
title('rc symbols');
subplot(3,1,2);
plot(fullcarrier);
title('carrier');
grid
subplot(3,1,3);
plot(modrcsym);
title('rc modulated symbols');
grid

%--------------------------------------------
% BPSK demodulator
% adapted from: https://dsp.stackexchange.com/questions/22316/simulating-bpsk-costas-loop-in-matlab

rfsignal = modrcsym; 
phi      = zeros(length(rfsignal),1);
xi       = zeros(length(rfsignal),1);
xq       = zeros(length(rfsignal),1);

% costas loop
for i = 2:length(rfsignal)
  xi(i)    = rfsignal(i) .* cos(- 2 * pi * i * carrierfreq / samplespersymbol - phi(i-1));
  xq(i)    = rfsignal(i) .* sin(- 2 * pi * i * carrierfreq / samplespersymbol - phi(i-1));
  zi       = conv(rcpulse, xi(1:i));  
  zq       = conv(rcpulse, xq(1:i));  
  error(i) = zi(i) * zq(i);
  phi(i)   = phi(i-1) + 0.1 * error(i);
end

figure(3)
subplot(4,1,1)
plot(zi)
title('I branch LPF')

subplot(4,1,2)
plot(zq)
title('Q branch LPF')

subplot(4,1,3)
plot(error)
title('Error')

subplot(4,1,4)
plot(phi)
title('Phase')
