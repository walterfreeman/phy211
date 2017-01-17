while (<STDIN>)
{
  ($t,$x)=split;
  $int+=$x*0.001;
  print "$t $int\n";
} 
