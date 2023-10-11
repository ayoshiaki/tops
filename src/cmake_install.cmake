# Install script for directory: /home/waldir/Documents/topsAndre/tops/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/static" TYPE STATIC_LIBRARY FILES "/home/waldir/Documents/topsAndre/tops/src/libToPS.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tops" TYPE FILE FILES
    "/home/waldir/Documents/topsAndre/tops/src/AkaikeInformationCriteria.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/Alphabet.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/BayesianInformationCriteria.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ConfigurationReader.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ContextTree.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/DecodableModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/DegenerateDistribution.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/FactorableModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/FactorableModelPrefixSumArray.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/DiscreteIIDModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/DiscreteIIDModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/FixedSequenceAtPosition.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/FixedSequenceAtPositionCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/GHMMStates.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/GeneralizedHiddenMarkovModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/GeneralizedHiddenMarkovModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/HiddenMarkovModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/HiddenMarkovModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/InhomogeneousFactorableModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/InhomogeneousMarkovChain.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/InhomogeneousMarkovChainCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/MultipleAlignment.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/NullPrefixSumArray.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/PhasedFactorableModelEvaluationAlgorithm.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/PhasedRunLengthDistribution.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/PhasedRunLengthDistributionCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/PrefixSumArray.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ProbabilisticModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ProbabilisticModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ProbabilisticModelCreatorClient.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ProbabilisticModelDecorator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ProbabilisticModelParameter.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/RemoveSequenceFromModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ReverseComplementDNA.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ReverseComplementDNACreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/Sequence.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SequenceEntry.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SequenceFactory.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SequenceFormat.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SmoothedHistogramBurge.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SmoothedHistogramKernelDensity.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SmoothedHistogramStanke.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SparseMatrix.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/Symbol.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TargetModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TargetModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainDiscreteIIDModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainFixedLengthMarkovChain.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainInterpolatedMarkovChain.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainHMMBaumWelch.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainHMMMaximumLikelihood.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainPhasedMarkovChain.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainInterpolatedPhasedMarkovChain.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainPhasedMarkovChainContextAlgorithm.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainVariableLengthInhomogeneousMarkovChain.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainVariableLengthMarkovChain.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainWeightArrayModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/VariableLengthMarkovChain.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/VariableLengthMarkovChainCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/util.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainGHMMTransitions.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/BernoulliModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SimilarityBasedSequenceWeighting.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/SimilarityBasedSequenceWeightingCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainSimilarityBasedSequenceWeighting.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/MultipleSequentialModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/MultipleSequentialModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/StoreLoadedModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/PairHiddenMarkovModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/PairHiddenMarkovModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainPHMMBaumWelch.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/MaximumDependenceDecomposition.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ProfileHiddenMarkovModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/ProfileHiddenMarkovModelCreator.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainProfileHMMMaxLikelihood.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/TrainProfileHMMBaumWelch.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/NeuralNetworkModel.hpp"
    "/home/waldir/Documents/topsAndre/tops/src/NeuralNetworkModelCreator.hpp"
    )
endif()

