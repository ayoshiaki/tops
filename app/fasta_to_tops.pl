#!/usr/bin/perl
use strict;
use warnings;

use Bio::SeqIO;

my $in = Bio::SeqIO->new(-fh => \*STDIN, '-format' => 'Fasta');
while (my $seq = $in -> next_seq())
{
    my $name = $seq->id();
    $name =~ s/\t/ /g;
    $name =~ s/^\s+//;
    $name =~ s/\s+$//;
    my @symbols = split("", $seq->seq());
    print "$name:\t".join(" ", @symbols)."\n";
}
