project('ctypes', 'c',
	version : '0.1',
	license: 'MIT/Expat')

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
  include_directories: include_directories('src'),
  install: true,
)

dep_ctypes = declare_dependency(
  link_with: lib_ctypes,
  include_directories: include_directories('src')
)

subdir('test')
