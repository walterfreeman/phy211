---
layout: page
title: Installing anim
permalink: anim-install.html
use_math: true
---

You can install the animation utility we're using on your own Linux or MacOS computer. To do this:

* Download it. It's available as a "tarball" <a href="anim-307.tar">here</a>.
* (Optional) Make a directory for anim to live in: `mkdir anim` and then `cd anim`
* (Optional) Move the tarball into that directory: `mv ~/Downloads/anim-307.tar .`
* Extract the files: `tar xvf anim-307.tar`
* Linux: Install needed packages: `sudo apt install freeglut3-dev libglew-dev`
* Compile: `make anim` (for Linux machines) or `make anim-mac` (for Macs)
* Install: `sudo make install`

The manual is included as PDF. Note that this distribution doesn't have the examples included, since one of them is your
homework!
