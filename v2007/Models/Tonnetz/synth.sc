(// init
  var initServer = 
  {
    //s.sendMsg("/notify",1);
    s.sendMsg("/dumpOSC",1);
    s.sendMsg("/g_new",1,1,0);
  };
  initServer.value;
)


(// synth
  SynthDef("Tonnetz",
  { |freq=440,mean=440,amp=0.1|
  
    // amplitude, max (1.0) for freq=mean, 0 for freq/mean>zeroRatio || <1/zeroRatio
    var ampSpectrum =
    { |freq,mean,meanAmp,zeroRat|
      var fa = 1-(abs(log(freq/mean))/log(zeroRat));
      meanAmp*max(fa,0)
    };
    
    // fundamental freq,partials,zero amplitude -> [[freq,ampl],...]
    var spectrum =
    { |fund,mean,meanAmp,partRats,zeroRat|
      var fq = (fund*partRats);
      var am = ampSpectrum.value(fq,mean,meanAmp,zeroRat);
      Array.fill(fq.size, {arg i; [fq[i],am[i]]});
    };

    // partials, where zero
    var parts = [1/8,1/4,1/2,1,2,4,8], zeroRat = 8;

    // go
    var sp  = spectrum.value(freq,mean,amp,parts,zeroRat);
    var osc = Array.fill(sp.size,
                { |i|
                  var fq = sp[i][0], am = sp[i][1];
                  SinOsc.ar([fq,fq],0,am)
                });
    osc.do{arg o; Out.ar(0,o)}
  }).load(s);
)

s.sendMsg("/s_new", "Tonnetz", x = s.nextNodeID, 1, 1, "freq", 440, "mean", 440, "amp", 0.3)
s.sendMsg("/s_new", "Tonnetz", x = s.nextNodeID, 1, 1, "freq", 550, "mean", 440, "amp", 0.3)
s.sendMsg("/s_new", "Tonnetz", x = s.nextNodeID, 1, 1, "freq", 660, "mean", 440, "amp", 0.3)
s.sendMsg("/s_new", "Tonnetz", x = s.nextNodeID, 1, 1, "freq", 770, "mean", 440, "amp", 0.3)
s.sendMsg("/s_new", "Tonnetz", x = s.nextNodeID, 1, 1, "freq", 880, "mean", 440, "amp", 0.3)

s.sendMsg("/s_new", "Tonnetz", x = s.nextNodeID, 1, 1, "freq", 220, "mean", 440, "amp", 0.3)
s.sendMsg("/s_new", "Tonnetz", x = s.nextNodeID, 1, 1, "freq", 440, "mean", 440, "amp", 0.3)
s.sendMsg("/s_new", "Tonnetz", x = s.nextNodeID, 1, 1, "freq", 880, "mean", 440, "amp", 0.3)
