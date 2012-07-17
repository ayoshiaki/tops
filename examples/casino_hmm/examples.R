library(ToPS)

# To use this script, you have to set the casino_hmm as the working directory.
# setwd("casino_hmm");

casino <- tops_load("casino.txt")
print(casino)

simulated <- tops_simulate(casino, 10)
print(simulated)

viterbi_result <- tops_viterbi(casino, simulated$sequence)
print(viterbi_result)

likelihood <- tops_evaluate (casino, simulated$sequence)
trained <- tops_train("casino_bw_train.txt")
tops_save("trained.txt", trained)

print(trained)

