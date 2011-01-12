extern "C" {
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <R_ext/Rdynload.h>
#include <Rembedded.h>
#include <Rinterface.h>
#include <R_ext/RS.h>
#include <R_ext/Error.h>
}


SEXP load_model(SEXP args);
SEXP save_model(SEXP args);

SEXP load_sequences(SEXP args) {
  printf("LOAD_SEQ\n");
  return R_NilValue;
}

SEXP save_sequences(SEXP args) {
  printf("SAVE_SEQ\n");
  return  R_NilValue;
}

SEXP evaluate(SEXP args);

SEXP viterbi(SEXP args);
SEXP simulate(SEXP args);


SEXP train_model(SEXP args);

extern "C" {
  
#include "tops.h"


  
  SEXP Rtops(SEXP args)
  {
    int i, nargs;
    const char *argname;
    const char *command;
    nargs = length(args) -1; 

    for(i = 0; i < nargs; i++) 
      {
	args = CDR(args);
	argname = CHAR(PRINTNAME(TAG(args)));
	if(strcmp(argname, "command") == 0) {
	  command =  CHAR(STRING_ELT(CAR(args), 0));
	  if(strcmp(command, "train_model") == 0)
	    {
	      return train_model(args);
	    }
	  else if(strcmp(command, "simulate") == 0)
	    {
	      return simulate(args);
	    }
	  else if (strcmp (command, "evaluate") == 0) 
	    {
	      return evaluate(args);
	    }
	  else if (strcmp (command, "viterbi") == 0) 
	    {
	      return viterbi(args);
	    }
	  else if (strcmp (command, "load_sequences") == 0) 
	    {
	      load_sequences(args);
	    }
	  else if (strcmp (command, "save_sequences") == 0) 
	    {
	      save_sequences(args);
	    }
	  else if (strcmp (command, "load_model") == 0) 
	    {
	      return load_model(args);
	    }
	  else if (strcmp (command, "save_model") == 0) 
	    {
	      return save_model(args);
	    }


	}
      }
     return (R_NilValue);
   }


}
