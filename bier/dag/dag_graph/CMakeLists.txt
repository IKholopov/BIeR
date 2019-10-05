add_library(bier_dag_graph
    graph.cpp
    graph_builder.cpp
    dot_serializer.cpp)
target_include_directories(bier_dag_graph PUBLIC ${BIER_INC})
target_cxx(bier_dag_graph)
target_link_libraries(bier_dag_graph bier_serialization)
