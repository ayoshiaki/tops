\name{tops_save}
\alias{tops_save}
\title{Write the model into a text file}
\description{
Saves a model into the text file
}
\usage{
tops_save(model,filename)
}
\arguments{
  \item{model}{ a model description returned by \code{tops_load}}
  \item{filename}{the name of the file}
}
\seealso{
  \code{\link{tops_load}}  \code{\link{tops_simulate}}
  \code{\link{tops_evaluate}}  \code{\link{tops_train}}
  \code{\link{tops_viterbi}}.
}
\examples{
hmm <- tops_load("hmm.txt") 
tops_save (hmm, "hmm2.txt")
}
\keyword{save}
