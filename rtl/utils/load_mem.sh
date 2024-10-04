./manage_mem -src ../devices/bios.o -tar ../devices/bios\
 -src_start 52 -src_end 1139 -tar_start 0 -tar_end 1087 #加载bios中的拉取指令
./manage_mem -src ../os/boot/bootsect.o -tar ../devices/flash\
 -src_start 52 -src_end 123 -tar_start 0 -tar_end 71 #加载bios中的拉取指令
