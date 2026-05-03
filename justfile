CXX := "g++"
asm_csrc := `find ./asm -name "*.cpp"`
emu_csrc := `find ./emu -name "*.cpp"`

_build_dir:
	mkdir -p build/

# Builds the Assembler
asm: _build_dir
	{{CXX}} {{asm_csrc}} -o dist/assembler

# Builds the Emulator
emu: _build_dir
	{{CXX}} {{emu_csrc}} -o dist/emulator

# Builds the Assembler and Emulator
[default]
build: emu asm