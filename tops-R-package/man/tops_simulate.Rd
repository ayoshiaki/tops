\name{tops_simulate}
\alias{tops_simulate}
\title{Simulate a model}
\description{
Simulates a probabilistic model
}
\usage{
simulated <- tops_simulate(model, length)
}
\arguments{
  \item{model}{the probabilistic model to be simulated}
  \item{length}{the length of the sequence}
}
\seealso{
  \code{\link{tops_simulate}}
  \code{\link{tops_evaluate}}  \code{\link{tops_train}}
  \code{\link{tops_viterbi}}   \code{\link{tops_save}}.
}
\examples{
hmm <- tops_load("hmm.txt") 
# generates a sequence of length 1000 from the hmm model
simulated <- tops_simulate(hmm, 1000)
}
\keyword{simulate}
