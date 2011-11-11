use Test::More;
use Data::Dumper;
ok(require 'bagger', 'loaded script') or exit;
my $a = new item('a 2');
my $b = new item('b 2 + b c');
my $c = new item('c 2 - a c');

ok($a->bagable_with($a));
ok($a->bagable_with($b));
ok($a->bagable_with($c));

ok(!$b->bagable_with($a));
ok( $b->bagable_with($b));
ok( $b->bagable_with($c));

ok(!$c->bagable_with($a));
ok( $c->bagable_with($b));
ok(!$c->bagable_with($c));

ok( $a->is_compatable($a));
ok(!$a->is_compatable($b));
ok(!$a->is_compatable($c));
ok( $b->is_compatable($b));
ok( $b->is_compatable($c));
ok(!$c->is_compatable($c));

done_testing;
