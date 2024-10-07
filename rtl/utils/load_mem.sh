./manage_mem -src ../devices/bios.o -tar ../devices/bios\
 -src_start 52 -src_end 1139 -tar_start 0 -tar_end 1087 #加载bios中的拉取指令
./manage_mem -src ../../os/boot/bootsect.o -tar ../devices/flash\
 -src_start 52 -src_end 127 -tar_start 0 -tar_end 75 #加载bootsect
./manage_mem -src ../../os/init/sysimg -tar ../devices/flash\
 -src_start 0 -src_end 4768 -tar_start 512 -tar_end 5280 #加载sys
