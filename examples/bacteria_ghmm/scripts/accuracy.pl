#!/usr/bin/perl


use strict;
use warnings;

if($#ARGV < 1) {
  die "USAGE $0 <sequence1> <sequence2>\n";
}

my $file1 = shift @ARGV;
my $file2 = shift @ARGV;

my @seq1;
my @seq2;

open (IN, "<$file1") or die "cant open $file1: $!";

foreach my $line (<IN>) 
  {
    my ($name, $s) = split(/\t/, $line);
    my @seq = split(/ /, $s);
    @seq1 = (@seq1, @seq);
  }
close(IN);


open (IN, "<$file2") or die "cant open $file2: $!";
foreach my $line (<IN>) 
  {
    my ($name, $s) = split(/\t/, $line);
    my @seq = split(/ /, $s);
    @seq2 = (@seq2, @seq);
  }
close(IN);

my $correct = 0;
my $error = 0;
for (my $i = 0; $i < scalar @seq1 && $i < scalar @seq2; $i++)
  {
    if($seq1[$i] eq $seq2[$i]) 
      {
	$correct ++;
      }
    else {
      $error ++;
    }
  }

print "Number of correctly predicted positions: $correct\n";
print "Number of incorrectly predicted positions: $error\n";
print "Identity: ".($correct*100.0)/($correct + $error)."\n";
