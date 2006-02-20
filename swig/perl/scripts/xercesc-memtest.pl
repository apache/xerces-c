# use blib;
use strict;
use Getopt::Long;
use Linux::MemInfo;
use Proc::ProcessTable;
use IO::Handle;

my %OPTIONS;
$OPTIONS{count} = 1000;
$OPTIONS{dom} = 1;
my $rc = GetOptions(\%OPTIONS,
		    'file=s',
		    'program=s',
		    'help',
		    );
my $USAGE = <<"EOU";
usage: $0 [required flags]
  required flags:
    --file=file_name     : the XML file to parse
    --program=prog_name  : the Xerces-C program to run

  optional parameters:
    --help            : print this message
EOU

die "$rc\n$USAGE" unless $rc;
die $USAGE if exists $OPTIONS{help};

die "Must specify --program\n$USAGE"
  unless exists $OPTIONS{program};

my $logfile = '/tmp/xercesc-memtest.log';
# open(LOG,">$logfile")
#    or die "Couldn't open $logfile for reading";

# get the initial results:
my %info = get_mem_info();
my $starting_mem = $info{mem_free};

STDERR->autoflush();

my $starting_size = 0;
my $pid;
if ($pid = fork) {
  print STDERR "Parent: $$, child: $pid\n";
  while (1) {
    sleep(1);
    if ($starting_size) {
      my ($proc) = get_proc($pid);
      my $size = $proc->size();
      my $leak = $size - $starting_size;
      my ($size_unit,$leak_unit);
      ($size,$size_unit) = get_val_unit($size);
      ($leak,$leak_unit) = get_val_unit($leak);
      printf STDERR "total mem: %d%s, leaked mem: %d%s\n",
	$size,
	$size_unit,
	$leak,
	$leak_unit,
	  ;
    } else {
      my ($proc) = get_proc($pid);
      $starting_size = $proc->size();
      printf STDERR "starting mem: %dk\n", int($starting_size/1024);
    }
  }
} else {
  print STDERR "execution loop: Parent: $$, child: $pid\n";
  my @args = ($OPTIONS{program});
  if (exists $OPTIONS{file}) {
    push(@args, $OPTIONS{file});
  }
  # push(@args," > $logfile 2>&1");
  exec(join(" ", @args))

}

sub get_val_unit {
  my $val = shift;
  my $unit;
  if ($val > 2 * 1024) {
    $unit = 'k';
    $val = int($val/1024);
  } else {
    $unit = 'b';
  }
  return ($val,$unit)
}

sub get_proc {
  my $pid = shift;
  return grep {$_->pid == $pid} @{Proc::ProcessTable->new->table};
}

