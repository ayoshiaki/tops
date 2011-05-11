#!/usr/bin/perl



my $version = `git branch -v`;
$version =~ s/no branch/no_branch/g;
my @lines = split (/\n/, $version);
foreach my $l (@lines) {
  if($l =~ m/^\*/) {
    my @fields = split (/\s+/, $l) ;
    my $version = $fields[1]." ".$fields[2];
    my $src =<<EOF;
#ifndef VERSION_HPP
#define VERSION_HPP
#define APP_VERSION \"\\"$version\\"\"
#endif
EOF
    open (OUT, "> src/version.hpp") or die "cant open src/version.hpp: $!\n";
    print OUT $src;
    close(OUT);
    print STDERR "Version: $version\n";
  }
}

