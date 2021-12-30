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

add_executable(spawn_anyexc ${SRCS})

target_compile_options(spawn_anyexc ${TCMALLOC_COMPILE_OPTIONS})
target_link_libraries(spawn_anyexc
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
