# Install script for directory: /home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/libToPS.so")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libToPS.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tops" TYPE FILE FILES
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/AkaikeInformationCriteria.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/Alphabet.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/BayesianInformationCriteria.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ConfigurationReader.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ContextTree.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/DecodableModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/DegenerateDistribution.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/FactorableModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/FactorableModelPrefixSumArray.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/DiscreteIIDModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/DiscreteIIDModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/FixedSequenceAtPosition.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/FixedSequenceAtPositionCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/GHMMStates.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/GeneralizedHiddenMarkovModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/GeneralizedHiddenMarkovModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/HiddenMarkovModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/HiddenMarkovModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/InhomogeneousFactorableModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/InhomogeneousMarkovChain.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/InhomogeneousMarkovChainCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/MultipleAlignment.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/NullPrefixSumArray.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/PhasedFactorableModelEvaluationAlgorithm.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/PhasedRunLengthDistribution.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/PhasedRunLengthDistributionCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/PrefixSumArray.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ProbabilisticModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ProbabilisticModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ProbabilisticModelCreatorClient.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ProbabilisticModelDecorator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ProbabilisticModelParameter.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/RemoveSequenceFromModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ReverseComplementDNA.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ReverseComplementDNACreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/Sequence.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SequenceEntry.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SequenceFactory.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SequenceFormat.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SmoothedHistogramBurge.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SmoothedHistogramKernelDensity.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SmoothedHistogramStanke.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SparseMatrix.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/Symbol.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TargetModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TargetModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainDiscreteIIDModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainFixedLengthMarkovChain.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainInterpolatedMarkovChain.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainHMMBaumWelch.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainPhasedMarkovChain.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainInterpolatedPhasedMarkovChain.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainPhasedMarkovChainContextAlgorithm.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainVariableLengthInhomogeneousMarkovChain.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainVariableLengthMarkovChain.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainWeightArrayModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/VariableLengthMarkovChain.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/VariableLengthMarkovChainCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/util.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainGHMMTransitions.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/BernoulliModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SimilarityBasedSequenceWeighting.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/SimilarityBasedSequenceWeightingCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainSimilarityBasedSequenceWeighting.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/MultipleSequentialModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/MultipleSequentialModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/StoreLoadedModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/PairHiddenMarkovModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/PairHiddenMarkovModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainPHMMBaumWelch.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/MaximumDependenceDecomposition.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ProfileHiddenMarkovModel.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/ProfileHiddenMarkovModelCreator.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainProfileHMMMaxLikelihood.hpp"
    "/home/rafael/Mestrado/Bioinformatica/Trabalho/antigo_tops/tops/src/TrainProfileHMMBaumWelch.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

