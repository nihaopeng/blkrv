run:compile
	make run -C ./rtl

compile:
	make -C ./os
	make -C ./rtl

vis:
	mkdir -p ./blkrvis/data
	@if [ -z "$(file_path)" ]; then \
		python3 -m http.server 8000 -d ./blkrvis; \
	else \
		cp "$(file_path)" ./blkrvis/data/data.txt; \
		python3 -m http.server 8000 -d ./blkrvis; \
	fi

clean:
	make clean -C ./os
	make clean -C ./rtl

.PHONY:run clean compile vis