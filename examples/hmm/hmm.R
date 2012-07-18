# Here, we are using the HiddenMarkov package to validate our BaumWelch implementation. The estimated parameters  must be the same in ToPS and HiddenMarkov implementations.

library(HiddenMarkov)

source('sequence_from_hmm.R')

Pi <- matrix ( c(0.5,0.5,0.5,0.5) , ncol=2 )
delta <- c(0.5,0.5)

hmm <- dthmm(sequence, Pi, delta, "binom", pm = list(prob=c(0.3, 0.1)) , pn = list(size=rep(1, 10000)), discrete = 1)

hmm_trained <- BaumWelch(hmm)

hmm_trained$pm
