print "#g 0 x -1.2 1.2 y -1.2 1.2\n";
printf "0 0\n%e %e\n#\n0 0\n%e %e\n#\n",cos(.5),sin(.5),cos(2.5),sin(2.5);
  print "#cm 2\n";
for ($t=0;$t<=6.28;$t+=0.02)
{
  $x++;
  if ($x % 2 == 0) {print "#\n";}
  printf "%e %e\n",cos($t),sin($t);
}
print "#cm 0\n";
for ($t=0.5;$t<=2.51;$t+=0.05)
{
  printf "%e %e\n",cos($t),sin($t);
}

print "#\n0.35 0.1 \"1 radius\"\n";
print "#\n-.2 1.1 \"\\gh radiuses\"\n";
print "#cs 1\n-.1 .3 \"\\gh radians\"\n";
