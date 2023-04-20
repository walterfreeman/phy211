all: anim membrane pend overrelax balls_box efield drum
.PHONY: all
clean: 
	rm anim membrane pend overrelax balls_box efield drum 
anim: anim.c 
	gcc anim.c -lGL -lGLU -lglut -lm -o anim -O3 -w
membrane: membrane.c 
	gcc membrane.c -lm -o membrane -O3
pend: pend.c
	gcc pend.c -lm -o pend -O3
overrelax: overrelax.c	
	gcc overrelax.c -lm -o overrelax -O3
balls_box: balls_box_masses.c
	gcc balls_box_masses.c -lm -o balls_box -O3
efield: efield.cpp vector.cpp vector.h
	g++ efield.cpp vector.cpp -lm -o efield -O3
drum: drum-serial.cpp
	gcc drum-serial.cpp -lm -o drum -O3
