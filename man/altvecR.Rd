\name{altvecR}
\alias{altvecR}
\title{
  altvecR constructor
}
\description{
  \code{altvecR} creates an ALTVEC object that calls R functions on the
  ALTVEC methods.
}
\usage{
altvecR(x)
}
\arguments{
  \item{x}{object that will be supplied to the functions/methods}
}
\details{
}
\value{
}
\author{
  Simon Urbanek
}
\examples{
get.len <- function(x) { cat("get.len: "); str(x); length(x)+0 }
get.all <- function(x) { cat("get.all: "); str(x); x }
get.all.or.NULL <- function(x) { cat("get.all|NULL: "); str(x); x }
get.elt <- function(x,i) { cat("get.elt(", i, "): "); str(x); x[i+1] }

x <- altvecR(1:5 + 0)
length(x)
x[1]
x[2:3]
max(x)
x
}
\keyword{manip}
