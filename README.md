# nncards
**nncards** is a simple TUI flashcard program written in C. It creates
flashcards from a given card file.

## Usage
**nncards** must be given at least 1 card file as an argument. Here is how
**nncards** interprets given files:

* Terms and definitions are seperated by a colon (:). Terms precede the colon,
definitions follow the colon. Any whitespace between the colon and definition
is ignored.
* Lines that start with a # are interpreted as comments and ignored.
* Blank lines are ignored.

*example.cards* shows how one could format a card file.

**nncards** is controlled through various key commands.

| Key                     | Action               |
| ----------------------- | -------------------- |
| Right Arrow, l, c       | Next card            |
| Left Arrow, h, z        | Previous card        |
| Space, Up/Down Arrow, x | Flip card            |
| Page Down, d            | Go to final card     |
| Page Up, a              | Go to beginning card |
| Escape, q               | Quit nncards         |

Consult the manual for further documentation.

## Building

The only thing you need to build **nncards** is a C compiler and make.

```bash
make
make install # run as root if you are installing directly into system
make clean
```
The nncards_version variable can be set if you wish to compile nncards with a
special version number.

```bash
make nncards_version=13.37
```

## Credit
[termbox2](https://github.com/termbox/termbox2) - TUI rendering

## License
**nncards** is licensed under the Simplified BSD License, also called the 
BSD-2-Clause License. View the LICENSE file for more information.
