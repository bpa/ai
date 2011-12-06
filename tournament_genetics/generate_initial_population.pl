#! /usr/bin/perl

use strict;
use warnings;

use utils;
use File::Slurp;

my $template = load_file('template.java');
my @markers = markers;
for (1 .. 20) {
	$dbh->do("insert into individual (active) values (1)");
	my $id = $dbh->last_insert_id("","","","");
	foreach my $m (@markers) {
		
		$dbh->do("insert into genes (individual_id, marker, value) values (?,?,?)", {}, $id, $m->{name}, $m->rand);
	}
}
