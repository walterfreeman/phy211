$n=shift;
$dx=0.1;
for ($x=0;$x<6-$dx/2;$x+=$dx)
{
  $x2=$x+$dx/2;
  $x3=$x+$dx;
  printf "v %e %e %e %e\n",$x,6+0.5*sin($x*$n/6*3.14159),$x2,6+0.5*sin($x2*$n/6*3.14159);
  printf "v %e %e %e %e\n",$x,6-0.5*sin($x*$n/6*3.14159),$x3,6-0.5*sin($x3*$n/6*3.14159);
}
