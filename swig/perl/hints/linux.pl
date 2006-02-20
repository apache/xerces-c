# until I get ahold of Detect::OS, I assume all linux is debian
$self->{CCFLAGS} .= " -DDEBIAN -D_GNU_SOURCE";
