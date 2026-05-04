CXX := "g++"
asm_csrc := `find ./asm -name "*.cpp"`
emu_csrc := `find ./emu -name "*.cpp"`

_build_dir:
	mkdir -p build/
	mkdir -p dist/


# Builds the Assembler
build_asm: _build_dir
	{{CXX}} {{asm_csrc}} -o dist/assembler -std=c++20

# Builds the Emulator
build_emu: _build_dir
	{{CXX}} {{emu_csrc}} -o dist/emulator -std=c++20

# Builds and runs the Assembler
asm infile: build_asm
	./dist/assembler {{infile}}

emu: build_emu
	./dist/emulator app.fm8

# Builds the Assembler and Emulator
[default]
build: build_emu build_asm