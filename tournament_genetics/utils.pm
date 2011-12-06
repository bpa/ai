package utils;

use strict;
use warnings;

use DBI;
use Exporter 'import';

our @EXPORT = qw/$dbh markers/;

our $dbh = DBI->connect("dbi:SQLite:dbname=db","","", {RaiseError=>1});

sub markers {
	my @markers;
	my $genes = $dbh->selectall_arrayref("select * from markers", { Slice => {} });
	foreach (@$genes) {
		my $gene = bless {}, $_->{datatype};
		$gene->build($_);
		push @markers, $gene;
	}
	return @markers;
}

package range;

sub build {
	my ($self, $row) = @_;
	$self->{name} = $row->{marker};
	($self->{low}, $self->{high}) = split /\s*\.\.\s*/, $row->{validation};
}

sub rand {
	my $self = shift;
	my $range = $self->{high} - $self->{low} + 1;
	return int(rand($range)) + $self->{low};
}

package set;

sub build {
	my ($self, $row) = @_;
	$self->{name} = $row->{marker};
	$self->{set} = [split /\s*,\s*/, $row->{validation}];
}

sub rand {
	my $self = shift;
	return $self->{set}[int(rand(scalar(@{$self->{set}})))];
}

1;
