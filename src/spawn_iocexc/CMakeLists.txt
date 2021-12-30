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

add_executable(spawn_iocexc ${SRCS})

target_compile_options(spawn_iocexc ${TCMALLOC_COMPILE_OPTIONS})
target_link_libraries(spawn_iocexc
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
