all:
	make -C ./os
	make -C ./rtl

run:
	make run -C ./rtl

clean:
	make clean -C ./os
	make clean -C ./rtl