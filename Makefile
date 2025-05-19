run:compile
	make run -C ./rtl

compile:
	make -C ./os
	make -C ./rtl

vis:
	mkdir -p ./blkrvis/data
	cp $(file_path) ./blkrvis/data/data.txt;
	python3 -m http.server 8000 -d ./blkrvis;

clean:
	make clean -C ./os
	make clean -C ./rtl

.PHONY:run clean compile vis