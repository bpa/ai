use Test::More;
use IO::Scalar;

ok(require 'bagger', 'loaded script') or exit;
my $output;
my $stdout = new IO::Scalar \$output;
main::parse_file(DATA);
is($main::bags, 5);
is($main::size, 12);

main::bag_items($stdout);
is($output, "success\n");

opendir(my $dh, 't') || die "can't opendir t: $!";
@tests = grep { /\.txt$/ && -f "t/$_" } readdir($dh);
closedir $dh;

for $test (@tests) {
	open my $t, "t/$test";
	local $/ = "--split\n";
	my ($in, $expected) = <$t>;
	$in =~ s/--split\n//;
	close $t;
	main::parse_file(new IO::Scalar \$in);
	$output = '';
	main::bag_items($stdout);
	is($output, $expected, $test);
}

done_testing();

__DATA__
5 stuff that doesn't matter
12 //some junk
