$th=0.4;
$pi=3.14159265;
$l=1;
$x=3+cos($pi/2-$th)*3;
$y=3+sin($pi/2-$th)*3;
$x2=3+cos($pi/2+$th)*3;

$x3=$x-$l*sin($pi/2-$th);
$x4=$x2-$l*sin($pi/2+$th);
$y3=$y+$l*cos($pi/2-$th);
$y4=$y+$l*cos($pi/2+$th);


for ($t=0;$t<360;$t+=4)
{
  $t2=$t+1.5;
  print "arc 3 3 3 $t $t2\n";
}
print "va $x $y $x3 $y3 1\n";
print "va $x2 $y $x4 $y4 2\n";
