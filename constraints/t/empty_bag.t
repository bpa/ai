use Test::More;
use t::Util;
tie *STDOUT, 't::Util';
use Data::Dumper;

ok(require 'bagger', 'loaded script') or exit;
main::parse_file(DATA);
is($main::bags, 5);
is($main::size, 12);
is(tied(*STDOUT)->lines, 1);
is(tied(*STDOUT)->[0], 'success');
done_testing();

__DATA__
5 stuff that doesn't matter
12 //some junk
