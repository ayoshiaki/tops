.initToPS <- function (where) {
  setClass("tops", representation(modelName="character", parameters="list"),where=where)
}
.First.lib <- function(libname, pkgname, where ) {
  if( !require(methods) ) stop("we require methods for package tops")
  where <- match(paste("package:", pkgname, sep=""), search())
  .initToPS(where)
  library.dynam("ToPS", pkgname, libname)
}
