set(TCMALLOC_COMPILE_OPTIONS
  PUBLIC -fno-builtin-malloc
  PUBLIC -fno-builtin-calloc
  PUBLIC -fno-builtin-realloc
  PUBLIC -fno-builtin-free
)

set(SRCS
  main.cpp
  listener.cpp
  session.cpp
)

add_executable(cpp20 ${SRCS})

target_compile_options(cpp20 ${TCMALLOC_COMPILE_OPTIONS})
target_compile_options(cpp20 PUBLIC -fcoroutines)
target_link_libraries(cpp20
  fmt::fmt
  ${Boost_LIBRARIES}
  stdc++fs
  backtrace.a
  profiler_static
  tcmalloc_static
  unwind
  pthread
  dl
  rt
)
