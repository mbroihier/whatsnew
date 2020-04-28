#
#  As a minimum, set URL, SOURCE_DESTINATION, and CREDENTIALS to the values necessary to access your email account and your script
#  Do NOT commit this file after alteration because you'll be storing that information into your repository.
#
cmake_minimum_required(VERSION 3.7)
project(whatsnew)
set(PROJECT_VERSION "0.9")
set(PROTOCOL "smtps:")
set(URL "smtp.someserver:465")
set(SOURCE_DESTINATION "address@someserver")
set(CREDENTIALS "address@someserver:pw")
set(DEBUG 0)
set(POLL_RATE 300)
configure_file("src/whatsnewConfig.h.in" "include/whatsnewConfig.h")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_BINARY_DIR}/include")
set(WHATSNEW_SRC src/whatsnew.cc)
add_executable(whatsnew ${WHATSNEW_SRC})
target_link_libraries(whatsnew curl)
