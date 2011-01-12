\name{tops_viterbi}
\alias{tops_viterbi}
\title{Find the most probable path using Viterbi algorithm}
\description{
Find the most probable path using viterbi algorithm
}
\usage{
seq  <- tops_viterbi(model, sequence)
}
\arguments{
  \item{model }{the probabilistic model}
  \item{sequence }{the sequence}
}
\seealso{
 \code{\link{tops_simulate}} \code{\link{tops_load}}
  \code{\link{tops_evaluate}}  \code{\link{tops_train}}
  \code{\link{tops_save}}.
}
\examples{
hmm <- tops_load("hmm.txt") 
simulated <- tops_simulate(hmm, 1000)
viterbi <- tops_viterbi(hmm, simulated$sequence)
}
\keyword{viterbi}
