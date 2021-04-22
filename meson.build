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

dep_m = cc.find_library('m')

ctypes_srcs = [
  'src/list.c',
  'src/vector.c',
  'src/vtree.c',
  'src/stack.c',
  'src/dhash.c',
  'src/queue.c',
  'src/radix/radix.c',
  'src/os/buffer.c',
  'src/os/os-compatibility.c',
  'src/os/exec.c',
  'src/os/file.c',
]

lib_ctypes = static_library(
  'ctypes',
  ctypes_srcs,
  c_args : global_args,
  dependencies : [dep_m],
  include_directories: include_directories('include'),
)

dep_ctypes = declare_dependency(
  link_with : lib_ctypes,
  include_directories : include_directories('include'),
  compile_args : global_args
)

subdir('test')
