# Build dag view

add_library(bier_dag
    dag_view.cpp)
target_include_directories(bier_dag PUBLIC ${BIER_INC})
target_cxx(bier_dag)
