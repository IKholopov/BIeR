# Build dag view

add_library(bier_dag
    dag_context.cpp
    dag_view.cpp)
target_include_directories(bier_dag PUBLIC ${BIER_INC})
target_cxx(bier_dag)
target_link_libraries(bier_dag PRIVATE Boost::boost)
set_target_properties(bier_dag
    PROPERTIES INTERFACE_LINK_LIBRARIES "")

add_subdirectory(dag_graph)
