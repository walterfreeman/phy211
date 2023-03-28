sub force {
  my $x=shift;
  return $x*0.7;
}

print "#x -1.1 13 y -1.1 11 g 0 cm 0\n";
print "#lx \"Compression\" ly \"Force\"\n";

#print "#cm 0\n-1 -2\n-1 11\n12 11\n12 -2\n-1 -2\n#\n";

print "#cm 0\n-1 0\n11 0\n#\n0 -1\n0 11\n#\n";

$xmin=0;
$xmax=10;
$drawmin=5;
$drawmax=10;
$xstep=0.2;
$ystep=0.1;
$slope=0.5;

print "#cs 1\n6 6 \"F = k\\gDx\"\n";

print "#m 2 cm 2\n $xmax -1\n$xmax 8\n#m 1 cs 1.2 cm 2\n10 -1 \"\\gDx\\sb1\\eb\"\n#cm 0\n";
print "#m 2 cm 2\n 5 -1\n5 8\n#m 1 cs 1.2 cm 2\n5 -1 \"\\gDx\\sb2\\eb\"\n#cm 0\n";

for ($x=0; $x<$xmax; $x+=0.001)
{
	$y=force($x);
	print "$x $y \"\\oc\"\n";
}

for ($x=$drawmin; $x<$drawmax; $x+=$xstep)
{
	$y=force($x);
	$x2=$x+$xstep;
	$y2=force($x2);
	if ($y > 0)
	{
		$ymin=0; $ymax=$y;
	}
	else
	{
		$ymin=$y;
		$ymax=0;
	}
	print "#cm 0\n";
	print "$x 0\n$x $y\n$x2 $y\n$x2 0\n#\n";
	print "#cm 1\n";
$slope=0.6;
	for ($y=$ymin-$xstep*$slope; $y<$ymax; $y+=$ystep)
	{
		$lx1=$x;
		$lx2=$x2;
		$ly1=$y;
		$ly2=$y+$xstep*$slope;
		#		print "DEBUG line from $lx1, $ly1 to $lx2, $ly2\n";
		if ($ly2 > $ymax)
		{
			$lx2 -= ($ly2-$ymax)/$slope;
			$ly2=$ymax;
		}
		if ($ly1 < $ymin)
		{
			$lx1 -= ($ly1-$ymin)/$slope;
			$ly1=$ymin;
		}
		print "$lx1 $ly1\n$lx2 $ly2\n#\n";

	}
$slope=0.4;
	for ($y=$ymin; $y<$ymax+$xstep*$slope; $y+=$ystep)
	{
		$lx1=$x;
		$lx2=$x2;
		$ly1=$y;
		$ly2=$y-$xstep*$slope;
		#		print "DEBUG line from $lx1, $ly1 to $lx2, $ly2\n";
		if ($ly1 > $ymax)
		{
			$lx1 += ($ly1-$ymax)/$slope;
			$ly1=$ymax;
		}
		if ($ly2 < $ymin)
		{
			$lx2 += ($ly2-$ymin)/$slope;
			$ly2=$ymin;
		}
		print "$lx1 $ly1\n$lx2 $ly2\n#\n";

	}
}
#print "#cm 0\n";
$drawmin=0;
$drawmax=0;
$xstep=0.2;
$ystep=0.12;
$slope=0.3;
for ($x=$drawmin; $x<$drawmax; $x+=$xstep)
{
        $y=force($x);
        $x2=$x+$xstep;
        $y2=force($x2);
        if ($y > 0)
        {
                $ymin=0; $ymax=$y;
        }
        else
        {
                $ymin=$y;
                $ymax=0;
        }
        print "#cm 0\n";
        print "$x 0\n$x $y\n$x2 $y\n$x2 0\n#\n";
        print "#cm 2\n";
$slope=0.4;
        for ($y=$ymin-$xstep*$slope; $y<$ymax; $y+=$ystep)
        {
                $lx1=$x;
                $lx2=$x2;
                $ly1=$y;
                $ly2=$y+$xstep*$slope;
                #               print "DEBUG line from $lx1, $ly1 to $lx2, $ly2\n";
                if ($ly2 > $ymax)
                {
                        $lx2 -= ($ly2-$ymax)/$slope;
                        $ly2=$ymax;
                }
                if ($ly1 < $ymin)
                {
                        $lx1 -= ($ly1-$ymin)/$slope;
                        $ly1=$ymin;
                }
                print "$lx1 $ly1\n$lx2 $ly2\n#\n";

        }
$slope=0.6;
        for ($y=$ymin; $y<$ymax+$xstep*$slope; $y+=$ystep)
        {
                $lx1=$x;
                $lx2=$x2;
                $ly1=$y;
                $ly2=$y-$xstep*$slope;
                #               print "DEBUG line from $lx1, $ly1 to $lx2, $ly2\n";
                if ($ly1 > $ymax)
                {
                        $lx1 += ($ly1-$ymax)/$slope;
                        $ly1=$ymax;
                }
                if ($ly2 < $ymin)
                {
                        $lx2 += ($ly2-$ymin)/$slope;
                        $ly2=$ymin;
                }
                print "$lx1 $ly1\n$lx2 $ly2\n#\n";

        }
}
print "#cm 0\n0 0\n0 1\n";

