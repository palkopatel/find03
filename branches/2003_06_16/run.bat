@echo off
:running
  findw_t -c -l -e
  if not exist "2log" goto next
  echo new pass
  move "2log" "1hrefs"
  goto running
:next
  if exist "1hrefs" del "1hrefs"
  echo next generation
  findw_t -d %1 -e
