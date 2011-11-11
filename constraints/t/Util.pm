package t::Util;

sub TIEHANDLE { bless [], shift; }

sub PRINT {
	my $self = shift;
	push @$self, join($,, @_);
}

sub PRINTF {
	my $self = shift;
	push @$self, sprintf(@_);
}

sub lines {
	my $self = shift;
	return scalar @$self;
}

1;
