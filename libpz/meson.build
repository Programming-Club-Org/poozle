inc_dir = include_directories('include')

subdir('util')

libpz_dep = declare_dependency(
  include_directories : inc_dir,
  dependencies : [libpz_util_dep],
)

install_headers(
  files([
    'include/pz_analysis.hpp',
    'include/pz_buffer.hpp',
    'include/pz_core.hpp',
    'include/pz_cxx_std.hpp',
    'include/pz_error.hpp',
    'include/pz_std.hpp',
    'include/pz_types.hpp',
    'include/pz_util.hpp',
  ]),
  subdir : 'poozle'
)
