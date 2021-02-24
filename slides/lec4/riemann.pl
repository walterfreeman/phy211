sub f
{
  $t=@_[0];
#  return sin(($t+1)**2/20)*5;
  return $t*0.8+4;
#   if ($t<5) {return 5;} else {return 2;}

}

$bin=shift;
print "#ly \"Velocity (m/s)\" lx \"Time (s)\" h 0.7 cs 0.8 x 0 12 1200 y -4 16 gx 1 gy 0\n";
for ($t=0;$t<10;$t+=0.004)
{
  $x=f($t);
  print "$t $x \"\\oc\"\n";
}
print "#cm 1\n";
$s=0.2000001;

for ($t=0;$t<10;$t+=$bin)
{
  $h=f($t);
  printf "#cm 1\n$t 0\n$t $h\n%e $h\n%e 0\n#cm 2\n",$t+$bin,$t+$bin;
  if ($h > 0)
  {
    for ($tt=$t;$tt<$t+$bin;$tt+=$s)
    {
      for($x=0;$x<$h;$x+=$s)
      {
        if ($x+$s*0.5<4) {print "#cm 3\n";} else {print "#cm 2\n";}
	if ($x+$s > $h) {$ss=$h-$x;} else {$ss=$s};
	printf "%e %e\n%e %e\n#\n",$tt,$x,$tt+$ss,$x+$ss;
	printf "%e %e\n%e %e\n#\n",$tt+$ss,$x,$tt,$x+$ss;
      }
    }
  }
  else
  {
    $h=-$h;
    print "#cm 3\n";
    print stderr "backwards: going to positive height $h\n";
    for ($tt=$t;$tt<$t+$bin;$tt+=$s)
    {
      for($x=0;$x<$h;$x+=$s)
      {
	if ($x+$s > $h) {$ss=$h-$x;} else {$ss=$s};
	printf "%e %e\n%e %e\n#\n",$tt,-$x,$tt+$ss,-$x-$ss;
	printf "%e %e\n%e %e\n#\n",$tt+$ss,-$x,$tt,-$x-$ss;
      }
    }
  }

}

print "#m 2 cm 0\n10 -4\n10 16\n#\n0 12\n12 12\n#\n0 4\n12 4\n";
print "#cs 2 m 1\n12 12 \"  at+v\\sb0\\eb\"\n";
print "#cs 2 m 1\n12 4 \"  v\\sb0\\eb\"\n";
print "#cs 2 m 1\n10 15 \"t\"\n";
