# panui2
A frontend for mupen64plus.

# Compiling
test.sh generates the frontend executable. build.sh generates the launcher. Modify them to reflect your development environment.

You need an object file for (.o) for Hiro, byuu's GUI toolkit. Of course, this is permissively licensed, but as it's not in this repo, you need to find its source elsewhere if you need to build it. This will change later.

On windows, DLL hell is a massive problem, so this setup is used to "hide" the frontend in a separate application, and the launcher is used to launch the frontend with the bin subdirectory of where they're located added to PATH. All dependencies, and the default emulator plugins, are currently stored in /bin. I might add more separate paths later.

# Usage
There's not much you can do with panui right now. You can load a ROM and use it as a basic launcher for m64p.

# License
panui can be used under a custom license (included), or under the same version of the GPL as Mupen64Plus is licensed under. The custom license allows you to take code from panui and use it for non-GPL projects/products, though I have no idea what in the world you might find it useful for.
