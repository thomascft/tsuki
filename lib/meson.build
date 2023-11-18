files = run_command('find | grep -i .lua', check: true)
lualibs = files.stdout().strip().split('\n')

install_data(lualibs,
  install_dir : get_option('datadir'))
  preserve_path: true,
)
