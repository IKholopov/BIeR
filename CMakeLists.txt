cmake_minimum_required(VERSION 3.8)
project("BIeR" LANGUAGES CXX VERSION 0.1.0)

find_program(CLANG_TIDY NAMES clang-tidy)
function(target_cxx target)
    target_compile_features(${target} PRIVATE cxx_std_17)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wno-sign-compare -Wno-overloaded-virtual)
    #[=[set_target_properties(${target}
        PROPERTIES
            CXX_CLANG_TIDY ${CLANG_TIDY})
     ]=]
endfunction(target_cxx)

find_package(Boost REQUIRED)

include(GNUInstallDirs)

set(BIER_INC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(BUILD_TESTS)
    enable_testing()
    set(TEST_PT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests/parser/sources)
endif()

add_subdirectory(bier)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_library(bier INTERFACE)
target_include_directories(
  bier
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bier>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(bier
    INTERFACE
        bier::bier_core
        bier::bier_builder
        bier::bier_ops
        bier::bier_serialization
        bier::bier_pass
        bier::bier_llvm
        bier::bier_dag
        bier::bier_dag_graph)
add_library(bier::bier ALIAS bier)


install(TARGETS bier bier_core bier_builder bier_ops bier_serialization bier_pass bier_llvm bier_dag bier_dag_graph
        EXPORT bier-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("BIERConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
configure_package_config_file(
        "BIERConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/BIERConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/bier/cmake"
)

install(EXPORT bier-targets
        FILE bier-targets.cmake
        NAMESPACE bier::
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/bier/cmake")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/BIERConfig.cmake"
              # "${CMAKE_CURRENT_BINARY_DIR}/BIERConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/bier/cmake")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/bier # source directory
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h")
