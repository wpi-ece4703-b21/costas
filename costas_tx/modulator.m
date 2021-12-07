%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% LAB 5 ECE 4703
% BPSK Modulator
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Main simulation parameters
samplespersymbol = 16;      % simulation done at this number of samples per symbol
numsymbols       = 256;     % simulate this number of symbol
carrierfreq      = 4;       % times the symbol frequency; must be <= samplespersymbol

% Generate the symbol stream.
% The symbol stream contains random symbols of value +1 and -1.
% However, at the start the the symbol stream sequence, there is a
% 'carrier sync' sequence containing alternating +1 and -1 symbols.
% This carrier synchronization will be used by the BPSK demodulator.
carriersync      = [ 1 -1  1 -1  1 -1  1 -1];
payload  = (floor((rand(numsymbols-length(carriersync),1) * 2)) - 0.5) * 2;
symbols  = [carriersync' ; payload]; % '

% Generate the carrier. 
% The carrier is a multiple of the symbol frequency. We generate a carrier 
% wave for the duration of a single symbol and next repeat that wave over 
% the number of symbols to yield the complete carrier.
carrier          = cos(2 * pi * carrierfreq*((1:samplespersymbol)-1)/samplespersymbol);
fullcarrier      = repmat(carrier', numsymbols, 1);     % '

% Create the upsampled signal.
upsampled        = upsample(symbols, samplespersymbol);

% Create the root-raised cosine pulse shapine filter.
% We use a filter with a length of 3 symbols and a rolloff of 0.25
rcpulse          = rcosdesign(0.25, 3, samplespersymbol, 'sqrt');

% Filter the upsampled symbol stream.
rcupsym          = conv(upsampled, rcpulse);

% Upconvert the shaped signal to the carrier frequency.
modrcsym         = fullcarrier .* rcupsym(1:(samplespersymbol*numsymbols));


