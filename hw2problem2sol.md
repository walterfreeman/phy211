---
layout: page
permalink: hw2p2.html
use_math: true
---

## Solution to HW2 Problem 2 (video lost due to a technical issue):

You know the cannonball travels a distance $d=2$ km and it's elevated an angle $\theta=45^\circ$ above the horizontal.

This means that the initial velocity vector is 

$v_{x,0} = v_0 \cos \theta$
$v_{y,0} = v_0 \sin \theta$

This means that the position and velocity relations become:

$x(t) = \frac{1}{2}a_xt^2 + v_{x,0} t + x_0 = (v_0 \cos \theta)t$
$y(t) = \frac{1}{2}a_yt^2 + v_{y,0} t + y_0 = -\frac{1}{2}gt^2 +
 (v_0 \sin \theta)t$
 
 where we've chosen up to be positive, and the origin to be where the bombard is.
 
 Then we ask: "What $v_0$ makes the cannonball come down a distance $d$ away, i.e. what $v_0$ makes $x=d$ at the time $y=0$?"
 
 This gives us: 
 
$ 0 = -\frac{1}{2}gt^2 + (v_0 \sin \theta)t$

which, after some algebra, tells us that

$t=\frac{2 v_0 \sin \theta}{g}$.

Substitute this into the $x$-equation to get:

$d=\frac {2v_0^2 \cos \theta \sin \theta}{g}$

Solve for $v_0$ to get 

$v_0 = \sqrt{\frac{gd}{2 \cos \theta \sin \theta}}$

Observe that $\cos \theta = \sin \theta = \frac{1}{\sqrt{2}}$ for $\theta=45^\circ$, so the denominator is just unity. So

$v_0 = \sqrt{g}{d} = 140\, \rm m/\rm s$


for the values given in the problem.
