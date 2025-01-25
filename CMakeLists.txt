cmake_minimum_required (VERSION 3.8)

# Włącz przeładowywanie kodu XAML na gorąco dla kompilatorów MSVC, jeśli jest to obsługiwane.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Diver")

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.

# Pobierz Raylib
include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG master
  GIT_SHALLOW 1
)
FetchContent_MakeAvailable(raylib)
message(STATUS "Raylib library downloaded and configured.")

# Dodaj bibliotekę DiverLib
add_library(DiverLib STATIC Diver.cpp Diver.h)
target_link_libraries(DiverLib PRIVATE raylib)
target_include_directories(DiverLib PRIVATE ${raylib_SOURCE_DIR}/src)

# Dodaj plik wykonywalny
add_executable(Diver "Diver.cpp" "Diver.h")
set_property(TARGET Diver PROPERTY CXX_STANDARD 20)
target_link_libraries(Diver PRIVATE raylib)

# Skopiuj zasoby
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

# Testy z GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Dodaj testy
add_executable(DiverTests "tests/Diver_test.cpp")
target_link_libraries(DiverTests PRIVATE gtest gtest_main DiverLib raylib)

# Włącz testowanie
enable_testing()
add_test(NAME RunAllTests COMMAND DiverTests)