tops_evaluate <-  function(model, seq) {
  return (.External("Rtops", command="evaluate", model, seq));
}

