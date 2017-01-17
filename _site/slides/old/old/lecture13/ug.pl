print "#y -80 40 h 0.7 lt \"U\\sbG\\eb for 1 kg mass\" ly \"U\\sbG\\eb (MJ)\" lx \"Distance from center of earth (10\\sp3\\ep km)\"\n";
print "#m 2\n0 0\n1e2 0\n#cm 1 m 1\n";
for ($r=6300000;$r<1e8;$r+=1e5)
{
$G=6.67e-11;
$m=5.98e24;
$U=-$G*$m/$r;
printf "%e %e\n",$r/1e6,$U/1e6;
}
