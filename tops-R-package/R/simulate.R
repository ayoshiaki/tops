tops_simulate <-  function(pars, length) {
  return (.External("Rtops", command="simulate", pars, length));
}

