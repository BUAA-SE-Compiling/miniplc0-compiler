cmake_minimum_required(VERSION 3.12)

project(miniplc0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(3rd_party/argparse)
add_subdirectory(3rd_party/fmt)

set(PROJECT_EXE ${PROJECT_NAME})
set(PROJECT_LIB "${PROJECT_NAME}_lib")

set(lib_src
	tokenizer/token.h
	tokenizer/tokenizer.h
	tokenizer/tokenizer.cpp
	tokenizer/utils.hpp
	error/error.h
	analyser/analyser.h
	analyser/analyser.cpp
	instruction/instruction.h
)

set(main_src
	main.cpp
	fmts.hpp
)

add_library(${PROJECT_LIB} ${lib_src})

add_executable(${PROJECT_EXE} ${main_src})

set_target_properties(${PROJECT_EXE} PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
)

set_target_properties(${PROJECT_LIB} PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_EXE} PRIVATE .)
target_include_directories(${PROJECT_LIB} PRIVATE .)



if(MSVC)
	target_compile_options(${PROJECT_EXE} PRIVATE /W3)
	target_compile_options(${PROJECT_LIB} PRIVATE /W3)
else()
	target_compile_options(${PROJECT_EXE} PRIVATE -Wall -Wextra -pedantic)
	target_compile_options(${PROJECT_LIB} PRIVATE -Wall -Wextra -pedantic)
endif()

# This will add the include path, respectively.
# target_link_libraries(${PROJECT_LIB} fmt::fmt)
target_link_libraries(${PROJECT_EXE} ${PROJECT_LIB} argparse fmt::fmt)

# For tests
add_subdirectory(3rd_party/catch2)
enable_testing()

set(test_src
	tests/test_main.cpp
	tests/test_tokenizer.cpp
	tests/simple_vm.hpp
	tests/test_analyser.cpp
)

add_executable(miniplc0_test ${test_src})
target_include_directories(miniplc0_test PRIVATE .)
target_link_libraries(miniplc0_test Catch2::Test ${PROJECT_LIB} fmt::fmt)
add_test(all_test miniplc0_test)
find_program(OPEN_CPP_COVERAGE OpenCppCoverage.exe)

if (MSVC AND OPEN_CPP_COVERAGE)
	string(REPLACE "/" "\\" MY_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
	add_custom_command(TARGET miniplc0_test
					   POST_BUILD
					   COMMAND ${OPEN_CPP_COVERAGE} 
					            --sources ${MY_SOURCE_DIR} 
								--excluded_sources ${MY_SOURCE_DIR}\\3rd_party
								"$<TARGET_FILE:miniplc0_test>")
else()
	
endif()

set_target_properties(miniplc0_test PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRE ON)
