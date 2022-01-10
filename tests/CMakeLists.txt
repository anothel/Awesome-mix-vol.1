include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)

# 현재 디렉토리에 있는 모든 파일을 SRC_FILES 변수에 추가한다.
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)

add_executable(ByteString_test ${SRC_FILES})

target_compile_features(ByteString_test PRIVATE cxx_std_11)

target_link_libraries(ByteString_test PRIVATE ByteString gtest gmock gtest_main)
