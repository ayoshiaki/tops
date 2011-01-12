\name{tops_load}
\alias{tops_load}
\title{Load a model described in a text file}
\description{
Loads a model described in a text file
}
\usage{
model <- tops_load(filename)
}
\arguments{
  \item{filename }{the name of the file}
}
\seealso{
 \code{\link{tops_simulate}}
  \code{\link{tops_evaluate}}  \code{\link{tops_train}}
  \code{\link{tops_viterbi}}   \code{\link{tops_save}}.
}
\examples{
hmm <- tops_load("hmm.txt") 
hmm
}
\keyword{likelihood}
