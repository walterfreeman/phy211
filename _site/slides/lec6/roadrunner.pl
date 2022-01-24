$h=shift;
$v=110 * 1000 / 3600;
print "#x 0 8 y 0 60\n";
for ($t=0;$t<6.8;$t+=0.02)
{
  $x=$v*$t-4.5*$t**2;
  if ($x < 0) {last;}
  print "$t $x\n";
  if ($h == 1 && $x > 30) {last;}
  if ($h == 2 && $x > 30) {print "#cm 2\n"; $h=0;}

}
print "#cm 1 m 3\n";
print "0 30\n7 30\n";
