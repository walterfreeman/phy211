$m=3.6;
$muk=0.3;
$mus=0.4;
$g=9.8;

for ($th=0; $th<1; $th+=0.001)
{
  if ($th < sin($mus)/cos($mus))
  {
    $ff=$m*$g*sin($th);
  }
  else
  {
    $ff=$m*$g*cos($th)*$muk;
  }
  printf "%e $ff\n",$th*180/3.14159;
}
