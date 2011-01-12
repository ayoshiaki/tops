\name{tops_evaluate}
\alias{tops_evaluate}
\title{Evaluates the likelihood of a sequence given the model}
\description{
Receives a model and a sequence and returns the likelihood of the sequence given the model
}
\usage{
tops_evaluate(model,sequence)
}
\arguments{
  \item{model}{ a model description returned by \code{tops_load}}
  \item{sequence}{the sequence of symbols}
}
\seealso{
  \code{\link{tops_simulate}}
  \code{\link{tops_evaluate}}  \code{\link{tops_train}}
  \code{\link{tops_viterbi}}   \code{\link{tops_save}}.

 
}
\examples{
hmm <- tops_load("hmm.txt") 
simulated <- tops_simulate(hmm, 1000)
tops_evaluate(hmm, simulated$sequence)
}
\keyword{likelihood}
