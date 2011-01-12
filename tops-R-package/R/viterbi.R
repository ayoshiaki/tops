tops_viterbi <-  function(model, seq) {
  return (.External("Rtops", command="viterbi", model, seq));
}

