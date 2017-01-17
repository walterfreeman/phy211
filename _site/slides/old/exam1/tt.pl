print "#lx \"Time (s)\" ly \"Position (m)\" y -12 12\n";
$x=0;
$v=0;
$dt=0.001;
for ($t=0;$t<11;$t+=$dt)
{
  if ($t<3) {$a=5;}
  elsif ($t<6) {$a=-10;}
  else {$a=3;}
  $x+=$v*$dt + 0.5 * ;
  $v+=$a*$dt;

  print "$t $x\n";
}
