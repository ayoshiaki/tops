tops_save <-  function(file, pars) {
  file = path.expand(file);
  return (.External("Rtops", command="save_model", file, pars));
}

