#!/usr/bin/perl
use strict;
use warnings;
my $start_offset_begin = 12;
my $start_offset_end = 15;
my $stop_offset_begin = 12;
my $donor_offset_begin = 12;
my $acceptor_offset_end = 9;

my ($seqfile) = @ARGV;
die("must define a valid seqfile to read") unless ( defined $seqfile && -r $seqfile);
my %seqs;
open (SEQ, "<$seqfile") or die "Cant open $seqfile!\n"; 
while(my $seq = <SEQ>){
    my @seqentry = split(/:\t/, $seq);
    my $name = $seqentry[0];
    $name =~ s/(.*)?:/$1/g;
    my $str = $seqentry[1];
    my @sequence = split(/ /, $str);
    $seqs{$name} = \@sequence;
}

close(SEQ);


while (my $seq = <STDIN>)
{
    my @seqentry = split(/:\t/, $seq);
    my $name = $seqentry[0];
    my @names = split(/:/, $name);
    $name = $names[0];
    my $str = $seqentry[1];
    my @sequence = split(/ /, $str);
    my $current_state = $sequence[0];
    my $start = 0;
    push @sequence, "+_+_+_+_+_+_+";
    for(my $i = 1; $i < scalar(@sequence)-1; $i++)
    {
	if(!($sequence[$i] eq $current_state)){
	    print "$name\t$current_state\t".($start+1)."\t".($i)."\t".($i -  $start)."\t";
	    my @aux =  @{$seqs{$name}};
	    for(my $j = $start; $j < $i; $j++){
		print $aux[$j];
	    }
	    print "\n";
	    $start = $i;
	    $current_state = $sequence[$i];
	}
    }
}
