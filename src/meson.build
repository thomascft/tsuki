tsuki_src = files(
  'main.c',
  'widgets.c',
  'signals.c',
  'service.c',
)

tsuki_deps = [
  dependency('gtk4'),
  dependency('lua'),
]

executable('tsuki', tsuki_src, dependencies: tsuki_deps, include_directories: include)
