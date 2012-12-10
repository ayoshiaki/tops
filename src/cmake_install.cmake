# Install script for directory: /home/felipe/workspace/ToPs/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so"
         RPATH "")
  ENDIF()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/felipe/workspace/ToPs/src/libToPS.so")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tops" TYPE FILE FILES
    "/home/felipe/workspace/ToPs/src/AkaikeInformationCriteria.hpp"
    "/home/felipe/workspace/ToPs/src/Alphabet.hpp"
    "/home/felipe/workspace/ToPs/src/BayesianInformationCriteria.hpp"
    "/home/felipe/workspace/ToPs/src/ConfigurationReader.hpp"
    "/home/felipe/workspace/ToPs/src/ContextTree.hpp"
    "/home/felipe/workspace/ToPs/src/DecodableModel.hpp"
    "/home/felipe/workspace/ToPs/src/DegenerateDistribution.hpp"
    "/home/felipe/workspace/ToPs/src/FactorableModel.hpp"
    "/home/felipe/workspace/ToPs/src/FactorableModelPrefixSumArray.hpp"
    "/home/felipe/workspace/ToPs/src/DiscreteIIDModel.hpp"
    "/home/felipe/workspace/ToPs/src/DiscreteIIDModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/FixedSequenceAtPosition.hpp"
    "/home/felipe/workspace/ToPs/src/FixedSequenceAtPositionCreator.hpp"
    "/home/felipe/workspace/ToPs/src/GHMMStates.hpp"
    "/home/felipe/workspace/ToPs/src/GeneralizedHiddenMarkovModel.hpp"
    "/home/felipe/workspace/ToPs/src/GeneralizedHiddenMarkovModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/HiddenMarkovModel.hpp"
    "/home/felipe/workspace/ToPs/src/HiddenMarkovModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/InhomogeneousFactorableModel.hpp"
    "/home/felipe/workspace/ToPs/src/InhomogeneousMarkovChain.hpp"
    "/home/felipe/workspace/ToPs/src/InhomogeneousMarkovChainCreator.hpp"
    "/home/felipe/workspace/ToPs/src/MultipleAlignment.hpp"
    "/home/felipe/workspace/ToPs/src/NullPrefixSumArray.hpp"
    "/home/felipe/workspace/ToPs/src/PhasedFactorableModelEvaluationAlgorithm.hpp"
    "/home/felipe/workspace/ToPs/src/PhasedRunLengthDistribution.hpp"
    "/home/felipe/workspace/ToPs/src/PhasedRunLengthDistributionCreator.hpp"
    "/home/felipe/workspace/ToPs/src/PrefixSumArray.hpp"
    "/home/felipe/workspace/ToPs/src/ProbabilisticModel.hpp"
    "/home/felipe/workspace/ToPs/src/ProbabilisticModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/ProbabilisticModelCreatorClient.hpp"
    "/home/felipe/workspace/ToPs/src/ProbabilisticModelDecorator.hpp"
    "/home/felipe/workspace/ToPs/src/ProbabilisticModelParameter.hpp"
    "/home/felipe/workspace/ToPs/src/RemoveSequenceFromModel.hpp"
    "/home/felipe/workspace/ToPs/src/ReverseComplementDNA.hpp"
    "/home/felipe/workspace/ToPs/src/ReverseComplementDNACreator.hpp"
    "/home/felipe/workspace/ToPs/src/Sequence.hpp"
    "/home/felipe/workspace/ToPs/src/SequenceEntry.hpp"
    "/home/felipe/workspace/ToPs/src/SequenceFactory.hpp"
    "/home/felipe/workspace/ToPs/src/SequenceFormat.hpp"
    "/home/felipe/workspace/ToPs/src/SmoothedHistogramBurge.hpp"
    "/home/felipe/workspace/ToPs/src/SmoothedHistogramKernelDensity.hpp"
    "/home/felipe/workspace/ToPs/src/SmoothedHistogramStanke.hpp"
    "/home/felipe/workspace/ToPs/src/SparseMatrix.hpp"
    "/home/felipe/workspace/ToPs/src/Symbol.hpp"
    "/home/felipe/workspace/ToPs/src/TargetModel.hpp"
    "/home/felipe/workspace/ToPs/src/TargetModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/TrainDiscreteIIDModel.hpp"
    "/home/felipe/workspace/ToPs/src/TrainFixedLengthMarkovChain.hpp"
    "/home/felipe/workspace/ToPs/src/TrainInterpolatedMarkovChain.hpp"
    "/home/felipe/workspace/ToPs/src/TrainHMMBaumWelch.hpp"
    "/home/felipe/workspace/ToPs/src/TrainPhasedMarkovChain.hpp"
    "/home/felipe/workspace/ToPs/src/TrainInterpolatedPhasedMarkovChain.hpp"
    "/home/felipe/workspace/ToPs/src/TrainPhasedMarkovChainContextAlgorithm.hpp"
    "/home/felipe/workspace/ToPs/src/TrainVariableLengthInhomogeneousMarkovChain.hpp"
    "/home/felipe/workspace/ToPs/src/TrainVariableLengthMarkovChain.hpp"
    "/home/felipe/workspace/ToPs/src/TrainWeightArrayModel.hpp"
    "/home/felipe/workspace/ToPs/src/VariableLengthMarkovChain.hpp"
    "/home/felipe/workspace/ToPs/src/VariableLengthMarkovChainCreator.hpp"
    "/home/felipe/workspace/ToPs/src/util.hpp"
    "/home/felipe/workspace/ToPs/src/TrainGHMMTransitions.hpp"
    "/home/felipe/workspace/ToPs/src/BernoulliModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/SimilarityBasedSequenceWeighting.hpp"
    "/home/felipe/workspace/ToPs/src/SimilarityBasedSequenceWeightingCreator.hpp"
    "/home/felipe/workspace/ToPs/src/TrainSimilarityBasedSequenceWeighting.hpp"
    "/home/felipe/workspace/ToPs/src/MultipleSequentialModel.hpp"
    "/home/felipe/workspace/ToPs/src/MultipleSequentialModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/StoreLoadedModel.hpp"
    "/home/felipe/workspace/ToPs/src/PairHiddenMarkovModel.hpp"
    "/home/felipe/workspace/ToPs/src/PairHiddenMarkovModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/TrainPHMMBaumWelch.hpp"
    "/home/felipe/workspace/ToPs/src/MaximumDependenceDecomposition.hpp"
    "/home/felipe/workspace/ToPs/src/ProfileHiddenMarkovModel.hpp"
    "/home/felipe/workspace/ToPs/src/ProfileHiddenMarkovModelCreator.hpp"
    "/home/felipe/workspace/ToPs/src/TrainProfileHMMMaxLikelihood.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

