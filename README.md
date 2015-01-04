# Whyyyy?

Because stuff. Oh and I want to learn OpenGL + C even though it somehow seems like masochism.

# k how do I run this?

Make sure you have SDL 2.0, GLEW and OpenGL + their respective header files (on Debian these are the `-dev` packages).
Then you should be able to just `make` this.

You can also try using `make watch` (if you have inotify-tools installed) to have it automatically rebuild when you change
things.

# Right, but why am *I* here?

I want to learn graphics programming with SDL and OpenGL. In C. Because <s>I hate myself</s> I thought that would be cool.

My target is to create a procedural planet generator with a user-controllable fly-by, though I will probably take
a fairly long time to get there. In the meantime, I will be making:

* [ ] Some 'Hello World' type programs
    * [x] Simple plane in the window space
    * [ ] Simple plane in a perspective in window space
    * [ ] Rotating cube in a perspective in window space
* [ ] A simple image viewer
    * [ ] Texture display in pixel space
    * [ ] Loading arbitrary files and padding them out to 2^x size if needed
* [ ] GL Pong
    * [ ] Simple physics engine
    * [ ] An interactive game with scoring for 2 players
    * [ ] Rubber-band AI for a computer player (selecting single and multiplayer mode via console switch)
* [ ] Other physical simulations
    * [ ] ...

...

* [ ] Procedural planet generator
    * [ ] Should be possible to get a new planet without restarting the program
    * [ ] Should be possible to change the generation parameters on the fly
    * [ ] Should be possible to fly by a planet using input controls (keyboard, mouse, joystick, touchscreen?)
    * [ ] Planet generation:
        * [ ] Should be realistic
        * [ ] Should use simulations of real-world planet forming processes (tectonic plates, climates):
            * See [this blog post][1]
        * [ ] Should not take 6 billion years (ie. should be fairly fast)
            * Re: changing generation parameters on the fly: It would be cool if I only needed to recompute the phases whose parameters changed
    * [ ] Should be able to produce high-resolution renders (off-screen rendering)
    * [ ] Should be able to save and load generation parameters to files and have them deterministically produce planets

[1]: http://experilous.com/1/blog/post/procedural-planet-generation
