# Install script for directory: /home/waldir/Documents/deeptops/src

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/static" TYPE STATIC_LIBRARY FILES "/home/waldir/Documents/deeptops/src/libToPS.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tops" TYPE FILE FILES
    "/home/waldir/Documents/deeptops/src/AkaikeInformationCriteria.hpp"
    "/home/waldir/Documents/deeptops/src/Alphabet.hpp"
    "/home/waldir/Documents/deeptops/src/BayesianInformationCriteria.hpp"
    "/home/waldir/Documents/deeptops/src/ConfigurationReader.hpp"
    "/home/waldir/Documents/deeptops/src/ContextTree.hpp"
    "/home/waldir/Documents/deeptops/src/DecodableModel.hpp"
    "/home/waldir/Documents/deeptops/src/DegenerateDistribution.hpp"
    "/home/waldir/Documents/deeptops/src/FactorableModel.hpp"
    "/home/waldir/Documents/deeptops/src/FactorableModelPrefixSumArray.hpp"
    "/home/waldir/Documents/deeptops/src/DiscreteIIDModel.hpp"
    "/home/waldir/Documents/deeptops/src/DiscreteIIDModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/FixedSequenceAtPosition.hpp"
    "/home/waldir/Documents/deeptops/src/FixedSequenceAtPositionCreator.hpp"
    "/home/waldir/Documents/deeptops/src/GHMMStates.hpp"
    "/home/waldir/Documents/deeptops/src/GeneralizedHiddenMarkovModel.hpp"
    "/home/waldir/Documents/deeptops/src/GeneralizedHiddenMarkovModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/HiddenMarkovModel.hpp"
    "/home/waldir/Documents/deeptops/src/HiddenMarkovModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/InhomogeneousFactorableModel.hpp"
    "/home/waldir/Documents/deeptops/src/InhomogeneousMarkovChain.hpp"
    "/home/waldir/Documents/deeptops/src/InhomogeneousMarkovChainCreator.hpp"
    "/home/waldir/Documents/deeptops/src/MultipleAlignment.hpp"
    "/home/waldir/Documents/deeptops/src/NullPrefixSumArray.hpp"
    "/home/waldir/Documents/deeptops/src/PhasedFactorableModelEvaluationAlgorithm.hpp"
    "/home/waldir/Documents/deeptops/src/PhasedRunLengthDistribution.hpp"
    "/home/waldir/Documents/deeptops/src/PhasedRunLengthDistributionCreator.hpp"
    "/home/waldir/Documents/deeptops/src/PrefixSumArray.hpp"
    "/home/waldir/Documents/deeptops/src/ProbabilisticModel.hpp"
    "/home/waldir/Documents/deeptops/src/ProbabilisticModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/ProbabilisticModelCreatorClient.hpp"
    "/home/waldir/Documents/deeptops/src/ProbabilisticModelDecorator.hpp"
    "/home/waldir/Documents/deeptops/src/ProbabilisticModelParameter.hpp"
    "/home/waldir/Documents/deeptops/src/RemoveSequenceFromModel.hpp"
    "/home/waldir/Documents/deeptops/src/ReverseComplementDNA.hpp"
    "/home/waldir/Documents/deeptops/src/ReverseComplementDNACreator.hpp"
    "/home/waldir/Documents/deeptops/src/Sequence.hpp"
    "/home/waldir/Documents/deeptops/src/SequenceEntry.hpp"
    "/home/waldir/Documents/deeptops/src/SequenceFactory.hpp"
    "/home/waldir/Documents/deeptops/src/SequenceFormat.hpp"
    "/home/waldir/Documents/deeptops/src/SmoothedHistogramBurge.hpp"
    "/home/waldir/Documents/deeptops/src/SmoothedHistogramKernelDensity.hpp"
    "/home/waldir/Documents/deeptops/src/SmoothedHistogramStanke.hpp"
    "/home/waldir/Documents/deeptops/src/SparseMatrix.hpp"
    "/home/waldir/Documents/deeptops/src/Symbol.hpp"
    "/home/waldir/Documents/deeptops/src/TargetModel.hpp"
    "/home/waldir/Documents/deeptops/src/TargetModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/TrainDiscreteIIDModel.hpp"
    "/home/waldir/Documents/deeptops/src/TrainFixedLengthMarkovChain.hpp"
    "/home/waldir/Documents/deeptops/src/TrainInterpolatedMarkovChain.hpp"
    "/home/waldir/Documents/deeptops/src/TrainHMMBaumWelch.hpp"
    "/home/waldir/Documents/deeptops/src/TrainHMMMaximumLikelihood.hpp"
    "/home/waldir/Documents/deeptops/src/TrainPhasedMarkovChain.hpp"
    "/home/waldir/Documents/deeptops/src/TrainInterpolatedPhasedMarkovChain.hpp"
    "/home/waldir/Documents/deeptops/src/TrainPhasedMarkovChainContextAlgorithm.hpp"
    "/home/waldir/Documents/deeptops/src/TrainVariableLengthInhomogeneousMarkovChain.hpp"
    "/home/waldir/Documents/deeptops/src/TrainVariableLengthMarkovChain.hpp"
    "/home/waldir/Documents/deeptops/src/TrainWeightArrayModel.hpp"
    "/home/waldir/Documents/deeptops/src/VariableLengthMarkovChain.hpp"
    "/home/waldir/Documents/deeptops/src/VariableLengthMarkovChainCreator.hpp"
    "/home/waldir/Documents/deeptops/src/util.hpp"
    "/home/waldir/Documents/deeptops/src/TrainGHMMTransitions.hpp"
    "/home/waldir/Documents/deeptops/src/BernoulliModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/SimilarityBasedSequenceWeighting.hpp"
    "/home/waldir/Documents/deeptops/src/SimilarityBasedSequenceWeightingCreator.hpp"
    "/home/waldir/Documents/deeptops/src/TrainSimilarityBasedSequenceWeighting.hpp"
    "/home/waldir/Documents/deeptops/src/MultipleSequentialModel.hpp"
    "/home/waldir/Documents/deeptops/src/MultipleSequentialModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/StoreLoadedModel.hpp"
    "/home/waldir/Documents/deeptops/src/PairHiddenMarkovModel.hpp"
    "/home/waldir/Documents/deeptops/src/PairHiddenMarkovModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/TrainPHMMBaumWelch.hpp"
    "/home/waldir/Documents/deeptops/src/MaximumDependenceDecomposition.hpp"
    "/home/waldir/Documents/deeptops/src/ProfileHiddenMarkovModel.hpp"
    "/home/waldir/Documents/deeptops/src/ProfileHiddenMarkovModelCreator.hpp"
    "/home/waldir/Documents/deeptops/src/TrainProfileHMMMaxLikelihood.hpp"
    "/home/waldir/Documents/deeptops/src/TrainProfileHMMBaumWelch.hpp"
    "/home/waldir/Documents/deeptops/src/NeuralNetworkModel.hpp"
    "/home/waldir/Documents/deeptops/src/NeuralNetworkModelCreator.hpp"
    )
endif()

