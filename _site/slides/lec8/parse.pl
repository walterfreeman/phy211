while (<STDIN>)
{
  chomp;
#  print "line is $_\n";
  @w=split /,/;
  $w[4]=~s/\"//g;
  print "$w[4]\n";
 
}
