# CMake 프로그램의 최소 버전
cmake_minimum_required(VERSION 3.11)

# 프로젝트 정보
project(
  Awesome_mix_vol_1
  VERSION 0.1
  DESCRIPTION "Awesome_mix_vol_1"
  LANGUAGES CXX)

# 라이브러리 파일은 빌드 디렉토리 안에 lib 폴더에 출력.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 빌드 형상(Configuration) 및 주절주절 Makefile 생성 여부
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_VERBOSE_MAKEFILE true)
# set(CMAKE_VERBOSE_MAKEFILE false)

# 실행 파일은 빌드 디렉토리 안에 bin 폴더에 출력.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# # set install directory
# set(INSTALL_HOME "/usr/local/MagicDB/")

# 확인할 디렉토리 추가
add_subdirectory(lib)

# # Options. Turn on with 'cmake -Dmyvarname=ON'.
# option(_TEST "Build all tests." OFF) # Makes boolean '_TEST' available.
# if(_TEST)
#   add_subdirectory(tests)
  
#   include(CTest)
  
#   add_test(test-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Awesome_mix_vol_1_test)
#   add_test(test-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Awesome_mix_vol_1_test)
# endif()
  add_subdirectory(tests)
  
  include(CTest)
  
  add_test(test-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Awesome_mix_vol_1_test)
  add_test(test-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Awesome_mix_vol_1_test)

message(STATUS "Compiler")
message(STATUS " - ID       \t: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS " - Version  \t: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " - Path     \t: ${CMAKE_CXX_COMPILER}")