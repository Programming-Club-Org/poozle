inc_dir = include_directories('../include')

libpz_util_sources = [
  'pz_analysis.cpp',
  'pz_buffer.cpp',
  'pz_core.cpp',
]

libpz_util = static_library(
  'pz_util',
  libpz_util_sources,
  include_directories : inc_dir,
)


libpz_util_dep = declare_dependency(
  link_with : libpz_util,
)
