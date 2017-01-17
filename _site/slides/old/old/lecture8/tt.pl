print "#r 0.05 x 0 100 y 0 100\n";
$c=3*10**8;
for ($t=0;$t<2e9;$t+=1e6)
{
  printf "%f %f\n",$t/1e6,$t/1e6;
}

print "#cm 2\n";
for ($v=0;$v<0.9999*$c;$v+=1e6)
{
  $p=$v/sqrt(1-($v*$v/$c/$c));
  printf "%e %e\n",$p/1e6,$v/1e6;
}

print "#lx \"Time (millions of seconds)\"\n";
print "#ly \"Velocity (millions of m/s)\"\n";


