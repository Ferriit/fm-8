CXX := "g++"
asm_csrc := `find ./asm -name "*.cpp"`
emu_csrc := `find ./emu -name "*.cpp"`

_build_dir:
	mkdir -p build/

# Builds the Assembler
asm: _build_dir
	{{CXX}} {{asm_csrc}} -o dist/assembler -std=c++20

# Builds the Emulator
emu: _build_dir
	{{CXX}} {{emu_csrc}} -o dist/emulator -std=c++20

# Builds the Assembler and Emulator
[default]
build: emu asm