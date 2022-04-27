$R=8;
$r=3.2;
$w=4;
$win=0.6;

print "#cm 3\n";

print "line -$w -$R -$win -$R\n";
print "line  $w -$R  $win -$R\n";
print "line -$w  $R -$win  $R\n";
print "line  $w  $R  $win  $R\n";

print "line -$w -$R -$w $R\n";
print "line  $w -$R  $w $R\n";

print "line -$win -$R -$win -0\n"; 
print "line  $win -$R  $win -0\n"; 
print "line -$win  $R -$win  0\n"; 
print "line  $win  $R  $win  0\n"; 

print "line -$win -$r $win -$r\n";
print "line -$win  $r $win  $r\n";


$rs=0.06;

for ($th=-3.14159/2; $th<3.14159/2; $th+=$rs)
{
    $y=$R*sin($th);
    print "line -$w $y -$win $y\n";
    print "line  $w $y  $win $y\n";
}

#for ($x=$win; $x<$w; $x+=$rs)
#{
#    print "line -$x -$R -$x $R\n";
#    print "line  $x -$R  $x $R\n";
#}

print "#cm 0\n";
$xs=0.001;
$wind=40;
for ($x=-$win; $x<$win; $x+=$xs)
{ 
    $th=$x * $wind;
    $y=($r*1.03) * sin($th);
    if (cos($th) > 0)
    {
	$xnew = $x+$xs;
	$thnew = $xnew * $wind;
	if (cos($th) > 0)
	{
	    $ynew=($r*1.03) * sin($thnew);
	    print "line $x $y $xnew $ynew\n";
	}
    }
 }
printf "line %e %e %e %e\n",$win/2,$r,$win/2,10;

