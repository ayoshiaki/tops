#!/usr/bin/perl

use strict;
use warnings;
my $first_exon = 0;
my $input_phase;
my $output_phase;
my @lines;
while (my $line = <STDIN>) {
    if($line =~ m/^\s*$/) 
    {
	next;
    }
    my @fields = split(/\t/, $line);
    push @lines, \@fields;
}
my $sequence_of_states;


for (my $i = 0; $i < scalar (@lines); $i++) 
{
    my @fields = @{$lines[$i]};
    my $next_is_stop = 0;
    my @next_fields ;
    if(($i + 1) < scalar(@lines)) {
	@next_fields = @{$lines[$i+1]};
	if($next_fields[2] =~ m/stop_codon/) 
	{
	    $next_is_stop = 1;
	}
    }
    if($fields[2] =~ /start_codon/) {
	$sequence_of_states .=  " N start";
	$first_exon = 1;
    } elsif(($fields[2] =~ /CDS/) && ($first_exon) && ($next_is_stop)) { 
	$first_exon = 0;
 	$sequence_of_states .= " ES";
    } elsif(($fields[2] =~ /CDS/) && ($first_exon) ){
	$input_phase = 0;
	$output_phase = ($fields[4] - $fields[3] + $input_phase)%3;
       	$sequence_of_states .=  " EI".$output_phase." don".(($output_phase+1)%3)." ";
	for(my $i  = ($fields[4] + 1); scalar(@next_fields) > 0 && ($i < $next_fields[3]); $i++)
	{
	    	$sequence_of_states .=  "I".(($output_phase+1)%3)." ";
	}
	$sequence_of_states .= "acc".(($output_phase+1)%3);
	$first_exon = 0;
    } elsif (($fields[2] =~ /CDS/) && ($next_is_stop) ) {
	$input_phase = ($output_phase+1)%3;
	$sequence_of_states .= " ET".$input_phase;
    } elsif ($fields[2] =~ /CDS/ ) {
	$input_phase = ($output_phase+1)%3;
	$output_phase = ($fields[4] - $fields[3] + $input_phase)%3;
	$sequence_of_states .= " E".$input_phase.$output_phase." don".(($output_phase+1)%3)." ";
	for(my $i  = ($fields[4] + 1); scalar(@next_fields) > 0 && ($i < $next_fields[3]); $i++)
	{
	    $sequence_of_states .= "I".(($output_phase+1)%3)." ";
	}
	$sequence_of_states .= "acc".(($output_phase+1)%3)." " ;
    }elsif ($fields[2] =~ /stop_codon/) { 
	$sequence_of_states .= " stop N";
    }
 }
$sequence_of_states =~ s/\s+/ /g;
print "STATES:\t".$sequence_of_states."\n";
my $reverse;
my @states = split(/\s/, $sequence_of_states) ;
for(my $i = scalar(@states) -1 ; $i >= 0; $i --)
{

    if($states[$i] =~ m/start/) {
	$reverse .= "rstart ";
    }elsif($states[$i] =~ m/stop/) {
	$reverse .= "rstop ";
    }elsif($states[$i] =~ m/(acc\d)/) {
	$reverse .= " r".$1." ";
    }elsif($states[$i] =~ m/(don\d)/) {
	$reverse .= " r".$1." ";
    }elsif($states[$i] =~ m/^(I\d)/) {
	$reverse .= " r".$1." ";
    }elsif($states[$i] =~ m/(EI\d)/) {
	$reverse .= " r".$1." ";
    }elsif($states[$i] =~ m/(ET\d)/) {
	$reverse .= " r".$1." ";
    }elsif($states[$i] =~ m/(E\d\d)/) {
	$reverse .= " r".$1." ";
    }elsif($states[$i] =~ m/(ES)/) {
	$reverse .= " r".$1." ";
    }elsif($states[$i] =~ m/N/) {
	$reverse .= " N ";
    }
}
$reverse =~ s/\s+/ /g;
print "REV_STATES:\t".$reverse."\n";





