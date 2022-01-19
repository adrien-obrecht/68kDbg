**Motorola 68k debugger**

This project's aim was to build a graphical debugger for the 68k motorola asm. It is hosted on github at https://github.com/adrien-obrecht/68kDbg.
It's made in c and needs gtk3.0 to compile. It is available for download here: https://www.gtk.org/.

Once the repo cloned, you can compile the project with `make` and you can lauch the executable with `./bin/main`

**Features :**

- Load any .s file, supporting most of the basic instructions.
- Real time display of the registers, memory and CCR
- Supports play/pause of execution
- Gives somewhat understandable warnings to ease code developpement

**Sources :**

- [M68k reference manual](https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf)
- [Great beginner tutorial](https://mrjester.hapisan.com/04_MC68/Sect01Part05/Index.html)
- [glib documentation](https://docs.gtk.org/glib/struct.List.html)
- [gtk 3.0+ documentation](https://docs.gtk.org/gtk3/method.Grid.set_column_spacing.html)

