run:compile
	make run -C ./rtl

compile:
	make -C ./os
	make -C ./rtl

clean:
	make clean -C ./os
	make clean -C ./rtl

.PHONY:run clean