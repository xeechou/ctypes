project('ctypes', 'c',
	version : '0.1',
	default_options: [
	  'warning_level=3',
	  'c_std=gnu11',
	  'werror=true'
	],
	license: 'MIT/Expat')

cc = meson.get_compiler('c')

global_args_maybe = [
	'-Wno-unused-parameter',
	'-Wno-missing-field-initializers',
	'-Wno-pedantic',

]
global_args = []

foreach a : global_args_maybe
  if cc.has_argument(a)
    global_args += a
  endif
endforeach

if get_option('buildtype') == 'release' and cc.has_argument('-fvisibility=hidden')
  global_args += '-fvisibility=hidden'
endif

ctypes_srcs = [
  'src/list.c',
  'src/vector.c',
  'src/vtree.c',
  'src/stack.c',
  'src/dhash.c',
  'src/queue.c',
  'src/os/buffer.c',
  'src/os/os-compatibility.c',
  'src/os/exec.c',
  'src/os/file.c',
]

lib_ctypes = static_library(
  'ctypes',
  ctypes_srcs,
  c_args : global_args,
  include_directories: include_directories('src'),
)

dep_ctypes = declare_dependency(
  link_with : lib_ctypes,
  include_directories : include_directories('src'),
  compile_args : global_args
)

subdir('test')
