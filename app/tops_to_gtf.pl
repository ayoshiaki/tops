#!/usr/bin/env perl
use strict;
use warnings;

use Getopt::Long;

my $start_offset_begin = 10;
my $start_offset_end = 16;
my $stop_offset_begin = 10;
my $donor_offset_begin = 5;
my $acceptor_offset_end = 5;



while (my $seq = <STDIN>)
{
    my $name;
    my @sequence;
    if($seq =~ m/^>(.*)/) {
	$name = $1;
	my @names = split(/:/, $name);
	$name = $names[0];
	$name =~ s/\s/_/g;
	my $str = <STDIN>;
	chomp($str);
	@sequence = split(/ /, $str);
    } else {
	my @seqentry = split(/:\t/, $seq);
	$name = $seqentry[0];
	my @names = split(/:/, $name);
	$name = $names[0];
	$name =~ s/\s/_/g;
	my $str = $seqentry[1];
	@sequence = split(/ /, $str);
    }
    my $current_state = $sequence[0];
    my $begin = 0;
    push @sequence, "+_+_+_+_+_+_+";
    my $id = 0;
    my $frame = 0;
    for(my $i = 1; $i < scalar(@sequence)-1; $i++)
    {
	if(!($sequence[$i] eq $current_state)){
	    my $nameid = $name.".$id";
	    if($current_state eq "start") {
		$frame  = 0;
		print "$name\ttops\tstart_codon\t".($begin + $start_offset_begin+1)."\t".($begin + $start_offset_begin+3)."\t.\t+\t".$frame."\tgene_id \"$nameid\"; transcript_id \"$nameid\";\n";
	    } elsif( $current_state eq "stop") {
		print "$name\ttops\tstop_codon\t".($begin + $stop_offset_begin+1)."\t".($begin + $stop_offset_begin+3)."\t.\t+\t".((3-$frame)%3)."\tgene_id \"$nameid\"; transcript_id \"$nameid\";\n\n";
		$id ++;
	    } elsif ( $current_state =~ m/EI(\d)/) {
		$frame =  0;
		print "$name\ttops\tCDS\t".($begin - $start_offset_end + 1)."\t".($i + $donor_offset_begin )."\t.\t+\t".$frame."\tgene_id \"$nameid\"; transcript_id \"$nameid\";\n";
		$frame =(($i + $donor_offset_begin-1)-($begin - $start_offset_end ))%3;
	    }elsif ($current_state =~ m/^E(\d)(\d)/) {
		$frame = $1;
		print "$name\ttops\tCDS\t".($begin - $acceptor_offset_end+1)."\t".($i + $donor_offset_begin )."\t.\t+\t".((3-$frame)%3)."\tgene_id \"$nameid\"; transcript_id \"$nameid\";\n";
		$frame = ( $frame + ($i + $donor_offset_begin - 1)-($begin - $acceptor_offset_end) )%3;
	    }elsif ($current_state =~ m/^ET(\d)/) {
		$frame = $1;
		print "$name\ttops\tCDS\t".($begin - $acceptor_offset_end+1)."\t".($i + $stop_offset_begin )."\t.\t+\t".((3-$frame)%3)."\tgene_id \"$nameid\"; transcript_id \"$nameid\";\n";
		$frame = ( $frame + ($i + $stop_offset_begin-1) - ($begin - $acceptor_offset_end) )%3;
	    }elsif ($current_state =~ m/^ES/) {
		$frame = 0;
		print "$name\ttops\tCDS\t".($begin - $start_offset_end+1)."\t".($i + $stop_offset_begin )."\t.\t+\t".$frame."\tgene_id \"$nameid\"; transcript_id \"$nameid\";\n";
	    } 
	    $begin = $i;
	    $current_state = $sequence[$i];
	}
    }
}

