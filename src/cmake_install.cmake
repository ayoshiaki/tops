# Install script for directory: /Users/igorbonadio/Projetos/tops/src

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/igorbonadio/Projetos/tops/src/libToPS.dylib")
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
    "/Users/igorbonadio/Projetos/tops/src/AkaikeInformationCriteria.hpp"
    "/Users/igorbonadio/Projetos/tops/src/Alphabet.hpp"
    "/Users/igorbonadio/Projetos/tops/src/BayesianInformationCriteria.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ConfigurationReader.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ContextTree.hpp"
    "/Users/igorbonadio/Projetos/tops/src/DecodableModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/DegenerateDistribution.hpp"
    "/Users/igorbonadio/Projetos/tops/src/FactorableModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/FactorableModelPrefixSumArray.hpp"
    "/Users/igorbonadio/Projetos/tops/src/DiscreteIIDModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/DiscreteIIDModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/FixedSequenceAtPosition.hpp"
    "/Users/igorbonadio/Projetos/tops/src/FixedSequenceAtPositionCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/GHMMStates.hpp"
    "/Users/igorbonadio/Projetos/tops/src/GeneralizedHiddenMarkovModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/GeneralizedHiddenMarkovModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/HiddenMarkovModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/HiddenMarkovModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/InhomogeneousFactorableModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/InhomogeneousMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops/src/InhomogeneousMarkovChainCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/MultipleAlignment.hpp"
    "/Users/igorbonadio/Projetos/tops/src/NullPrefixSumArray.hpp"
    "/Users/igorbonadio/Projetos/tops/src/PhasedFactorableModelEvaluationAlgorithm.hpp"
    "/Users/igorbonadio/Projetos/tops/src/PhasedRunLengthDistribution.hpp"
    "/Users/igorbonadio/Projetos/tops/src/PhasedRunLengthDistributionCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/PrefixSumArray.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ProbabilisticModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ProbabilisticModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ProbabilisticModelCreatorClient.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ProbabilisticModelDecorator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ProbabilisticModelParameter.hpp"
    "/Users/igorbonadio/Projetos/tops/src/RemoveSequenceFromModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ReverseComplementDNA.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ReverseComplementDNACreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/Sequence.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SequenceEntry.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SequenceFactory.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SequenceFormat.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SmoothedHistogramBurge.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SmoothedHistogramKernelDensity.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SmoothedHistogramStanke.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SparseMatrix.hpp"
    "/Users/igorbonadio/Projetos/tops/src/Symbol.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TargetModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TargetModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainDiscreteIIDModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainFixedLengthMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainInterpolatedMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainHMMBaumWelch.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainHMMMaximumLikelihood.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainPhasedMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainInterpolatedPhasedMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainPhasedMarkovChainContextAlgorithm.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainVariableLengthInhomogeneousMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainVariableLengthMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainWeightArrayModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/VariableLengthMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops/src/VariableLengthMarkovChainCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/util.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainGHMMTransitions.hpp"
    "/Users/igorbonadio/Projetos/tops/src/BernoulliModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SimilarityBasedSequenceWeighting.hpp"
    "/Users/igorbonadio/Projetos/tops/src/SimilarityBasedSequenceWeightingCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainSimilarityBasedSequenceWeighting.hpp"
    "/Users/igorbonadio/Projetos/tops/src/MultipleSequentialModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/MultipleSequentialModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/StoreLoadedModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/PairHiddenMarkovModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/PairHiddenMarkovModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainPHMMBaumWelch.hpp"
    "/Users/igorbonadio/Projetos/tops/src/MaximumDependenceDecomposition.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ProfileHiddenMarkovModel.hpp"
    "/Users/igorbonadio/Projetos/tops/src/ProfileHiddenMarkovModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainProfileHMMMaxLikelihood.hpp"
    "/Users/igorbonadio/Projetos/tops/src/TrainProfileHMMBaumWelch.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

