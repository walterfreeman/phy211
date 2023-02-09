$th=.4;
$l=3;
$y3=3-$l*sin($pi/2-$th);
$y4=3-$l*sin($pi/2+$th);
$x3=3-$l*cos($pi/2-$th);
$x4=3-$l*cos($pi/2+$th);
print "va 3 3 $x3 $y3 1\n";
print "va 3 3 $x4 $y4 2\n";
print "va $x3 $y3 $x4 $y4 3\n";

$x3=$x-$l*sin($pi/2-$th);
$x4=$x2-$l*sin($pi/2+$th);
$y3=$y+$l*cos($pi/2-$th);
$y4=$y+$l*cos($pi/2+$th);

