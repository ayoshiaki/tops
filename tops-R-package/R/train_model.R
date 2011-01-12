tops_train <-  function(file) {
  file = path.expand(file);
  return (.External("Rtops", command="train_model", filename = file ));
}

