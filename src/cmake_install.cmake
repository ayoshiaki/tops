# Install script for directory: /Users/igorbonadio/Projetos/tops-old/src

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/static" TYPE STATIC_LIBRARY FILES "/Users/igorbonadio/Projetos/tops-old/src/libToPS.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/static/libToPS.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/static/libToPS.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/static/libToPS.a")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/tops" TYPE FILE FILES
    "/Users/igorbonadio/Projetos/tops-old/src/AkaikeInformationCriteria.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/Alphabet.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/BayesianInformationCriteria.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ConfigurationReader.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ContextTree.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/DecodableModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/DegenerateDistribution.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/FactorableModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/FactorableModelPrefixSumArray.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/DiscreteIIDModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/DiscreteIIDModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/FixedSequenceAtPosition.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/FixedSequenceAtPositionCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/GHMMStates.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/GeneralizedHiddenMarkovModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/GeneralizedHiddenMarkovModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/HiddenMarkovModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/HiddenMarkovModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/InhomogeneousFactorableModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/InhomogeneousMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/InhomogeneousMarkovChainCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/MultipleAlignment.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/NullPrefixSumArray.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/PhasedFactorableModelEvaluationAlgorithm.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/PhasedRunLengthDistribution.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/PhasedRunLengthDistributionCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/PrefixSumArray.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ProbabilisticModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ProbabilisticModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ProbabilisticModelCreatorClient.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ProbabilisticModelDecorator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ProbabilisticModelParameter.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/RemoveSequenceFromModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ReverseComplementDNA.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ReverseComplementDNACreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/Sequence.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SequenceEntry.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SequenceFactory.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SequenceFormat.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SmoothedHistogramBurge.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SmoothedHistogramKernelDensity.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SmoothedHistogramStanke.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SparseMatrix.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/Symbol.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TargetModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TargetModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainDiscreteIIDModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainFixedLengthMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainInterpolatedMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainHMMBaumWelch.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainHMMMaximumLikelihood.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainPhasedMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainInterpolatedPhasedMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainPhasedMarkovChainContextAlgorithm.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainVariableLengthInhomogeneousMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainVariableLengthMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainWeightArrayModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/VariableLengthMarkovChain.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/VariableLengthMarkovChainCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/util.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainGHMMTransitions.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/BernoulliModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SimilarityBasedSequenceWeighting.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/SimilarityBasedSequenceWeightingCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainSimilarityBasedSequenceWeighting.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/MultipleSequentialModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/MultipleSequentialModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/StoreLoadedModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/PairHiddenMarkovModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/PairHiddenMarkovModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainPHMMBaumWelch.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/MaximumDependenceDecomposition.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ProfileHiddenMarkovModel.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/ProfileHiddenMarkovModelCreator.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainProfileHMMMaxLikelihood.hpp"
    "/Users/igorbonadio/Projetos/tops-old/src/TrainProfileHMMBaumWelch.hpp"
    )
endif()

