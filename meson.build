project('poozle', 'cpp',
  version : '0.1.0',
  default_options : [
    'cpp_std=c++17',
    'warning_level=2',   # Meson’s built-in warning level
  ]
)

cc = meson.get_compiler('cpp')
add_project_arguments(
  cc.get_supported_arguments([
    '-Wall',
    '-Wextra',
  ]),
  language : 'cpp'
)

subdir('libpz')
