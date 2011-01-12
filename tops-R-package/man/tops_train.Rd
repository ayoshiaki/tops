\name{tops_train}
\alias{tops_train}
\title{Train a probabilistic model}
\description{
Train a probabilistic model
}
\usage{
model  <- tops_train(filename)
}
\arguments{
  \item{filename}{the configuration file}
}
\seealso{
 \code{\link{tops_simulate}} \code{\link{tops_load}}
  \code{\link{tops_evaluate}}  
  \code{\link{tops_save}}.
}
\examples{
hmm <- tops_train("hmm_train.txt") 
}
\keyword{viterbi}
