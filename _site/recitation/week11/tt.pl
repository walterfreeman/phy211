$pi=3.14159265;
print "#lx \"Time (s)\" ly \"Displacement (cm)\" x 0 13 y -1.5 1.5 cm 1 h 0.5\n";
for ($t=0; $t<4*$pi; $t+=0.01)
{
	printf "%e %e\n",$t,cos($t);
}
print "#cm 0 m 2\n0 0\n13 0\n";
