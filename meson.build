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

sanitizer = get_option('sanitizer')
if sanitizer != 'none'
  san_flags = ['-fsanitize=' + sanitizer, '-fno-omit-frame-pointer']
  add_project_arguments(cc.get_supported_arguments(san_flags), language : 'cpp')
  add_project_link_arguments(cc.get_supported_link_arguments(san_flags), language : 'cpp')
endif

subdir('libpz')

# Optional gtest suite (meson_options.txt → -Dtests=enabled|disabled).
if get_option('tests').enabled()
  subdir('test')
endif
