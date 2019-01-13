$line=0;
$day=1;
while (<STDIN>)
{
  chomp;
  @w=split /,/;
  $date[$line]=$w[0];
  $topic[$line]=$w[1];
  
  unless ($topic[$line] =~ /EXAM/)
  {
    $topic[$line]="<a href=\"slides/lecture$day.pdf\">$topic[$line]</a>";
    $day++;
  }
  $sec8[$line]=$w[2];
  $pages8[$line]=$w[3];
  print STDERR "Read a line: topic $topic[$line], sections $sec8[$line]\n";
  $line++;
}

# calendar

print <<END;
---
layout: page
title: Calendar
permalink: calendar.html
category: top 
use_math: true
---

    
This calendar is tentative and might change based on your input. Exam dates will not change without overwhelming consensus, but topics might be adjusted if the calendar changes.

Note that the textbook does not track our presentation exactly; I've chosen the order of topics to simplify things for you, but that makes it a bit tougher to follow along in the book.
  
END


print "<br><br>\n### Eighth edition page numbers:\n\n";

print "| Class Date    | Topics                                                  | Textbook sections              |\n";
print "|:-------------:|:-------------------------------------------------------:|:------------------------------:|\n";

for ($line=0; $line<@date; $line++)
{
  if ($topic[$line] =~ /FINAL/)
  {
    print "|===============+=========================================================+=====================+\n";
  }
  print "| $date[$line] | $topic[$line] | $sec8[$line] |\n";
}
print "|--------------------------------------------------------------------------------------------------------+-------------------------+--------------+\n\n\n\n";


