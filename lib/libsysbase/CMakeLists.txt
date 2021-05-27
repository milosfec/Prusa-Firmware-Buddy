add_library(
  sysbase
  dirent.c
  iosupport.c
  )

target_include_directories(sysbase PUBLIC .)

add_library(sysbase::sysbase ALIAS sysbase)
