# Install script for directory: /Users/yoshiaki/work/programas/tops/src

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

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/yoshiaki/work/programas/tops/src/libToPS.dylib")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.dylib")
    EXECUTE_PROCESS(COMMAND "/usr/bin/install_name_tool"
      -id "libToPS.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.dylib")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.dylib")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tops" TYPE FILE FILES
    "/Users/yoshiaki/work/programas/tops/src/AkaikeInformationCriteria.hpp"
    "/Users/yoshiaki/work/programas/tops/src/Alphabet.hpp"
    "/Users/yoshiaki/work/programas/tops/src/BayesianInformationCriteria.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ConfigurationReader.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ContextTree.hpp"
    "/Users/yoshiaki/work/programas/tops/src/DecodableModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/DegenerateDistribution.hpp"
    "/Users/yoshiaki/work/programas/tops/src/FactorableModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/FactorableModelPrefixSumArray.hpp"
    "/Users/yoshiaki/work/programas/tops/src/DiscreteIIDModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/DiscreteIIDModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/FixedSequenceAtPosition.hpp"
    "/Users/yoshiaki/work/programas/tops/src/FixedSequenceAtPositionCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/GHMMStates.hpp"
    "/Users/yoshiaki/work/programas/tops/src/GeneralizedHiddenMarkovModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/GeneralizedHiddenMarkovModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/HiddenMarkovModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/HiddenMarkovModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/InhomogeneousFactorableModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/InhomogeneousMarkovChain.hpp"
    "/Users/yoshiaki/work/programas/tops/src/InhomogeneousMarkovChainCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/MultipleAlignment.hpp"
    "/Users/yoshiaki/work/programas/tops/src/NullPrefixSumArray.hpp"
    "/Users/yoshiaki/work/programas/tops/src/PhasedFactorableModelEvaluationAlgorithm.hpp"
    "/Users/yoshiaki/work/programas/tops/src/PhasedRunLengthDistribution.hpp"
    "/Users/yoshiaki/work/programas/tops/src/PhasedRunLengthDistributionCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/PrefixSumArray.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ProbabilisticModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ProbabilisticModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ProbabilisticModelCreatorClient.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ProbabilisticModelDecorator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ProbabilisticModelParameter.hpp"
    "/Users/yoshiaki/work/programas/tops/src/RemoveSequenceFromModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ReverseComplementDNA.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ReverseComplementDNACreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/Sequence.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SequenceEntry.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SequenceFactory.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SequenceFormat.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SmoothedHistogramBurge.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SmoothedHistogramKernelDensity.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SmoothedHistogramStanke.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SparseMatrix.hpp"
    "/Users/yoshiaki/work/programas/tops/src/Symbol.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TargetModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TargetModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainDiscreteIIDModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainFixedLengthMarkovChain.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainInterpolatedMarkovChain.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainHMMBaumWelch.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainHMMMaximumLikelihood.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainPhasedMarkovChain.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainInterpolatedPhasedMarkovChain.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainPhasedMarkovChainContextAlgorithm.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainVariableLengthInhomogeneousMarkovChain.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainVariableLengthMarkovChain.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainWeightArrayModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/VariableLengthMarkovChain.hpp"
    "/Users/yoshiaki/work/programas/tops/src/VariableLengthMarkovChainCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/util.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainGHMMTransitions.hpp"
    "/Users/yoshiaki/work/programas/tops/src/BernoulliModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SimilarityBasedSequenceWeighting.hpp"
    "/Users/yoshiaki/work/programas/tops/src/SimilarityBasedSequenceWeightingCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainSimilarityBasedSequenceWeighting.hpp"
    "/Users/yoshiaki/work/programas/tops/src/MultipleSequentialModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/MultipleSequentialModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/StoreLoadedModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/PairHiddenMarkovModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/PairHiddenMarkovModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainPHMMBaumWelch.hpp"
    "/Users/yoshiaki/work/programas/tops/src/MaximumDependenceDecomposition.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ProfileHiddenMarkovModel.hpp"
    "/Users/yoshiaki/work/programas/tops/src/ProfileHiddenMarkovModelCreator.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainProfileHMMMaxLikelihood.hpp"
    "/Users/yoshiaki/work/programas/tops/src/TrainProfileHMMBaumWelch.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

