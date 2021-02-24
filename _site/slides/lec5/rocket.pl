$dt = 0.1;
$vx=0;
$vy=0;
$x=0;
$y=0;

$wait = 0;

while (1)
{
  $wait = $wait + 1;
  $dt = 0.1;
  if ($wait < 1000) {$dt = 0;}
  if ($y < 0) {$dt = 0;}
  print ("T -0.2 0.9\nTrajectory\n");
  print ("!T -0.2 0.9\n!Velocity vs Time\n");
  print ("!!T -0.2 0.9\n!!Acceleration vs Time\n");

  $x += $vx * $dt/2;
  $y += $vy * $dt/2;

  if ($t > 10) {$ax=0; $ay=-9.8;} else {$ax=10*cos(30/180*3.14159); $ay=5;}
  $vx += $ax * $dt;
  $vy += $ay * $dt;
  
  $x += $vx * $dt/2;
  $y += $vy * $dt/2;

  $t += $dt;

  print ("ct3 0 $x $y 0 5\n");

  print ("!C 1 0 0\n");
  print ("!ct3 0 $t $vx 0 1\n");
  print ("!C 0 1 0\n");
  print ("!ct3 1 $t $vy 0 1\n");
  
  print ("!!C 1 0 0\n");
  print ("!!ct3 0 $t $ax 0 0.5\n");
  print ("!!C 0 1 0\n");
  print ("!!ct3 1 $t $ay 0 0.5\n");

  print ("F\n");
  print ("!F\n");
  print ("!!F\n");
  }
