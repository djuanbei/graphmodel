cmake_minimum_required(VERSION 2.8)

set(PROJECT_ROOT "/Users/yunyun/mycode/c++/graphmodel/ftma")

# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." OFF) # Makes boolean 'test' available.

# Make PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR, and PROJECT_NAME available.
set(PROJECT_NAME gmodel)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -fdiagnostics-absolute-paths -Wno-deprecated-register")
link_directories(/usr/local/lib/)

# force static runtime libraries for msvc builds
if(MSVC)
  set(variables 
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
	if(${variable} MATCHES "/MD")
	  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	endif()
  endforeach()
endif()

# If you want your own include/ directory, set this, and then you can do
# include_directories(${COMMON_INCLUDES}) in other CMakeLists.txt files.
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES}
    ~/mycode/c++/raptor/include
    /usr/local/include
)

file(GLOB_RECURSE CURRENT_HEADERS   ${PROJECT_ROOT}/include/*.h  ${PROJECT_ROOT}/include/*.hpp)
source_group("include" FILES ${CURRENT_HEADERS}) 

file(GLOB_RECURSE PARSER_SRC   ${PROJECT_ROOT}/src/*.l  ${PROJECT_ROOT}/src/*.y)

source_group("parser" FILES ${PARSER_SRC})

################################
# Normal Libraries & Executables
################################
file(GLOB_RECURSE LIB_SRCS   ${PROJECT_ROOT}/src/*.cpp)
list(REMOVE_ITEM LIB_SRCS "src/main.cpp")
add_library(gmodel_lib ${LIB_SRCS} ${CURRENT_HEADERS} ${PARSER_SRC})
add_executable(gmodel src/main.cpp)
# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere.

target_link_libraries(gmodel gmodel_lib glog)

################################
# Testing
################################
if (test)
  if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
  endif (APPLE)

  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(lib/gtest-1.6.0)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############
  add_executable(runUnitTests test_project1.cpp test_project2.cpp)

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)
  
  # Extra linking for the project.
  target_link_libraries(runUnitTests project1_lib)

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  add_test(NAME that-test-I-made COMMAND runUnitTests)


  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  add_test(that-other-test-I-made runUnitTests)
endif()

###########################
# parser
###########################
if (parser)
      execute_process(COMMAND 	lex  -o ${PROJECT_ROOT}/include/io/uppaalscan.h ${PROJECT_ROOT}/src/uppaalscan.l)
      execute_process(COMMAND   bison  -o  ${PROJECT_ROOT}/src/uppaalparser.cpp  -v ${PROJECT_ROOT}/src/uppaalparser.y)
endif()
