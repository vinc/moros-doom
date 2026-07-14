# MOROS DOOM

A port of [DOOM](https://github.com/id-Software/DOOM)
to [MOROS](https://github.com/vinc/moros),
built on [doomgeneric](https://github.com/ozkl/doomgeneric).

MOROS doesn't have a libc so DOOM is a freestanding executable
using [nanoprintf](https://github.com/charlesnicholson/nanoprintf),
and some functions from [musl](https://musl.libc.org/).

## Build on Unix

    $ cd moros
    $ make install

## Run on MOROS

    > pkg install doom
    > doom -iwad /lib/doom/squashware.wad

## License

DOOM and doomgeneric are released under the GPL, so this port is GPL as well,
nanoprintf is public domain and the borrowed musl functions are MIT-licensed.
