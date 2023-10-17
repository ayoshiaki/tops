# Install script for directory: /home/edisone/Documents/Doutorado/deeptops/src

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
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/static" TYPE STATIC_LIBRARY FILES "/home/edisone/Documents/Doutorado/deeptops/src/libToPS.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tops" TYPE FILE FILES
    "/home/edisone/Documents/Doutorado/deeptops/src/AkaikeInformationCriteria.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/Alphabet.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/BayesianInformationCriteria.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ConfigurationReader.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ContextTree.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/DecodableModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/DegenerateDistribution.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/FactorableModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/FactorableModelPrefixSumArray.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/DiscreteIIDModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/DiscreteIIDModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/FixedSequenceAtPosition.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/FixedSequenceAtPositionCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/GHMMStates.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/GeneralizedHiddenMarkovModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/GeneralizedHiddenMarkovModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/HiddenMarkovModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/HiddenMarkovModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/InhomogeneousFactorableModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/InhomogeneousMarkovChain.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/InhomogeneousMarkovChainCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/MultipleAlignment.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/NullPrefixSumArray.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/PhasedFactorableModelEvaluationAlgorithm.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/PhasedRunLengthDistribution.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/PhasedRunLengthDistributionCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/PrefixSumArray.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ProbabilisticModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ProbabilisticModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ProbabilisticModelCreatorClient.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ProbabilisticModelDecorator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ProbabilisticModelParameter.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/RemoveSequenceFromModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ReverseComplementDNA.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ReverseComplementDNACreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/Sequence.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SequenceEntry.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SequenceFactory.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SequenceFormat.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SmoothedHistogramBurge.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SmoothedHistogramKernelDensity.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SmoothedHistogramStanke.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SparseMatrix.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/Symbol.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TargetModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TargetModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainDiscreteIIDModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainFixedLengthMarkovChain.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainInterpolatedMarkovChain.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainHMMBaumWelch.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainHMMMaximumLikelihood.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainPhasedMarkovChain.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainInterpolatedPhasedMarkovChain.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainPhasedMarkovChainContextAlgorithm.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainVariableLengthInhomogeneousMarkovChain.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainVariableLengthMarkovChain.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainWeightArrayModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/VariableLengthMarkovChain.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/VariableLengthMarkovChainCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/util.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainGHMMTransitions.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/BernoulliModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SimilarityBasedSequenceWeighting.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/SimilarityBasedSequenceWeightingCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainSimilarityBasedSequenceWeighting.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/MultipleSequentialModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/MultipleSequentialModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/StoreLoadedModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/PairHiddenMarkovModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/PairHiddenMarkovModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainPHMMBaumWelch.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/MaximumDependenceDecomposition.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ProfileHiddenMarkovModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/ProfileHiddenMarkovModelCreator.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainProfileHMMMaxLikelihood.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/TrainProfileHMMBaumWelch.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/NeuralNetworkModel.hpp"
    "/home/edisone/Documents/Doutorado/deeptops/src/NeuralNetworkModelCreator.hpp"
    )
endif()

