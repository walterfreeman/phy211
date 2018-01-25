use POSIX;
print "#x -2 2 y -2 2\n";
print "circ 0 0 1.1\n";
print "circ 0 0 1.15\n";

$x=0;
$y=-1;
$vx=0;
$vy=0;

$w=2.7;
$dt=0.0001;
print "#cm 1\n";
for ($t=0;$t<10;$t+=$dt)
{
  $vy -= 9.8*$dt;
  $x+=$vx*$dt;
  $y+=$vy*$dt;
  $lastsync=$sync;
  $sync=0; 
  if ($x*$x + $y*$y > 1)
  {
    $r=sqrt($x*$x + $y*$y);
    $x/=$r;
    $y/=$r;
    # synch v
    $vx = -$y*$w;
    $vy = $x*$w;
    $sync=1;
  }
  else {$sync=0;}
  if ($sync == 0 && $flag==0)
  {
    $s++;
  }
  else {$s=0;}
  if ($s==1000)
  {
    $flag=1;
    print "#cm 3\n";
    printf "#\n%e %e\n0 0\n1.1 0\n",$x*1.1,$y*1.1;
    print "#cm 1\n";
  }
  if (floor($t*50) % 2 == 0)
  {
    print "$x $y\n";
  }
  else {print "#\n";}
  if ($x < 0 && $x+$vx*$dt > 0) {last;}
}
