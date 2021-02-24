#print "#lx \"Time (s)\" #ly \"Position (m)\"\n";
for ($t=-3;$t<10;$t=$t+0.1)
{
  $x=-4.5*$t**2 + 30.556*$t;
  print "$t $x\n";
}
print "#cm 1\n-3 30\n10 30\n";
