tops_load <-  function(file) {
  file = path.expand(file)
  return (.External("Rtops", command="load_model", filename = file ));
}

