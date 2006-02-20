package SWIG;
use vars qw(@ISA @EXPORT_OK);
require Exporter;
@ISA = qw(Exporter);

@EXPORT_OK =  qw(remove_method 
		 skip_to_closing_brace
		 fix_method);

sub remove_method {
  my ($in_fh) = @_;
  skip_to_closing_brace($in_fh);
}

sub skip_to_closing_brace {
  my ($in_fh,$subst_line_func,$out_fh) = @_;
  if (defined $subst_line_func) {
    die "skip_to_closing_brace: Must supply a CODE ref for substitution method"
      unless ref($subst_line_func) eq 'CODE';
  } else {
    $subst_line_func = 0;
  }
  my $braces = 1;
  while ($braces && ! eof($in_fh)) {
    $_ = <$in_fh>;
    if (/\}/ and /\{/) {
      my $tmp = $_;
      $tmp =~ s/\{[^\}]*\}//g;
      $braces-- if $tmp =~ /\}/;
      $braces++ if $tmp =~ /\{/;
    } else {
      $braces-- if /\}/;
      $braces++ if /\{/;
    }
    if ($subst_line_func) {
      $subst_line_func->($_);
      print $out_fh $_;
    }
  }
  if ($braces) {
    print STDERR "skip_to_closing_brace exited with positive brace count";
  }
}

sub fix_method {
  my ($in_fh,$out_fh,$match_pattern,$subst_string,$keep,$subst_line_func) = @_;
  $keep = 0 unless defined $keep;
  if (defined $subst_line_func) {
    die "fix_method: Must supply a CODE ref for substitution method"
      unless ref($subst_line_func) eq 'CODE';
  } else {
    $subst_line_func = 0;
  }
  my $braces = 1;
  print $out_fh $_;
  while ($braces && ! eof($in_fh)) {
    $_ = <$in_fh>;
    $braces-- if /\}/;
    $braces++ if /\{/;

    # we substitute the line only if we need to
    $subst_line_func->($_)
      if $subst_line_func;

    if (/$match_pattern/) {
      if ($keep) {
	$_ .= $subst_string;
      } else {
	s/^.*\n/$subst_string\n/;
      }
    }
    print $out_fh $_;
  }
  if ($braces) {
    print STDERR "fix_method exited with positive brace count";
  }
}

