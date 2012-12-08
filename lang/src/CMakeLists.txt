ADD_LIBRARY(tops-lang SHARED ASTNode PropertyNode KeyNode 
                             ValueNode StringNode ListNode
                             ConfigurationNode IntegerNode
                             FloatNode ConditionalProbabilityNode
                             ConditionalProbabilityMapNode 
                             ConditionNode ProbabilityMapNode
                             ProbabilityNode parser tokens
                             lang)
                             
TARGET_LINK_LIBRARIES(tops-lang tops)

install(TARGETS tops-lang
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static )

install(FILES ASTNode.hpp PropertyNode.hpp KeyNode.hpp 
              ValueNode.hpp StringNode.hpp ListNode.hpp
              ConfigurationNode.hpp IntegerNode.hpp
              FloatNode.hpp ConditionalProbabilityNode.hpp 
              ConditionalProbabilityMapNode.hpp 
              ConditionNode.hpp ProbabilityMapNode.hpp
              ProbabilityNode.hpp ToPSLang.hpp
              DESTINATION include/tops/lang)
