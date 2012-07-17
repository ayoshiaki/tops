library(HiddenMarkov)

source('sequence_from_hmm.R')

Pi <- matrix ( c(0.5,0.5,0.5,0.5) , ncol=2 )
delta <- c(0.5,0.5)

hmm <- dthmm(sequence, Pi, delta, "binom", pm = list(prob=c(0.3, 0.1)) , pn = list(size=rep(1, 10000)), discrete = True)

hmm_trained <- BaumWelch(hmm)

