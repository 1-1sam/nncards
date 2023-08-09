# nncards
TUI flashcard program for Unix-like systems designed with simplicity and 
portability in mind.

## Usage
nncards must be invoked with a card file provided as an argument. Here is how
nncards interprets given files:

* Terms and definitions are seperated by a colon (:). Terms precede the colon,
definitions follow the colon. Any whitespace between the colon and definition
is ignored.
* Lines that start with a # are interpreted as comments and ignored.
* Blank lines are ignored.

example.cards shows how one could format a card file.

nncards is controlled through various key commands.

| Key | Action               |
| --- | ---------------------|
| c   | Next card            |
| z   | Previous card        |
| x   | Flip card            |
| d   | Go to final card     |
| a   | Go to beginning card |
| q   | Quit nncards         |

## Building

The only thing you need to build nncards is a C compiler.

```bash
make
make install DESTDIR=/where/ever
make clean
```

## Credit
[termbox2](https://github.com/termbox/termbox2) - TUI rendering

## License
nncards is licensed under the Simplified BSD License, also called the 
BSD-2-Clause License. View the LICENSE file for more information.
