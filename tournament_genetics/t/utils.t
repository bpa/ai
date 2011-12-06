use Test::More;
BEGIN { use_ok("utils") }

my $set = bless {}, 'set';
$set->build({marker=>'colors', datatype=>'set', validation=>'red,orange,yellow,green,blue'});
is(scalar(@{$set->{set}}), 5);
is($set->{name}, 'colors');
my $t = $set->rand;
ok(grep{ $_ eq $t } qw/red orange yellow green blue/);

my $range = bless {}, 'range';
$range->build({marker=>'size', datatype=>'range', validation=>'3..8'});
is($range->{name}, 'size');
is($range->{low}, 3);
is($range->{high}, 8);
my @dist;
for (1 .. 1000) {
	$dist[$range->rand]++;
}
ok(not defined $dist[2]);
ok($dist[$_] > 0, "got $_ at least once") for 3 .. 8;
ok(not defined $dist[9]);

done_testing;
