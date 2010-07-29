#!/usr/bin/perl
use strict;
use warnings;




while (my $seq = <>)
{
    my @seq = split(/:\t/, $seq);
    my $name = $seq[0];
    my $str = $seq[1];
    $str =~ s/\s//g;
    print ">$name\n$str\n";
}
