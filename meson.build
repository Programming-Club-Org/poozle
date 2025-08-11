project('poozle', 'cpp',
  version: 'v0.1.0',
  meson_version: '>=0.57.0',
  default_options: [
    'buildtype=debugoptimized',
    'b_vscrt=from_buildtype',
    'warning_level=1',
  ]
)

pz_srcs = [
    'api/pz_api_descs.cpp',
]

pz_inc_dirs = include_directories('libpz/include')

poozle_lib = library(
  'poozle',
  pz_srcs,
  include_directories: pz_inc_dirs,
  install: true
)

poozle_dep = declare_dependency(
  include_directories: pz_inc_dirs,
  link_with: poozle_lib
)

install_subdir('libpz', install_dir: get_option('includedir'))
install_headers('api/pz_api_descs.hpp', subdir: 'poozle')