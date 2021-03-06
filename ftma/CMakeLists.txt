cmake_minimum_required(VERSION 2.8)
#set( CMAKE_VERBOSE_MAKEFILE on )
set(PROJECT_ROOT ".")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." OFF) # Makes boolean 'test' available.

# Make PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR, and PROJECT_NAME available.
set(PROJECT_NAME gmodel)
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "-Werror -g -std=c++11")
# -fdiagnostics-absolute-paths -Wno-deprecated-register")

#set(CMAKE_CXX_FLAGS "-Werror -g -std=c++11 -fdiagnostics-absolute-paths -Wno-deprecated-register")
#link_directories(/usr/local/lib/)

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
        /home/yun/mycode/c++/raptor/include/
    #    /usr/local/include
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
add_library(gmodel_lib ${LIB_SRCS} ${CURRENT_HEADERS} ${PARSER_SRC} include/sim/data/trace.h include/sim/constant.h src/sim/data/trace.cpp src/sim/data/tracemanager.cpp include/sim/system/ievent.h include/sim/system/ienvironment.h include/sim/system/ieventsource.h include/sim/system/system.h include/sim/system/inode.h include/sim/system/iedge.h include/sim/system/istate.h)
add_executable(gmodel src/main.cpp include/sim/data/trace.h include/sim/constant.h include/sim/data/tracemanager.h src/sim/data/trace.cpp src/sim/data/tracemanager.cpp include/sim/system/ievent.h include/sim/system/ievnetbus.h include/sim/system/ienvironment.h include/sim/system/ieventsource.h include/sim/system/system.h include/sim/system/inode.h include/sim/system/iedge.h include/sim/system/istate.h)
# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere.

target_link_libraries(gmodel gmodel_lib)

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
  file(GLOB_RECURSE TEST_SRCS   ${PROJECT_ROOT}/test/*.cpp)
  source_group("test" FILES ${TEST_SRCS})
  add_executable(runUnitTests ${TEST_SRCS} include/sim/data/trace.h include/sim/constant.h src/sim/data/trace.cpp src/sim/data/tracemanager.cpp include/sim/system/ievent.h include/sim/system/ienvironment.h include/sim/system/ieventsource.h include/sim/system/system.h include/sim/system/inode.h include/sim/system/iedge.h include/sim/system/istate.h)

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)
  
  # Extra linking for the project.
  target_link_libraries(runUnitTests gmodel_lib)

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
